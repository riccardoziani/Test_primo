/*******************************************************************************
Filename: timer.h
Author: Riccardo Ziani
Created: 23/01/2018

Description:
File header for timer.c
< place here any information concerning the module purpose >

Usage:
< place here info for module users >
******************************************************************************/

#ifndef timer_H
#define timer_H

// Include files **************************************************************
#include "philips_types.h"
// Public definitions *********************************************************

// Public data ****************************************************************


// Public functions ***********************************************************
void_t    TimeBase_Refresh     (void_t);
void_t    TimeBase_Handler     (void_t);
//*****************************************************************************
#endif /* ifndef timer_H */
