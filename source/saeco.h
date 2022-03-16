/*******************************************************************************
Filename: SAECO.h
Author: Riccardo Ziani
Created: 11/04/2011

Description:
File header for Npr_Top.c
< place here any information concerning the module purpose >

Usage:
< place here info for module users >
******************************************************************************/

#ifndef SAECO_H
#define SAECO_H
//*****************************************************************************

// Include files **************************************************************

#include "fsl_common.h"

// Public definitions *********************************************************


#define MACHINE_MODEL       (3030)
#define STATUSFLAG          {0,0,0,0,0,0,0,0}      
#define BOOT_VERSION        (((*(const char_t*)0x000000B1 - '0')*10) + *(const char_t*)0x000000B2 - '0')


#define MAX_DREG_NUMBER     (15)


// Public typedefs ************************************************************

typedef union//   __attribute__((__packed__))
{
    struct
    {
        uint8_t Brwing_Unit_Warmed  :1; // 0
        uint8_t alarm_refill        :1; // 1
        uint8_t pod_already_wet     :1; // 2
        uint8_t descale_alert       :1; // 3
        uint8_t debug_mode          :1; // 4
        uint8_t steamout_warning    :1; // 5
        uint8_t unused_6            :1; // 6
        uint8_t unused_7            :1; // 7
    }bit;
    uint8_t reg;
}nvm_t_STATUS_FLAGS;

typedef enum
{
    ms_ResetMode,
    ms_StartUpMode,
    ms_FactoryTestMode,
    ms_NormalMode,
    ms_ProductMode,
    ms_UserProgramMode,
    ms_SafetyMode,
    ms_StandByMode,
    ms_PowerOffMode,
    ms_USCP_TestMode,
    ms_USCP_CommandMode,
}   TStateMode;

// Public data ****************************************************************
extern const	uint8_t		checkword[];	// Marking Word for the BOOTLOADER	with virtual E2prom  and AC32

// Public functions ***********************************************************

//*****************************************************************************
#endif /* ifndef SAECO_H */

