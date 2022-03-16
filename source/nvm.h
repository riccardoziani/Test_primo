/**
@file              nvm.h
@brief             nvm exported function's prototypes.
@details           This file contains all the nvm exported functions prototypes 
                   used by the modules in the project.

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


#ifndef nvm_H
#define nvm_H

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


/** @brief nvm Initialization.
    nvm initialization function, must be called
    only one time before the main loop and before
    the use of all other nvm functions.
    
    @see nvm_hal_init()
    @param none.
    @return none.
*/
void_t nvm_init (void_t);



/** @brief A function used to save data from ram to flash.
    This function checks all the conditions needed to writes
    data over the nvm region, after that, make the crc checksum
    of the nvm bank(CRC-ITT seed 0xFFFF), writes the bank,
    verify it, and finally erases the opposite nvm block if needed... 

    @param none.
    @return The results of the saving process
    @retval 1 SUCCESS
    @retval 0 Failure
*/
uint8_t nvm_save (void_t);



/** @brief A nvm enable function.
    This function enables/unlock the nvm_save to writes data over the nvm flash.

    @param none.
    @return none
*/
void_t nvm_enable (void_t);




/** @brief A function used initialize the nvm flash region.
    This function erases the nvm region and copy the default

    @param none.
    @return none
*/
void_t nvm_format (void_t);



/** @brief A function that checke/erase nvm block.
    This function checks if the opposite(not active) block
    is effectively erased, if not, erases it.

    @param none.
    @return none
*/
void_t nvm_check_erase_block (void_t);



///** @brief A function description .
//    detalied description and code example as
//    @fn nvm_check_and_save
//
//    @see related_function()
//    @return none
//*/
//void_t  nvm_check_and_save   	(void_t);





/** @brief A function description .
    detalied description and code example as 

    @param none.
    @return none
*/
void_t  nvm_load_network_cntr	(void_t);


#endif /* ifndef nvm_H */
