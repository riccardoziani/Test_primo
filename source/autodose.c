/**
@file              autodose_system.c
@brief             Autodose system
@author            Legacy
@version           $Id: Autodose_System.c 72 2018-01-23 14:56:14Z peter.tjeerdsma@philips.com $

@copyright         Copyright(C) 2018 Koninklijke Philips N.V., All Rights Reserved.
                   This source code and any compilation or derivative thereof is the
                   information of Koninklijke Philips N.V. is confidential in nature.
                   no circumstances is this software to be exposed to or placed
                   an Open Source License of any type without the expressed
                   permission of Koninklijke Philips N.V.
*/

// include files
#include "nvm_map.h"
#include "autodose.h"
//#include "autodose_nvm.h"


// private definitions *********************************************************
#define ADS_GRINDER_MIN_TIME_A1                 (3000)
#define ADS_GRINDER_MIN_TIME_A2                 (3500)
#define ADS_GRINDER_MIN_TIME_A3                 (4000)

#define ADS_K_P_10                              (10)    // K factor 1
#define ADS_K_P_15                              (15)    // K factor 1.5
#define ADS_K_P_30                              (30)    // K factor 3

#define ADS_MAX_ALLOWABLE_P_ERROR               (1000)

#define ADS_MAX_UNLOADED_CURRENT                (400)

#define ADS_EXTRA_TIME_COFFEE_DUCT_EMPTY_A1_A2  (3000)
#define ADS_EXTRA_TIME_COFFEE_DUCT_EMPTY_A3     (1500)

// private typedefs ************************************************************

// global data *****************************************************************
#ifdef AUTODOSE_DEBUG
uint16_t picco_corrente=0;
uint16_t picco_corrente_a_vuoto = 0;
#endif
// private data ****************************************************************
static const    uint16_t gr_min[] = {   ADS_GRINDER_MIN_TIME_A1,
                                        ADS_GRINDER_MIN_TIME_A2,
                                        ADS_GRINDER_MIN_TIME_A3
                                    };

static          uint16_t gr_max[NUMBER_OF_AROMA];



Type_nvm_sas* nvm_sas;

// prototype of private functions ***********************************************************
static void_t       Check_limit_aroma           (uint16_t* val, uint8_t Aroma);
static void_t       Setup_Mill_Time_Ratio       (uint16_t* p_val1, uint16_t* p_val2, uint8_t ref);
static void_t       Check_Mill_Time_Limits      (void_t);
static uint16_t     Get_BU_Unloaded_Current_AVG (void_t);
static uint8_t      Convert_Aroma_DoseType      (Type_Aroma aroma);

////******************************************************************************
//// Implementation
////******************************************************************************

// Public functions ***********************************************************

/**
 * \brief
 * This procedure initializes the variables used by the module grinder,
 * should be called only once at the beginning of the application,
 * before the main function.
 *
 * @author Emanuel Urgese
 * \date   02/03/2016
 *
 * \param  None
 * \return None
 */
void_t Autodose_init (void_t)
{
    uint32_t tmp;

    tmp = nvm.sas.max_grinder_time;

    gr_max[AROMA_3] = (uint16_t)tmp;
    tmp = (tmp*9)/10;
    gr_max[AROMA_2] = (uint16_t)tmp;
    tmp = (tmp*9)/10;
    gr_max[AROMA_1] = (uint16_t)tmp;

    Check_Mill_Time_Limits();
    return;
} // end of function Autodose_Init

/**
 * \brief
 * Change the grinding time for Aroma medium adding a quantity
 * passed as paramenter.
 * The modification is riflected also in other aroma:
 * - Time Aroma Light  = Time Aroma medium -20%
 * - Time Aroma Strong = Time Aroma medium +20%
 *
 * @author  Emanuel Urgese
 * \date    02/03/2016
 *
 * @param mill_time - time in ms
 *
 * @return None
 */
