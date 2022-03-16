/**
                                 PLEASE READ ME QUICK-REFERENCE
@brief this is a brief one line documentation/descriptions of this file.

write  any  text/documentation  here  as this one and it will be a detailed  description,
for writing a brief one line description, start the line with the @brief command like above.

ALWAYS USE @brief, otherwise the index entry won't have a brief explanation.

[FONT] format commands :
@a italic
@b bold

@file INDEX.dox
@todo please remove the Doxygen quick reference from your source code !
*/

/**
@file              template.c
@brief             A quick documentation
@details           This class is used to demonstrate a number of section commands.
@author            first author ...
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
#include "source.h"

/* include std libs only when required.  */
#include <stdio.h>     

/* ******************************************************************
                    NON EXPORTED LOCAL TYPES
*********************************************************************/

/** An enum type. 
    The documentation block cannot be put after the enum! 
*/
typedef enum
{
    VAL1,     /**< enum value 1 */
    VAL2      /**< enum value 2 */
} enum_local_type_name_t;

/**
    @brief this struct represents etc.....
    
    Details here ....
*/
typedef struct boiler_descaling_local_status_tag
{
    uint32_t descaling_equiv_pulses_counter;          /**<  equivalent number of water pulses(considering water hardness and filter) */
    uint16_t execution_number;                        /**<  equivalent number of water pulses(considering water hardness and filter) */
    uint32_t total_current_water_pulses;              /**<  current water pulse ...*/

}  __attribute__((__packed__)) boiler_descaling_local_status_t;

/* ******************************************************************
                     GLOBAL VARIABLES                                       
*********************************************************************/

int32_t global_var; /**< Detailed description of a global variable */

/* ******************************************************************
                     LOCAL VARIABLES                                       
*********************************************************************/

static int32_t local_var; /**< Detailed description of a local variable */

/** a static pointer to function.
    you can also any commands like \\sa or \\param or \\return just like in the previous functions !*/

static int32_t (*static_handler)(int32_t a,int32_t b);
 

/* ******************************************************************
                LOCAL FUNCTIONS DECLARATIONS
*********************************************************************/
/** @brief this function would add one to the number and returns the addition result

    @code
        uint32_t x = 5;
        related_function(x);
    @endcode

    @param c1 the first argument.
    @return the input plus 1
*/

static uint32_t related_function(uint32_t para1);

/** @brief A function description .
    detalied description and code example as 
    @fn static_function
    
    @code
        uchar_t a;
        uchar_t b;
        static_function(a, b);
    @endcode

    @see related_function()
    @param c1 the first argument.
    @param c2 the second argument.
    @return The test results
*/

static uint32_t static_function(uchar_t uc1, uchar_t uc2);


/* ******************************************************************
                    ALL EXPORTED FUNCTIONS IMPLEMENTATIONS
*********************************************************************/

bool_t test_me_too(char_t c1, char_t c2)
{
    /* do something ? */
}

/* ******************************************************************
                    ALL LOCAL FUNCTIONS IMPLEMENTATIONS
*********************************************************************/

uint32_t related_function(uint32_t para1);
{
    return para1 + 1;
}

uint32_t static_function(uchar_t uc1, uchar_t uc2)
{
    /* some code here */
    uint32_t x = (uint32_t) uc1 + uc2;
    return related_function(x);
}

