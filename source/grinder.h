/**
@file              grinder_driver.h
@brief             Grinder driver interface
@author            Legacy
@version           $Id: GRINDER_Driver.h 71 2018-01-23 12:38:43Z peter.tjeerdsma@philips.com $

@copyright         Copyright(C) 2018 Koninklijke Philips N.V., All Rights Reserved.
                   This source code and any compilation or derivative thereof is the
                   information of Koninklijke Philips N.V. is confidential in nature.
                   no circumstances is this software to be exposed to or placed
                   an Open Source License of any type without the expressed
                   permission of Koninklijke Philips N.V.
*/

#ifndef GRINDER_DRIVER_H
#define GRINDER_DRIVER_H
//*****************************************************************************

// Include files **************************************************************
#include "philips_types.h"

// Public definitions *********************************************************

#define F_60HZ                              (60)
#define F_50HZ                              (50)

#define GR_RAMP_UP_WINDOW                   (500)   /** Time for ramp-up - 500 ms fixed*/
#define GR_SENSING_WINDOW                   (2500)  /** Time for sensing at the end of grinding */

#define GR_DEFAULT_SUM_SENSOR_PRESENCE      (56000)  // Somma dei valori del sensore delel ultime 16 macinatura con caffè
#define GR_DEFAULT_SUM_SENSOR_ABSENCE       (40000)  // Somma dei valori del sensore delel ultime 16 macinatura in assenza di caffè

#define DEFAULT_NVM_GRINDER_STRUCTURE {                                 \
    0,                                  /*  Free0;                  */  \
    0,                                  /*  Free1;                  */  \
    0,                                  /*  Free2;                  */  \
    0,                                  /*  Free3;                  */  \
    5,                                  /*  mills_counter;;         */  \
    0,                                  /*  estimated_motor_use;    */  \
    GR_DEFAULT_SUM_SENSOR_ABSENCE,      /*  sensor_sum_absence;     */  \
    GR_DEFAULT_SUM_SENSOR_PRESENCE,     /*  sensor_sum_presence;    */  \
    0,                                  /*  Free4;                  */  \
    0,                                  /*  bit;                    */  \
}   // 24 Bytes


// Public typedefs ************************************************************

typedef struct//  __attribute__((__packed__)) nvm_grinder_struct_tag
{
    uint32_t    Free0;
    uint32_t    Free1;
    uint32_t    Free2;
    uint16_t    Free3;
    uint16_t    mills_counter;
    uint16_t    estimated_motor_use;
    uint16_t    sensor_sum_absence;
    uint16_t    sensor_sum_presence;
    uint8_t     Free4;
    uint8_t     Free5;
}Type_nvm_gr;

extern Type_nvm_gr Gr;


// Public data ****************************************************************

// Public functions ***********************************************************
void_t      Gr_Control                          (void_t);

bool_t      Gr_Start_Milling                    (uint16_t);
void_t      Gr_Stop                             (void_t);

void_t      Grinder_Handler                     (void_t);
void_t      Grinder_Handler_Timer_100ms         (void_t);
void_t      Grinder_PW_OFF_Timer_100ms          (void_t);
uint8_t     Is_BeanDoor_Timeout_Alarm           (void_t);
uint8_t     Is_BeanDoor_Alarm                   (void_t);
uint8_t     Is_Grinder_Handler_Idle             (void_t);
uint8_t     Is_Grinder_Handler_Fail             (void_t);
bool_t      Is_Grinder_Ramp_up                  (void_t);
bool_t      Is_Grinder_Running                  (void_t);
bool_t      Is_Grinder_Sensing                  (void_t);
bool_t      Is_Grinder_FULL                     (void_t);
bool_t      Is_Grinder_ON                       (void_t);
bool_t      Is_Grinder_Blocked                  (void_t);
bool_t      Is_Grinder_Open                     (void_t);
bool_t      Is_Grinder_Fail                     (void_t);

// To ISR
void_t      Set_GR_Pulse_time                   (uint16_t value);
void_t      Set_GR_Pulse_Counter_freerun        (uint16_t value);
void_t      Set_GR_CheckPulses_timeout          (void_t);
uint16_t    Get_GR_Current_mill_time            (void_t);
uint16_t    Get_GR_phase                        (void_t);
bool_t      Is_Bean_Presence                    (void_t);
void_t      Reset_Bean_Presence                 (void_t);
#ifdef AUTODOSE_DEBUG
uint16_t    Get_BeanLackLastValue();
uint16_t    Get_BeanLackTH();
#endif
void_t      newGrinder_Format_Parameter         (void_t);
#endif /* GRINDER_DRIVER_H */