void_t Change_Reference_Time (int16_t mill_time)
{
    uint16_t local_step;

    if(mill_time >= (ADS_GRINDER_MIN_TIME_A1 / 3))
    {
        mill_time = (ADS_GRINDER_MIN_TIME_A1 / 3);
    }
    else if(mill_time <= -(ADS_GRINDER_MIN_TIME_A1 / 3))
    {
        mill_time = -(ADS_GRINDER_MIN_TIME_A1 / 3);
    }
    else
    {    // value in range no changes
    }

    mill_time = mill_time - (mill_time % 100);        //arrotondo alle centinaia
    nvm.sas.time_aroma[AROMA_2] += mill_time;

    local_step = nvm.sas.time_aroma[AROMA_2] / 5;   //20% of total value

    nvm.sas.time_aroma[AROMA_1] = nvm.sas.time_aroma[AROMA_2] - local_step;
    nvm.sas.time_aroma[AROMA_3] = nvm.sas.time_aroma[AROMA_2] + local_step;

    Check_Mill_Time_Limits();
    return;
} // end of function Gr_Change_Reference_Time


/**
 * \brief
 * The function return the grinding time associated with the aroma
 * passed in argument.
 * The value passed are from 0 to 4. (see enum)
 * Outside this value, the function return the value of time
 * corresponding the Aroma e_Very_Light
 *
 * \author Emanuel Urgese
 * \date 01/03/2016
 *
 * \param aroma - Aroma strenght
 *
 * \return time - value in milliseconds corresponding
 *                the value of Aroma selected
 */
uint16_t Get_Mill_Time (Type_Aroma aroma)
{
    uint16_t time;

    switch (aroma)
    {
        case e_Powder:
            time = 0;   /** No grinding*/
            break;

        case e_Light:
        case e_Medium:
            time = nvm.sas.time_aroma[AROMA_2];
            break;

        case e_Strong:
        case e_Very_Strong:
            time = nvm.sas.time_aroma[AROMA_3];
            break;

        case e_Very_Light:
        default:
            time = nvm.sas.time_aroma[AROMA_1];
            break;
    }
#ifdef OMNIA_FOR_PDC
        nvm.sas.nvm_sas_bit.coffe_duct_empty = FALSE;   // Done...
#else 
    if (nvm.sas.nvm_sas_bit.coffe_duct_empty == TRUE /*&& !Is_Main_State_Test_Mode()*/)
    {   // Very first coffee after Steam-Out, extra time must be added...
        nvm.sas.nvm_sas_bit.coffe_duct_empty = FALSE;   // Done...
        if (aroma == (Type_Aroma)AROMA_3)
        {
            time += ADS_EXTRA_TIME_COFFEE_DUCT_EMPTY_A3;
        }
        else
        {
            time += ADS_EXTRA_TIME_COFFEE_DUCT_EMPTY_A1_A2;
        }
    }
#endif 

    return time;
}

/**
 * \brief
 * This procedure initializes the variables inside NVM used
 * in Autodose system
 *
 * @author Emanuel Urgese
 * \date   02/03/2016
 * \param  None
 *
 * @return None
 */
