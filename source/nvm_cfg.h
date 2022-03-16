/**
@file              nvm_cgf.h
@brief             This is a config files.
@details           A inteface file between nvm.c module anf physical microntroller flash...
@author            Riccardo Ziani
@version           $Id$ 30/03/2018
@warning           Improper use can crash your application

@copyright         Copyright(C) 2017 Koninklijke Philips N.V., All Rights Reserved.
                   This source code and any compilation or derivative thereof is the
                   information of Koninklijke Philips N.V. is confidential in nature.
                   no circumstances is this software to be exposed to or placed
                   an Open Source License of any type without the expressed
                   permission of Koninklijke Philips N.V.
*/


#ifndef nvm_cgf_H
#define nvm_cgf_H

/* ******************************************************************
                            INCLUDES
*********************************************************************/

#include "philips_types.h"

/* ******************************************************************
                            MACRO DEFINITIONS
 *********************************************************************/
#define FLASH_SECTOR_SIZE           (512)                              	/* Dimension of the flash sector size in byte */
#define FLASH_SECTORS               (128)                              	/* Total numbers of phisical flash sectors implemented in flash of the u */
#define FLASH_SIZE                  (FLASH_SECTOR_SIZE * FLASH_SECTORS)	

#define BLOCK_SIZE					(FLASH_SECTOR_SIZE)					/* Dimension of the one Block in byte.     */
#define NVM_SIZE					(BLOCK_SIZE*2)                     	/* Dimension of the entire nvm region in byte. */
#define BLOCK_0_ADDR                (uint8_t*)(FLASH_SIZE - NVM_SIZE)  	/* Address of the first byte of the first Block of the nvm region */
#define BLOCK_1_ADDR                (uint8_t*)(FLASH_SIZE - BLOCK_SIZE)	/* Adderess of the first byte of the second Block of the nvm region */

#define NVM_BANK_SIZE               (BLOCK_SIZE/2)                     	/* Dimension of one bank of the nvm, in bytes */ 
#define NUMBER_OF_BANKS             (NVM_SIZE/NVM_BANK_SIZE)           	

#if NVM_SIZE%NVM_BANK_SIZE                                             	
  #error nvm size does not supported by module.                        	
#endif                                                                 	

#define NVM_MAP_VERSION_0x1         (0x01)                             	/* Number of the first Memory Map version */ 
#define NVM_MAP_VERSION             (NVM_MAP_VERSION_0x1)              	

#define HEADER_WORD                 (0xAABB)                           	/* First half word of the nvm buffer, use by nvm algorithm to serch the latest valid bank */ 
#define FOOTER_WORD                 (0xCCDD)                           	/* Last half word of the nvm buffer, use by nvm algorithm to serch the latest valid bank */

#define NVM_ALLOW_WRITE             (0xAA)                             	/* Key to allow/unlock nvm writes/erases in flash */
#define NVM_DONT_ALLOW_WRITE        (0x55)                             	/* Key to lock nvm writes/erases in flash */

#define TIMEOUT_WRITE               (3)                                	/* Number of zx's lost, after that the system saves buffer sector in nvm... */ 
#define ZC_FILTER                   (50)                               	/* Number of zx's to wait from a write to another. */ 

/* ******************************************************************
                          EXPORTED TYPES
*********************************************************************/


/* ******************************************************************
                    EXPORTED GLOBAL VARIABLES
*********************************************************************/


/* ******************************************************************
                    EXPORTED GLOBAL FUNCTIONS
*********************************************************************/

#endif /* ifndef nvm_cgf_H */