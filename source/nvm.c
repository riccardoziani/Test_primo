/**
@file              nvm.c
@brief             nvm manager file.
@details           This file contains all the functions needed to manage the access to the Non Volatile Memory (nvm).
				   At start-up the nvm_init retrives the last nvm_ram struct saved in the previous session and 
				   provides the specific functions to check the coherency of the buffer that will be write at 
				   the end of the session.

@author            Riccardo Ziani 03/04/2018
@version           $Id$
@warning		   This file uses a minimum nvm size of a 2 Blocks whit a minim 2 Banks per Block and a 
				   single Block mustn't be lower than a single flash sector implemented in the uController used.
				   Improper use can crash your application

@copyright         Copyright(C) 2017 Koninklijke Philips N.V., All Rights Reserved.
                   This source code and any compilation or derivative thereof is the
                   information of Koninklijke Philips N.V. is confidential in nature.
                   no circumstances is this software to be exposed to or placed
                   an Open Source License of any type without the expressed
                   permission of Koninklijke Philips N.V.
*/

/* include module header */ 
#include "nvm.h"
#include "nvm_cfg.h"
#include "nvm_hal.h"
#include "nvm_map.h"

/* ******************************************************************
                    NON EXPORTED LOCAL TYPES
*********************************************************************/

/** An enum type. 
Structure used to control the nvm read and write process.
*/ 
typedef struct
{
	uint8_t nvm_enable;     /* Flag used in nvm lock/unlock mechanism */
	uint8_t enable_write;	/* Flag used for enable/disable the nvm for a certain period after a write */
	uint8_t read_OK;        /* Flag used to register a good bank found in a nvm_init serching algorithm  */
	uint8_t *ptr_read;		/* Contains the pointer used for the read of the last a bank in the nvm */
	uint8_t *ptr_write;		/* Contains the pointer used for the next write over the nvm */
	uint8_t timer;			/* Counters used in the lock/unlock debounce mechanism against zero crossing multiple writes...*/
	uint8_t zero_x_cntr;	/* Counters of the number of the zx events after a write*/
}nvm_control_t;          


/* ******************************************************************
                     GLOBAL VARIABLES                                       
*********************************************************************/


/* ******************************************************************
                     LOCAL VARIABLES                                       
*********************************************************************/

static  volatile    nvm_control_t	nvm_control;
static              nvm_t*       	p_nvm;

static  volatile    uint32_t     	nvm_flash_bits;

static              uint8_t*     	block_0_addr;
static              uint8_t*     	block_1_addr;
static              uint32_t     	flash_size;

static  volatile    uint32_t     	size_nvm = sizeof(nvm);

#ifdef DEBUG                     	
static  volatile    uint32_t     	write_timer;
static  volatile    uint32_t     	erase_timer;

#pragma data_alignment=4
static __root volatile  const  uint16_t nvm_address_buffer[][2] =
{																											//  address, size 
	/*   0  */  {(uint8_t*)&	nvm.heder               - (uint8_t*)&nvm,   sizeof( nvm.heder               )},	// 
	/*   1*/    {(uint8_t*)&	nvm.fw_ver              - (uint8_t*)&nvm,   sizeof( nvm.fw_ver              )},	// 
	/*   2*/    {(uint8_t*)&	nvm.sas                 - (uint8_t*)&nvm,   sizeof( nvm.sas                 )},	// 
	/*   3*/    {(uint8_t*)&	nvm.gr                  - (uint8_t*)&nvm,   sizeof( nvm.gr                  )},	// 
	/*   4*/    {(uint8_t*)&	nvm.status_flag         - (uint8_t*)&nvm,   sizeof( nvm.status_flag         )},	// 
	/*   5*/    {(uint8_t*)&	nvm.bu_status           - (uint8_t*)&nvm,   sizeof( nvm.bu_status           )},	// 
	/*   6*/    {(uint8_t*)&	nvm.shut_down_temp      - (uint8_t*)&nvm,   sizeof( nvm.shut_down_temp      )},	// 
	/*   7*/    {(uint8_t*)&	nvm.dregCounter         - (uint8_t*)&nvm,   sizeof( nvm.dregCounter         )},	// 
	/*   8*/    {(uint8_t*)&	nvm.last_error_logged   - (uint8_t*)&nvm,   sizeof( nvm.last_error_logged   )},	// 
	/*   9*/    {(uint8_t*)&	nvm._8bit_arr           - (uint8_t*)&nvm,   sizeof( nvm._8bit_arr          	)}, // 
	/*  10*/    {(uint8_t*)&	nvm._16bit_arr          - (uint8_t*)&nvm,   sizeof( nvm._16bit_arr          )},	// 
	/*  11*/    {(uint8_t*)&	nvm.last_standby_time   - (uint8_t*)&nvm,   sizeof( nvm.last_standby_time   )},	// 
	/*  12*/    {(uint8_t*)&	nvm.total_standby_time  - (uint8_t*)&nvm,   sizeof( nvm.total_standby_time  )},	// 
	/*  13*/    {(uint8_t*)&	nvm._32bit_arr          - (uint8_t*)&nvm,   sizeof( nvm._32bit_arr          )},	// 
	/*  14*/    {(uint8_t*)&	nvm.footer              - (uint8_t*)&nvm,   sizeof( nvm.footer              )},	// 
};                                                                                                                 
#endif /* DEBUG */