void_t SAS_Format_Parameter (void_t)
{
    uint8_t jj;

    /** Reset  */
    for (jj = 0;jj < ADS_UNLOAD_ARRAY_DIM; jj++)
    {
        nvm.sas.BU_unload_current_array[jj]  = ADS_DEFAULT_CURRENT_ARRAY;
    }
    nvm.sas.BU_unload_current_array_index = 0;

    /** Time for autodose sistem */
    nvm.sas.time_aroma[AROMA_1] = ADS_GRINDER_TIME_AROMA_1;
    nvm.sas.time_aroma[AROMA_2] = ADS_GRINDER_TIME_AROMA_2;
    nvm.sas.time_aroma[AROMA_3] = ADS_GRINDER_TIME_AROMA_3;

    nvm.sas.num_skip_adjust_dose = 0;
    nvm.sas.nvm_sas_bit.coffe_duct_empty = TRUE;

    /** Current threshold for Autodose system */
    nvm.sas.Current_TH_array[AROMA_1]  = ADS_DEFAULT_SET_POINT_CURRENT_LIGHT;
    nvm.sas.Current_TH_array[AROMA_2]  = ADS_DEFAULT_SET_POINT_CURRENT_MEDIUM;
    nvm.sas.Current_TH_array[AROMA_3]  = ADS_DEFAULT_SET_POINT_CURRENT_STRONG;

    /** Max grinder time */
    nvm.sas.max_grinder_time    = ADS_DEFAULT_MAX_GRINDER_TIME;
    return;
} // end of function SAS_Format_Parameter

/**
 * \brief
 * Save a value of current inside the unloaded current vector
 * Must be called after the BU work during rinsing cycle
 *
 * @author Emanuel Urgese
 * \date   02/03/2016
 *
 * @param value - current in mA
 *
 * @return None
 */
void_t Save_BU_Unloaded_Current (uint16_t value)
{
    if (ADS_MAX_UNLOADED_CURRENT > value)
    {   // Solo se non supero il valore limite...
        nvm.sas.BU_unload_current_array[nvm.sas.BU_unload_current_array_index] = value;
        nvm.sas.BU_unload_current_array_index++;
        nvm.sas.BU_unload_current_array_index %= ADS_UNLOAD_ARRAY_DIM;
    }
    return;

} // end of function Bu_Store_Unloaded_Cycle_Current

/**
 * \brief
 * This function must be called at the end of the compression of
 * the powder inside the brewing unit. Some value are passed to
 * the function as an argument, and on the basis of these values
 * will correct the grinding time.
 *
 * @author Emanuel Urgese
 * \date   02/03/2016
 *
 * @param Aroma             - Aroma selected from menu
 * @param current           - Bu current peak - mA
 *
 * @return None
 */
