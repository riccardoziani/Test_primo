/**
@brief          this file contains all global defined datatypes to be used in the project
@copyright      Copyright(C) 2016 Koninklijke Philips N.V., All Rights Reserved.
                This source code and any compilation or derivative thereof is the
                information of Koninklijke Philips N.V.
                is confidential in nature.
                no circumstances is this software to be exposed to or placed
                an Open Source License of any type without the expressed
                permission of Koninklijke Philips N.V.
 */

#ifndef PHILIPS_TYPES_H
#define PHILIPS_TYPES_H

#ifdef    FALSE
#undef    FALSE
#endif
#define   FALSE     (0)     /**< FALSE redefined to ensure portability */
    
#ifdef    TRUE
#undef    TRUE
#endif
#define   TRUE      (1)     /**< TRUE redefined to ensure portability */
    
#ifdef    NULL
#undef    NULL
#endif
#define   NULL      (0)     /**< NULL redefined to ensure portability */


#if (__ARMCC_VERSION)
    typedef void                void_t;      /** void datatype */
    typedef bool                bool_t;      /** boolean datatype */
    typedef char                char_t;      /** plain char type (only used for ascii chars) */
    typedef signed char         int8_t;      /** 8 bit signed integer signed char */
    typedef unsigned char       uint8_t;     /** 8 bit unsigned integer */
    typedef unsigned short      uchar_t;     /** a unicode char */
    typedef signed short        int16_t;     /** 16 bit signed integer */
    typedef unsigned short      uint16_t;    /** 16 bit unsigned integer*/
    typedef signed long         int32_t;     /** 32 bit signed integer */
    typedef unsigned long       uint32_t;    /** 32 bit unsigned integer */
    typedef signed long long    int64_t;     /** 64 bit signed integer long int */
    typedef unsigned long long  uint64_t;    /** 64 bit unsigned integer unsigned long int */
    typedef float               float32_t;   /** 32 bit single precision float */
    typedef double              float64_t;   /** 64 bit single precision float */
    //typedef long double       float128_t;  /** 128 bit single precision float */



#elif defined (__GNUC__)

    #define _INT8_T_DECLARED
    #define _INT32_T_DECLARED
    #define _UINT32_T_DECLARED
    #define _INT64_T_DECLARED
    #define _UINT64_T_DECLARED
    #define _FLOAT32_T_DECLARED
    #define _FLOAT64_T_DECLARED

    typedef void                void_t;      /** void datatype */
    typedef bool                bool_t;      /** boolean datatype */
    typedef char                char_t;      /** plain char type (only used for ascii chars) */
    typedef signed char         int8_t;      /** 8 bit signed integer signed char */
    typedef unsigned char       uint8_t;     /** 8 bit unsigned integer */
    typedef unsigned short      uchar_t;     /** a unicode char */
    typedef signed short        int16_t;     /** 16 bit signed integer */
    typedef unsigned short      uint16_t;    /** 16 bit unsigned integer*/
    typedef signed long         int32_t;     /** 32 bit signed integer */
    typedef unsigned long       uint32_t;    /** 32 bit unsigned integer */
    typedef signed long long    int64_t;     /** 64 bit signed integer long int */
    typedef unsigned long long  uint64_t;    /** 64 bit unsigned integer unsigned long int */
    typedef float               float32_t;   /** 32 bit single precision float */
    typedef double              float64_t;   /** 64 bit single precision float */
    //typedef long double       float128_t;  /** 128 bit single precision float */

#elif defined (__ICCARM__)
    typedef void                void_t;      /** void datatype */
    typedef _Bool               bool_t;      /** boolean datatype */
    typedef char                char_t;      /** plain char type (only used for ascii chars) */
    typedef signed char         int8_t;      /** 8 bit signed integer signed char */
    typedef unsigned char       uint8_t;     /** 8 bit unsigned integer */
    typedef unsigned short      uchar_t;     /** a unicode char */
    typedef signed short        int16_t;     /** 16 bit signed integer */
    typedef unsigned short      uint16_t;    /** 16 bit unsigned integer*/
    typedef signed int          int32_t;     /** 32 bit signed integer */
    typedef unsigned int        uint32_t;    /** 32 bit unsigned integer */
    typedef signed long long    int64_t;     /** 64 bit signed integer long int */
    typedef unsigned long long  uint64_t;    /** 64 bit unsigned integer unsigned long int */
    typedef float               float32_t;   /** 32 bit single precision float */
    typedef double              float64_t;   /** 64 bit single precision float */
    //typedef long double         float64_t;   /** 64 bit single precision float */
#endif


#endif /*PHILIPS_TYPES_H*/

