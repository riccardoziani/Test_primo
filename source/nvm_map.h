/**                       
@file              nvm_map.h
@brief             Declare the structure of the nvm.
@details           This class is used to demonstrate a number of section commands.
@author            Riccardo Ziani
@version           $Id$  03/04/2018
@warning           Improper use can crash your application

@copyright         Copyright(C) 2017 Koninklijke Philips N.V., All Rights Reserved.
                   This source code and any compilation or derivative thereof is the
                   information of Koninklijke Philips N.V. is confidential in nature.
                   no circumstances is this software to be exposed to or placed
                   an Open Source License of any type without the expressed
                   permission of Koninklijke Philips N.V.
*/



#ifndef nvm_map_H
#define nvm_map_H

/* ******************************************************************
                            INCLUDES
*********************************************************************/

/* include phlips global file and all other headers */  
#include "saeco.h"
#include "autodose.h"
#include "grinder.h"

/* ******************************************************************
                            MACRO DEFINITIONS
 *********************************************************************/

#define	NVM_HEDER      {\
    HEADER_WORD,        \
    NVM_MAP_VERSION,    \
    MACHINE_MODEL,      \
    0,                  \
}                       

#define	NVM_FOOTER     {\
    0,                  \
    0,                  \
    FOOTER_WORD,        \
}

/* ******************************************************************
                          EXPORTED TYPES
*********************************************************************/

/** An enum type. 
    The documentation block cannot be put after the enum! 
*/
typedef struct
{               
    uint16_t    header_word;     
    uint16_t    map_version; 
    uint16_t    machine_model;  
    uint16_t    header_free;  
}header_t;

typedef struct
{
    uint32_t    checksum;        
    uint16_t    total_nvm_writes;
    uint16_t    footer_word;     
}footer_t;

typedef struct
{                                                       //  description
            header_t              heder;
            uint32_t                fw_ver;             //  Sistema: versione firmware
/*  48  */  Type_nvm_sas            sas;                //  autodose system structure. 40 bytes long */
/*  92 */   Type_nvm_gr             gr;                 //  24 bytes
/*  4  */   nvm_t_STATUS_FLAGS   status_flag;           //  flag di stato della macchina
/*  5  */   uint8_t                 bu_status;          //  flag dello stato del gruppo
/*  6  */   uint8_t                 shut_down_temp;     //  Caldaia: temperatura della caldaia allo spegnimento  
/*  7  */   uint8_t                 dregCounter;        //  number of dregs left before the drawer is full */
            uint8_t                 last_error_logged;  //  Sistema: ultimo errore verificatosi
/*  8  */   uint8_t                 _8bit_arr[11];            //
            
                        // 16 bit                                   
/* 160 */   uint16_t                _16bit_arr[36];     //  60 bytes
            
                        // 32 bit                                   
/* 162 */   
/* 242 */   uint32_t                last_standby_time;  //  
            uint32_t                total_standby_time; //  
/* 226 */   uint32_t                _32bit_arr[19];     //  64 bytes

            footer_t             footer;             //
}nvm_t;                                      


/* ******************************************************************
                    EXPORTED GLOBAL VARIABLES
*********************************************************************/

/* please limit the use of global variables accoarding to rule DI.2 */
extern	        nvm_t	nvm;
extern	const   nvm_t	nvm_defaults;


/* ******************************************************************
                    EXPORTED GLOBAL FUNCTIONS
*********************************************************************/

#endif /* nvm_map_H */