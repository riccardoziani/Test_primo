/*******************************************************************************
Filename: timer.c
Author: Riccardo Ziani
Created: 23/01/2018
Description: 

*******************************************************************************/

// include files
#include "hal.h"
#include "nvm.h"
#include "timer.h"
// private definitions *********************************************************


// private typedefs ************************************************************

typedef union
{
    struct
    {
        uint8_t    _1msec      :1;
        uint8_t    _10msec     :1;
        uint8_t    _100msec    :1;
        uint8_t    _1000msec   :1;
        uint8_t                :4;
    }bit;
    uint8_t reg; 

}   Type_TimerBase_Status;  

// global data *****************************************************************
uint8_t     save_nvm_timer;
uint16_t    main_freq;

// private data ****************************************************************

static Type_TimerBase_Status	TimerBase_Status = {0x00};

// private functions ***********************************************************

//******************************************************************************
// Implementation
//******************************************************************************

//------------------------------------------------------------------------------
// Function: TimeBase_Refresh
// Parameters: 
// Returns: 
// Description: Called every interrupt of 1ms
// 
//------------------------------------------------------------------------------
void_t   TimeBase_Refresh (void_t)
{
	static uint16_t TimerBase_Counter;
    static uint8_t  blink_1_sec;
	
	if (TimerBase_Counter < 1000)
	{
		TimerBase_Counter += 1;
	}
	else
	{
		TimerBase_Counter = 1;
	}
	TimerBase_Status.bit._1msec = true;

    if ((TimerBase_Counter%10) == 0)
    {
		TimerBase_Status.bit._10msec = true;
    }

    if ((TimerBase_Counter%100) == 0)
    {
		TimerBase_Status.bit._100msec = true;
    }

    if ((TimerBase_Counter%1000) == 0)
    {
		TimerBase_Status.bit._1000msec = true;

        main_freq = main_freq_tmp;
		main_freq_tmp = 0;
        blink_1_sec++;
		if ((blink_1_sec & 0x01) == 1)
		{   /** After 3 seconds enable nvm to write*/
            nvm_enable();
            if (nvm_save())
            {
            }
		}
    }
	//--------------------------------------------------//
	return;
}

//------------------------------------------------------------------------------
// Function: TimeBase_Handler
// Parameters: 
// Returns: 
// Description: Task scheduler
//              Update timer for modules
// 
//------------------------------------------------------------------------------
void_t   TimeBase_Handler (void_t)
{
    if (TimerBase_Status.bit._1msec == true)
    {
    	TimerBase_Status.bit._1msec = false;
    }
    if (TimerBase_Status.bit._10msec == true)
    {
    	TimerBase_Status.bit._10msec = false;

        //Input_Handler_Timer_10ms();
        //nvm_check_and_save();

    }
	
    if (TimerBase_Status.bit._100msec == true)
    {
    	TimerBase_Status.bit._100msec = false;
    }

    if (TimerBase_Status.bit._1000msec == true)
    {
        TimerBase_Status.bit._1000msec = false;
    }
	
	return;
}