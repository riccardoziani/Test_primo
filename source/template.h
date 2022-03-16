/**
                                PLEASE READ ME QUICK-REFERENCE
@brief this is a brief one line documentation/descriptions of this file.

write  any  text/documentation  here  as this one and it will be a detailed  description   ,
for writing a brief one line description, start the line with the @brief command like above.

ALWAYS USE @brief, otherwise the index entry won't have a brief explanation.

[FONT] format commands :
@a italic
@b bold

@file INDEX.dox
@todo please remove the Doxygen quick reference from your source code !
*/

/**
@file              template.h
@brief             A quick documentation
@details           This class is used to demonstrate a number of section commands.
@author	           first author ...
@version           $Id$
@warning           Improper use can crash your application

@copyright         Copyright(C) 2017 Koninklijke Philips N.V., All Rights Reserved.
                   This source code and any compilation or derivative thereof is the
                   information of Koninklijke Philips N.V. is confidential in nature.
                   no circumstances is this software to be exposed to or placed
                   an Open Source License of any type without the expressed
                   permission of Koninklijke Philips N.V.
*/



#ifndef HEADERNAME_CHANGEME_GUARDS_H
#define HEADERNAME_CHANGEME_GUARDS_H

/* ******************************************************************
                            INCLUDES
*********************************************************************/

/* include phlips global file and all other headers */  
#include "philips_types.h"
#include "template.h"
 
/* ******************************************************************
                            MACRO DEFINITIONS
 *********************************************************************/

#define EXAMPLE 10 /**< we have this EXAMPLE macro variable  */

/* ******************************************************************
                          EXPORTED TYPES
*********************************************************************/

/** An enum type. 
    The documentation block cannot be put after the enum! 
*/

typedef enum
{
    ST_IDLE,         /**< the motor is  idle */
    ST_RUNNING,      /**< the motor is spinning */
    ST_STOPPED,      /**< the motor has stopped */
} motor_state_t;
 
/**
    @brief this struct represents etc.....
    
    Details here ....
*/
typedef struct boiler_descaling_status_tag
{
    uint32_t descaling_equiv_pulses_counter_global;          /**<  equivalent number of water pulses(considering water hardness and filter) */
    uint16_t execution_number_global;                        /**<  equivalent number of water pulses(considering water hardness and filter) */
    uint32_t total_current_water_pulses_global;              /**<  the pulse of current water .... */
}  __attribute__((__packed__)) boiler_descaling_status_t;


/* ******************************************************************
                    EXPORTED GLOBAL VARIABLES
*********************************************************************/

/* please limit the use of global variables accoarding to rule DI.2 */
extern uint32_t global_var; /**< Detailed description of a global variable */

/** A function variable Details. or a pointer to function */
extern int32_t (*handler)(int32_t a, int32_t b);


/* ******************************************************************
                    EXPORTED GLOBAL FUNCTIONS
*********************************************************************/


/** @brief A function description .
    detalied description and code example as 

    @code
        char_t a;
        char_t b;
        test_me_too(a, b);
    @endcode

    @param c1 the first argument.
    @param c2 the second argument.
    @return The test results
    @retval 1 SUCCESS
    @retval 0 Failure
*/

bool_t test_me_too (char_t c1, char_t c2);

#endif /* HEADERNAME_CHANGEME_GUARDS_H */