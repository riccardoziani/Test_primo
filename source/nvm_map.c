/**
@file              nvm_map.c
@brief             define the structure of the nvm
@details           This class is used to demonstrate a number of section commands.
@author            Riccardo Ziani 03/04/2018
@version           $Id$
@warning           Improper use can crash your application

@copyright         Copyright(C) 2017 Koninklijke Philips N.V., All Rights Reserved.
                   This source code and any compilation or derivative thereof is the
                   information of Koninklijke Philips N.V. is confidential in nature.
                   no circumstances is this software to be exposed to or placed
                   an Open Source License of any type without the expressed
                   permission of Koninklijke Philips N.V.
*/


/* include module header */ 
#include "nvm_cfg.h"
#include "nvm_map.h"


/* ******************************************************************
                    NON EXPORTED LOCAL TYPES
*********************************************************************/


/* ******************************************************************
                     GLOBAL VARIABLES                                       
*********************************************************************/

__root  const nvm_t nvm_defaults =
{
/* 0    */  NVM_HEDER,                          
            0,                                  // Sistema: versione firmware
/*      */  DEFAULT_NVM_SAS_STRUCTURE,          //  autodose system structure. 40 bytes
/*      */  DEFAULT_NVM_GRINDER_STRUCTURE,      //  24 bytes
/*      */  STATUSFLAG,                         //  flag di stato della macchina
/*      */  0,                                  //  flag dello stato del gruppo
/*      */  25,                                 //  Caldaia: temperatura della caldaia allo spegnimento
/*      */  MAX_DREG_NUMBER,                    //  dreg number to fill the drawer
/*      */  0,                                  //  

/*      */  0,0,0,0,0,0,0,0,0,0,0,              // 11 bytes

            // 16 bit                           

/* 100  */  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 16 bytes
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 16 Bytes
            0,0,0,0,                            // 4 Bytes

            // 32 bit                           

/* 240  */  0,                                  //  
/* 244  */  0,                                  //  
/* 164  */  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 64 bytes    
            0,0,0,                              // 3 Bytes

/* 254  */  NVM_FOOTER,                         // Coda di fine struttura
};                                              

#pragma data_alignment=4
nvm_t   nvm;

/* ******************************************************************
                     LOCAL VARIABLES                                       
*********************************************************************/


/* ******************************************************************
                LOCAL FUNCTIONS DECLARATIONS
*********************************************************************/


/* ******************************************************************
                    ALL EXPORTED FUNCTIONS IMPLEMENTATIONS
*********************************************************************/


/* ******************************************************************
                    ALL LOCAL FUNCTIONS IMPLEMENTATIONS
*********************************************************************/
