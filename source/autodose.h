/*******************************************************************************
Filename: autodose.h
Author: Riccardo Ziani
Created: 22/01/2018

Description:
File header for autodose.c
< place here any information concerning the module purpose >

Usage:
< place here info for module users >
******************************************************************************/

#ifndef autodose_H
#define autodose_H
//*****************************************************************************
// Include files **************************************************************
#include "philips_types.h"

// Public definitions *********************************************************                     
enum
{                               
    AROMA_1,
    AROMA_2,
    AROMA_3,
    NUMBER_OF_AROMA,
    POWDER,                                   
};

#define ADS_UNLOAD_ARRAY_DIM                    (4) 
#define ADS_GRINDER_TIME_AROMA_1                (5500)                      
#define ADS_GRINDER_TIME_AROMA_2                (7000)                      
#define ADS_GRINDER_TIME_AROMA_3                (7500)                        
#define ADS_DEFAULT_CURRENT_ARRAY               (150)   // mAmpere 
#define ADS_DEFAULT_SET_POINT_CURRENT_LIGHT     (55)    // mAmpere A1
#define ADS_DEFAULT_SET_POINT_CURRENT_MEDIUM    (100)   // mAmpere A2
#define ADS_DEFAULT_SET_POINT_CURRENT_STRONG    (200)   // mAmpere A3
#define ADS_DEFAULT_MAX_GRINDER_TIME            (10000) // Max time grinding
#define ADS_COFFEE_DUCT_EMPTY_FIRST_INIT        (true)
#define ADS_GR_BIT						        {ADS_COFFEE_DUCT_EMPTY_FIRST_INIT,0,0,0, 0,0,0,0}



#define	DEFAULT_NVM_SAS_STRUCTURE       {   \
    0,                                      \
    0,                                      \
    0,                                      \
    0,                                      \
    0,                                      \
    ADS_GR_BIT,                             \
    ADS_GRINDER_TIME_AROMA_1,               \
    ADS_GRINDER_TIME_AROMA_2,               \
    ADS_GRINDER_TIME_AROMA_3,               \
    ADS_DEFAULT_CURRENT_ARRAY,              \
    ADS_DEFAULT_CURRENT_ARRAY,              \
    ADS_DEFAULT_CURRENT_ARRAY,              \
    ADS_DEFAULT_CURRENT_ARRAY,              \
    ADS_DEFAULT_SET_POINT_CURRENT_LIGHT,    \
    ADS_DEFAULT_SET_POINT_CURRENT_MEDIUM,   \
    ADS_DEFAULT_SET_POINT_CURRENT_STRONG,   \
    ADS_DEFAULT_MAX_GRINDER_TIME,           \
    0,                                      \
    0,                                      \
}   // 40 Byte



// Public typedefs ************************************************************
typedef enum
{
    e_Powder,       // 0
    e_Very_Light,   // 1
    e_Light,        // 2
    e_Medium,       // 3
    e_Strong,       // 4
    e_Very_Strong,  // 5
}Type_Aroma;

typedef struct
{                               
    uint8_t coffe_duct_empty    :1; //  0  
    uint8_t unused_1            :1; //  1
    uint8_t unused_2            :1; //  2
    uint8_t unused_3            :1; //  3
    uint8_t unused_4            :1; //  4
    uint8_t unused_5            :1; //  5
    uint8_t unused_6            :1; //  6
    uint8_t unused_7            :1; //  7
}T_nvm_sas_bit;                                    



typedef struct//   __attribute__((__packed__))
{                               
    uint32_t        Free0;                                          /** 32 bit free */                      // 4 Byte
    uint32_t        Free1;                                          /** 32 bit free */                      // 4 Byte
    uint32_t        Free2;                                          /** 32 bit free */                      // 4 Byte
    uint16_t        Free3;                                          /** 16 bit free */                      // 2 Byte
    uint8_t         Free4;                                          /** 8 bit free */                       // 1 Byte
    T_nvm_sas_bit   nvm_sas_bit;                                    /** 8 bit  T_nvm_sas_bit*/              // 1 Byte
    uint16_t        time_aroma[NUMBER_OF_AROMA];                    /** Grinder time vector */              // 6 Byte       
    uint16_t        BU_unload_current_array[ADS_UNLOAD_ARRAY_DIM];  /** BU Unloaded current vector */       // 8 Byte
    int16_t         Current_TH_array[NUMBER_OF_AROMA];              /** Current threshold vector */         // 6 Byte     
    uint16_t        max_grinder_time;                               /** Max time for Aroma 3 */             // 2 Byte   
    uint8_t         num_skip_adjust_dose;                           /** Number of dose tunings to skip */   // 1 Byte
    uint8_t         BU_unload_current_array_index;                  /** BU unloaded vector index */         // 1 Byte
}Type_nvm_sas;                                                                                              // 40 Byte


// Public data ****************************************************************
#ifdef AUTODOSE_DEBUG
extern uint16_t picco_corrente;
extern uint16_t picco_corrente_a_vuoto;
#endif
// Public functions ***********************************************************
extern  void_t      Autodose_init               (void_t);
extern  uint16_t    Get_Mill_Time               (Type_Aroma);
extern  void_t      TuningDoseTime              (Type_Aroma, uint16_t);
extern  void_t      Save_BU_Unloaded_Current    (uint16_t value);
extern  void_t      SAS_Format_Parameter        (void_t);
extern  void_t      Change_Reference_Time       (int16_t);

#endif /* ifndef autodose_H */
