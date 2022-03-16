/**
@file              nvm_hal.c
@brief             Hardware Level Abstract file interface.
@details           A file that contains the the functions used by nvm.c 
                   to interface it with the low-level HW functions specific for the
                   microcontroller used in the project.
@author	           Riccardo ziani
@version           $Id$ 30/03/2018
@warning           Improper use can crash your application

@copyright         Copyright(C) 2017 Koninklijke Philips N.V., All Rights Reserved.
                   This source code and any compilation or derivative thereof is the
                   information of Koninklijke Philips N.V. is confidential in nature.
                   no circumstances is this software to be exposed to or placed
                   an Open Source License of any type without the expressed
                   permission of Koninklijke Philips N.V.
*/


/* include module header */ 
#include "nvm_hal.h"
#include "crc.h"
#include "flash.h"


/* ******************************************************************
                    NON EXPORTED LOCAL TYPES
*********************************************************************/
#define SEED 0xFFFF

/* ******************************************************************
                     GLOBAL VARIABLES                                       
*********************************************************************/


/* ******************************************************************
                     LOCAL VARIABLES                                       
*********************************************************************/


 
#ifdef DEBUG
static volatile uint32_t write_timer;
static volatile uint32_t erase_timer;

#endif /* DEBUG */

/* ******************************************************************
                LOCAL FUNCTIONS DECLARATIONS
*********************************************************************/


/* ******************************************************************
                    ALL EXPORTED FUNCTIONS IMPLEMENTATIONS
*********************************************************************/

void_t nvm_hal_erase_block(uint32_t sector_address)
{
    #ifdef DEBUG
        erase_timer  = PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1);
        if (FLASH_ERR_SUCCESS != FLASH_EraseSector(sector_address))
        {
            asm ("BKPT #0");
        }
        erase_timer = (erase_timer - PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1)) >> 4; // useconds..
    #else
        if (FLASH_ERR_SUCCESS != FLASH_EraseSector(sector_address))
        {
        }
    #endif /* DEBUG */
        return;
}

void_t nvm_hal_init(void_t)
{
    #ifdef DEBUG
        if (FLASH_ERR_SUCCESS != FLASH_Init(SystemCoreClock))
        {
            asm ("BKPT #0");    /* break instruction */
        }                       
    #else
        if (FLASH_ERR_SUCCESS != FLASH_Init(SystemCoreClock))
        {
        }                       
    #endif /* DEBUG */
        return;
}

void_t nvm_hal_write_bank(uint32_t flash_address, uint32_t* ptr_nvm_ram, uint32_t nvm_bank_size)
{
    #ifdef DEBUG
        write_timer  = PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1);
        if (FLASH_ERR_SUCCESS != FLASH_Write_Bank(flash_address, ptr_nvm_ram, nvm_bank_size))
        {
            asm ("BKPT #0");
        }
        write_timer = (write_timer - PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1)) >> 4; // useconds..
    #else
        if (FLASH_ERR_SUCCESS != FLASH_Write_Bank(flash_address, ptr_nvm_ram, nvm_bank_size))
        {
        }
    #endif /* DEBUG */
        return;
}

uint32_t nvm_hal_crc(uint8_t *buff, uint32_t size)    
{
    return (CRC_Cal16((uint32_t)SEED, buff, size));    
}

/* ******************************************************************
                    ALL LOCAL FUNCTIONS IMPLEMENTATIONS
*********************************************************************/