void_t TuningDoseTime (Type_Aroma Aroma, uint16_t current)
{
    #define CURRENT_BEAN_UPPER_LIMIT    (1000)
    #define CURRENT_BEAN_LOWER_LIMIT    (-1000)

    int16_t     current_bean;
    uint16_t    unloaded_current;
    uint8_t     type;
    int16_t     P_error;
    uint8_t     bu_torque_alarm = FALSE;

    unloaded_current = Get_BU_Unloaded_Current_AVG();
    type = Convert_Aroma_DoseType(Aroma);

    current_bean = (int16_t)current - (int16_t)unloaded_current;
#ifdef AUTODOSE_DEBUG
    picco_corrente = current;
    picco_corrente_a_vuoto = unloaded_current;
#endif
    /** Check limit to avoid overflow/underflow */
    if(current_bean >= CURRENT_BEAN_UPPER_LIMIT)
    {
        current_bean = CURRENT_BEAN_UPPER_LIMIT;
        bu_torque_alarm = TRUE;
    }
    else if(current_bean <= CURRENT_BEAN_LOWER_LIMIT)
    {
        current_bean = CURRENT_BEAN_LOWER_LIMIT;
    }
    else
    {   /** Nothing, current is inside limits*/
    }

    if(FALSE == Is_Bean_Presence())
    {
        nvm.sas.num_skip_adjust_dose = 2;
    }

    if (nvm.sas.num_skip_adjust_dose == 0)
    {
        switch (type)
        {
            case AROMA_1:
                P_error = ((nvm.sas.Current_TH_array[AROMA_1] - current_bean)*ADS_K_P_30)/10;
                if (P_error < -ADS_MAX_ALLOWABLE_P_ERROR)
                {   /** Max correction is 1second */
                    P_error = -ADS_MAX_ALLOWABLE_P_ERROR;
                }

                nvm.sas.time_aroma[AROMA_1] += P_error;
                Check_limit_aroma(&nvm.sas.time_aroma[AROMA_1], AROMA_1);

                nvm.sas.time_aroma[AROMA_2] += P_error/3;
                nvm.sas.time_aroma[AROMA_3] += P_error/3;

                Setup_Mill_Time_Ratio (&nvm.sas.time_aroma[AROMA_1], &nvm.sas.time_aroma[AROMA_2], 1);
                Setup_Mill_Time_Ratio (&nvm.sas.time_aroma[AROMA_2], &nvm.sas.time_aroma[AROMA_3], 1);
                break;

            case AROMA_2:
                P_error = ((nvm.sas.Current_TH_array[AROMA_2] - current_bean)*ADS_K_P_15)/10;
                if (P_error < -ADS_MAX_ALLOWABLE_P_ERROR)
                {
                    P_error = -ADS_MAX_ALLOWABLE_P_ERROR;
                }
                nvm.sas.time_aroma[AROMA_2] += P_error;

                Check_limit_aroma(&nvm.sas.time_aroma[AROMA_2], AROMA_2);

                nvm.sas.time_aroma[AROMA_1] += P_error/3;
                nvm.sas.time_aroma[AROMA_3] += P_error/3;

                Setup_Mill_Time_Ratio (&nvm.sas.time_aroma[AROMA_2], &nvm.sas.time_aroma[AROMA_3], 1);
                Setup_Mill_Time_Ratio (&nvm.sas.time_aroma[AROMA_2], &nvm.sas.time_aroma[AROMA_1], 2);
                break;

            case AROMA_3:
                P_error = ((nvm.sas.Current_TH_array[AROMA_3] - current_bean)*ADS_K_P_10)/10;
                if (P_error < -ADS_MAX_ALLOWABLE_P_ERROR || bu_torque_alarm)
                {   /** Max 1 seconds, also if Torque alarm*/
                    P_error = -ADS_MAX_ALLOWABLE_P_ERROR;
                }

                /** Modify grinding time value */
                nvm.sas.time_aroma[AROMA_3] += P_error;

                /** Check Limits */

                Check_limit_aroma(&nvm.sas.time_aroma[AROMA_3], AROMA_3);

                /** Correct other aroma's time */
                nvm.sas.time_aroma[AROMA_1] += P_error/3;
                nvm.sas.time_aroma[AROMA_2] += P_error/3;

                /** Check limits with reference of type selected */
                Setup_Mill_Time_Ratio (&nvm.sas.time_aroma[AROMA_3], &nvm.sas.time_aroma[AROMA_2], 2);
                Setup_Mill_Time_Ratio (&nvm.sas.time_aroma[AROMA_2], &nvm.sas.time_aroma[AROMA_1], 2);
                break;

            case POWDER:
                /** Nothing to check */
                break;

            default:
                break;
        }
    }

    if (nvm.sas.num_skip_adjust_dose)
    {
        nvm.sas.num_skip_adjust_dose--;
    }

    Check_Mill_Time_Limits();
    return;
} // end of function Gr_TuningDoseTime

// private functions ***********************************************************

/**
 *  \brief
 *  Th function check limit time after a correction done
 *  by tuning dose function and correct the value if out of
 *  range
 *
 * \author Emanuel Urgese
 * \date   02/03/2016
 *
 * \param  *val  - Pointing to variable to check
 * \param  Aroma - Aroma to be checked in limit
 * \return None
 *
 */
static void_t Check_limit_aroma(uint16_t* val, uint8_t Aroma)
{
    if (*val < gr_min[Aroma])
    {
        *val = gr_min[Aroma];
    }
    else if (*val > gr_max[Aroma])
    {
        *val = gr_max[Aroma];
    }
    else
    {   /** Value in range, do nothings... */
    }
    return;
}