/* ******************************************************************
                LOCAL FUNCTIONS DECLARATIONS
*********************************************************************/
/** @brief	Erase a flach block. It Checks if the block pointed by the ptr_lock 
			pointer is really erased otherwise it erase that immediately.
    @see is_nvm_empty_buffer()
    @see nvm_hal_erase_block()
    @param the first address of the block that we want erase.
    @return none
*/
static void_t nvm_erase_block (uint8_t* ptr_block);




/** @brief Copy a bank from flash to ram.
    It copy of an nvm bank from a specific
    location of the flash pointed by ptr_block. 
    @param  the first address of the bank that we want read from flash.
    @return none
*/
static void_t nvm_read_bank (uint8_t* ptr_bank);     




/** @brief It executes the crc of a nvm bank.
    It executes the crc checksum of a specific
    nvm bank and returns the value of the crc.
    @see nvm_hal_crc()
    @param  the first address of the bank that we want make the crc.
    @return The test results of the crc
*/
static uint32_t nvm_make_bank_chk (uint8_t* buff);




/** @brief It checks if a buffer is empty.
    It checks if a buffer is full-filled
    with erased value(0xFF) for a length
    specified by parameter size, and
    returns true if it is completely erased. 
    
    @param the first address of the buffer that we want check if it is erased.
    @param the amount of bytes that we want check if it is erase.
    @return The result of the check.
    @retval 1 SUCCESS, all byte are 0xFF.
    @retval 0 Failure, at leat obe byte isn't 0xFF...
*/
static uint8_t is_nvm_empty_buffer (uint8_t* buff, uint16_t size);




/** @brief It checks if the pointer ptr is overflowing
    from the nvm space, if so, it restarts from
    the initial point of the nvm.
 
    @param the pointer to a pointer to first byte of the next bank
    @return none
*/
static void_t nvm_check_boundary (uint8_t** ptr);




/** @brief Ckecks the integrity of the header word and the footer word.
    @param the first address of the bank that we want check.
    @return The result of the check.
    @retval 1 SUCCESS, cohernt...
    @retval 0 Failure, not cohernt...
*/
static uint8_t nvm_check_head_and_foot (uint16_t* ptr);



/* ******************************************************************
                    ALL EXPORTED FUNCTIONS IMPLEMENTATIONS
*********************************************************************/


