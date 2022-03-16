/**
@file           	nvm_hal.h 
@brief          	Hardware Level Abstract file interface for nvm_hal.
@details        	A file that contains the prototypes of the functions used by nvm.c 
    				to interface it with the low-level HW functions specific for the
					microcontroller used in the project.

@author	        	Riccardo Ziani
@version        	$Id$ 30/03/2018
@warning        	Improper use can crash your application

@copyright      	Copyright(C) 2017 Koninklijke Philips N.V., All Rights Reserved.
					This source code and any compilation or derivative thereof is the
					information of Koninklijke Philips N.V. is confidential in nature.
					no circumstances is this software to be exposed to or placed
					an Open Source License of any type without the expressed
					permission of Koninklijke Philips N.V.
*/


#ifndef NVM_HAL_H
#define NVM_HAL_H

/* ******************************************************************
                            INCLUDES
*********************************************************************/

/* include phlips global file and all other headers */  
#include "philips_types.h"
 
/* ******************************************************************
                            MACRO DEFINITIONS
 *********************************************************************/


/* ******************************************************************
                          EXPORTED TYPES
*********************************************************************/


/* ******************************************************************
                    EXPORTED GLOBAL VARIABLES
*********************************************************************/


/* ******************************************************************
                    EXPORTED GLOBAL FUNCTIONS
*********************************************************************/


/** @brief A function used to erase a single block.
    This function launch a specific microcontroller-dependent
    function that erases a single nvm block that it can be composed by
    a single or multiple physical sectors of the nvm flash region.
    
    @warning This func calls a low level func that disable all
    interrupts during flash operations...

    @see FLASH_EraseSector()
    @param sector_address of the firt byte of the flash sector.
    @return none
*/
void_t nvm_hal_erase_block(uint32_t sector_address);



/** @brief A specific low level flash initialization.
    This function launch a specific microcontroller-dependent
    function that initialize all the registers used by the microcontroller
    in order to use all the specific nvm flash functions properly.
    
    @warning This func calls a low level func that disable all
    interrupts during flash operations...
    
    @see FLASH_Init()
    @return none
*/
void_t nvm_hal_init(void_t);




/** @brief Low level crc function.
    This function executes a CRC-ITT checksum of the buffer
    pointed by the *buff for an amount of bytes passed from param size.
    The algorithm uses a fixed seed of 0xFFFF.
    Although a crc-ccitt algorithm uses a 16 bits result we use a 32 bits
    result in order to maintain the compatibility all others methods(32 bits...). 
    
    @see CRC_Cal16()
    @param uint8_t *buff, pointer for the buffer.
    @param size number of the bytes used by crc algorithm.
    @return the value of the crc calculations
*/
uint32_t nvm_hal_crc(uint8_t *buff, uint32_t size);    



/** @brief Writes a bank in a flash block.
    This function launch a low level routine that writes 
    a bank in the active block in nvm region.
    
    @warning This func calls a low level func that disable all
    interrupts during flash operations...
    
    @see FLASH_Write_Bank()
    @param flash_address is the address of the first byte of the bankin nvm region.
    @param ptr_nvm_ram the pointer of the nvm ram structure the second argument.
    @param nvm_bank_size the amount of bytes to writes.
    @return none.
*/
void_t nvm_hal_write_bank(uint32_t flash_address, uint32_t* ptr_nvm_ram, uint32_t nvm_bank_size);

#endif /* NVM_HAL_H */