/**
 * \brief
 * This function check the limit time for all the
 * type of aroma
 *
 * @author Emanuel Urgese
 * \date   02/03/2016
 *
 * \param  None
 *
 * @return None
 */
static void_t Check_Mill_Time_Limits (void_t)
{
    uint8_t jj;
    for (jj = 0;jj < NUMBER_OF_AROMA; jj++)
    {
        if (nvm.sas.time_aroma[jj] < gr_min[jj])
        {
            nvm.sas.time_aroma[jj] = gr_min[jj];
        }
        else if (nvm.sas.time_aroma[jj] > gr_max[jj])
        {
            nvm.sas.time_aroma[jj] = gr_max[jj];
        }
        else
        {   // Value in range, do nothings...

        }
    }
    return;
} // end of function Check_Mill_Time_Limits

/**
 * \brief
 *
 * @author Emanuel Urgese
 * \date   01/03/2016
 *
 * \param p_val1
 * \param p_val2
 * \param ref
 */
static void_t Setup_Mill_Time_Ratio (uint16_t* p_val1, uint16_t* p_val2, uint8_t ref)
{
    uint16_t tmp;
    uint32_t val1;

    val1 = *p_val1;
    if (ref == 1)
    {
        val1 = val1 * 110 / 100;    // Il settaggio superiore non deve essere inferiore al 110%
        if(val1 > UINT16_MAX)
    {
            tmp = UINT16_MAX;
    }
    else
    {
            tmp = (uint16_t)val1;
    }

        if (*p_val2 < tmp)
        {
            *p_val2 = tmp;
        }

    val1 = val1 * 130 / 100;    // Il settaggio superiore non deve essere inferiore al 130%
    if(val1 > UINT16_MAX)
    {
            tmp = UINT16_MAX;
    }
    else
    {
            tmp = (uint16_t)val1;
    }

        if (*p_val2 > tmp)
        {
            *p_val2 = tmp;
        }
    }
    else if (ref == 2)
    {
    val1 = val1 * 90 / 100;    // Il settaggio inferiore non deve superare il 90% del superiore
    tmp = (uint16_t)val1;    //no overflow over 65535
        if (*p_val2 > tmp)
        {
            *p_val2 = tmp;
        }

    val1 = val1 * 70 / 100;    // Il settaggio inferiore non deve essere inferiore all'70%
    tmp = (uint16_t)val1;    //no overflow over 65535
        if (*p_val2 < tmp)
        {
            *p_val2 = tmp;
        }
    }
    else
    {
        //do nothing
    }

    return;
} // end of function Setup_Mill_Time_Ratio

/**
 * \brief
 * The function return the average of the BU unloaded
 * current samples acquired in the machine.
 * The dimension of array is set to 4.
 *
 * \author Emanuel Urgese
 * \date   02/03/2016
 *
 * \param None
 * \return AVG_Bu_unload_current - value in mA
 */
static uint16_t Get_BU_Unloaded_Current_AVG (void_t)
{
    uint8_t i;
    uint16_t sum;

    sum = 0;
    for(i=0;i<ADS_UNLOAD_ARRAY_DIM;i++)
    {
        sum += nvm.sas.BU_unload_current_array[i];
    }
    sum /=ADS_UNLOAD_ARRAY_DIM;

    return(sum);
} // end of function Bu_Get_Unloaded_Cycle_Current

/**
 * \brief
 * Convert aroma selected into Dose_type for
 * SAS algorithm calculation
 *
 * \param Aroma
 * \return Dose_type
 */
uint8_t Convert_Aroma_DoseType(Type_Aroma aroma)
{
    uint8_t type;

    switch (aroma)
    {
        case e_Powder:
            type = POWDER;
            break;

        case e_Light:
        case e_Medium:
            type = AROMA_2;
            break;

        case e_Strong:
        case e_Very_Strong:
            type = AROMA_3;
            break;

        case e_Very_Light:
        default:
            type = AROMA_1;
            break;
    }
    return type;
}