void_t nvm_init (void_t)
{
	bool_t check_result = FALSE;
	uint8_t* local_ptr_read;


	#ifdef DEBUG
		(void_t)nvm_address_buffer;
	#endif /* DEBUG */

	if (size_nvm != NVM_BANK_SIZE)
	{
	#ifdef DEBUG
		asm ("BKPT #0");
	#endif /* DEBUG */
	}

	flash_size = FLASH_SIZE;
	block_0_addr = BLOCK_0_ADDR;
	block_1_addr = BLOCK_1_ADDR;

	local_ptr_read = block_0_addr;
	nvm_control.ptr_write = block_0_addr;
	nvm_control.read_OK = FALSE;

	nvm_hal_init();

	/* Start searching... */
	for (uint8_t banks = NUMBER_OF_BANKS + 1; banks ; banks--)
	{
		if (nvm_check_head_and_foot((uint16_t*)local_ptr_read))
		{	/* If the bank was not erased...*/
			p_nvm = (nvm_t*)local_ptr_read;
			if (nvm_make_bank_chk(local_ptr_read) == p_nvm->footer.checksum)
			{	/* Coherency check... */
				/* ...if yes it reads the bank and sets that it is a good bank */
				nvm_read_bank(local_ptr_read);
				nvm_control.ptr_read = local_ptr_read;
				nvm_control.read_OK = TRUE;	/* sets that this bank is good, almost a one bank has been found */
			}
		}
		else
		{
			if (is_nvm_empty_buffer (local_ptr_read, NVM_BANK_SIZE))
			{	/* This bank results empty...*/
				nvm_control.ptr_write = local_ptr_read;/* Pointer preparation for a next writing */
				if (nvm_control.read_OK)
				{	/* If I had already picked up a good bank ...*/
					/* Now I'm sure that the last bank picked up was certainly also the last written before. */
					if (nvm.heder.machine_model != MACHINE_MODEL)
					{	/* If the change of memory model is required... */
						/* ...The nvm will be reformatted... */
						check_result = FALSE;	// Esci dal for e vai a formattare con parametri di default...
						break;
					}
					else
					{
						check_result = TRUE;	// Successfull...
						break;
					}
				}
			}
		}

		local_ptr_read += NVM_BANK_SIZE;
		nvm_check_boundary((uint8_t**)&local_ptr_read);
	}

	if (check_result == FALSE)
	{	/* ... no good bank found or is a change of memory model */
		nvm_format();
	}
	else
	{
		nvm_check_erase_block(); /* final check if the opposite block is empty... */
		if (nvm.heder.map_version != NVM_MAP_VERSION)
		{
			/* Here you can manage the change of memory map... */
			nvm.heder.map_version = NVM_MAP_VERSION;
		}
	}
	return;
}



void_t nvm_format (void_t)
{
	nvm_erase_block(block_0_addr);
	nvm_erase_block(block_1_addr);
	nvm_control.ptr_write = block_0_addr; 	/* a pointer init */
	nvm = nvm_defaults;						/* Load default parameters from flash to nvm ram buffer */ 
	nvm_enable();
	(void_t)nvm_save();						/* Save the first bank in the first nvm block */
	return;
}




void_t nvm_load_network_cntr (void_t)
{
	nvm_control.timer = TIMEOUT_WRITE;
	if (nvm_control.enable_write == FALSE)
	{
		/*  se sto aspettando di cancellare.. */
		if (nvm_control.zero_x_cntr > ZC_FILTER)
		{
			/*  Ok filtro superato ora puoi scrivere */
			nvm_control.enable_write = TRUE;
		}
		else
		{
			++nvm_control.zero_x_cntr;
		}
	}
	return;
}




uint8_t nvm_save (void_t)
{
	uint8_t result = FALSE;

	if (nvm_control.nvm_enable == NVM_ALLOW_WRITE)
	{	/* Checks some conditions required to allow nvm activity... */
		register uint32_t tmp;
		tmp = (uint32_t)nvm_control.ptr_write;
		if (nvm_check_head_and_foot((uint16_t*)&nvm)    &&	/* Integrity check, header and footer */
			tmp%NVM_BANK_SIZE == 0                      &&	/* Integrity check, the pointer must be a multiple of the nvm bank size*/
			nvm_control.ptr_write >= block_0_addr       &&	/* Integrity check, the pointer have to work inside of the nvm region...*/
			nvm_control.ptr_write < (uint8_t*)flash_size)
		{
			/* The write goes on only if the listed above conditions are satisfied... */
			nvm_control.ptr_read = nvm_control.ptr_write;
			nvm.footer.total_nvm_writes++;								/* Update the total numbers of writes */
			nvm.footer.checksum = nvm_make_bank_chk((uint8_t*)&nvm);	/* This operation must be the last before write. */

			nvm_hal_write_bank((uint32_t)nvm_control.ptr_write, (uint32_t*)&nvm, (uint32_t)NVM_BANK_SIZE);

			/* Verify if the bank is effectively written on the flash... */
			uint32_t* ptr_flash = (uint32_t*)nvm_control.ptr_write;
			uint32_t* ptr_nvm = (uint32_t*)&nvm;
			result = TRUE;
			for (uint8_t jj = NVM_BANK_SIZE/4; jj; jj--)
			{
				if (*ptr_nvm++ != *ptr_flash++)
				{
					#ifdef DEBUG
						asm ("BKPT #0");
					#endif /* DEBUG */
					result = FALSE;
					break;
				}
			}

			nvm_control.ptr_write += NVM_BANK_SIZE;
			nvm_check_boundary((uint8_t**)&nvm_control.ptr_write);	/* it Prepare the pointer for a successive writes. */

			nvm_control.enable_write = FALSE;
			nvm_control.zero_x_cntr = 0;
		}
		nvm_check_erase_block();	/* final check if the opposite block is empty... */
		nvm_control.nvm_enable = NVM_DONT_ALLOW_WRITE;
	}
	return(result);
}






//void_t nvm_check_and_save (void_t)
//{
//    if (nvm_control.timer == 0)
//    {
//        // Tre zero cross li ho persi andiamo avanti...
//        if (nvm_control.enable_write == FALSE)
//        {
//            // se non ho abilitazione a scrivere resetto antirimbalzo!!
//            nvm_control.zero_x_cntr = 0;  // Riparti a contare!!
//        }
//        else
//        {
//            // Ok persi N° TIMEOUT_WRITE zeri di rete consecutivi, dopo un periodo di
//            // ZC_FILTER zero cross stabili, mi sto spegnendo davvero, scrivi array!!!!
//            //Nvm_CheckIn();
//            nvm_enable();
//            if (nvm_save())
//            {
//                // Se la scrittura è andata a buon fine allora...
//                nvm_check_erase_block();              // cancello pagina opposta se ce ne bisogno.
//            }
//        }
//    }
//    else
//    {
//    }
//}



/* ******************************************************************
                    ALL LOCAL FUNCTIONS IMPLEMENTATIONS
*********************************************************************/

void_t nvm_enable (void_t)
{
	nvm_control.nvm_enable = NVM_ALLOW_WRITE;
}





void_t nvm_erase_block (uint8_t* ptr_block)
{
	volatile uint32_t primask;

	if (!is_nvm_empty_buffer (ptr_block, BLOCK_SIZE))
	{
		if (ptr_block == block_0_addr || ptr_block == block_1_addr)
		{
			nvm_hal_erase_block((uint32_t)ptr_block);
		}
	}
}





void_t nvm_check_erase_block (void_t)
{
	if (nvm_control.ptr_read >= block_1_addr)
	{
		nvm_erase_block(block_0_addr);
	}                                  	
	else                               	
	{                                  	
		nvm_erase_block(block_1_addr);
	}                                   	
}





void_t nvm_read_bank (uint8_t* ptr_bank)
{
	p_nvm = (nvm_t*)ptr_bank;
	nvm = *p_nvm;

}




static uint32_t nvm_make_bank_chk (uint8_t* buff)
{
	return(nvm_hal_crc(buff, (size_nvm - sizeof(nvm.footer))));    
}





uint8_t is_nvm_empty_buffer (uint8_t* buff, uint16_t size)
{
	bool_t ret = TRUE;
	uint32_t* ptr = (uint32_t*)buff;

	for (uint32_t ii = size/4; ii; ii--)
	{
		if (*ptr++ != 0xFFFFFFFF)
		{
			ret = FALSE;
			break;
		}
	}
	return(ret);
}




void_t nvm_check_boundary (uint8_t** ptr)
{
	if (*ptr >= (uint8_t*)flash_size)
	{
		*ptr = block_0_addr;
	}
}




uint8_t nvm_check_head_and_foot (uint16_t* ptr)
{
	bool_t ret = TRUE;
	p_nvm = (nvm_t*)ptr;

	if (p_nvm->heder.header_word == HEADER_WORD && p_nvm->footer.footer_word == FOOTER_WORD)
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
	return(ret);
}