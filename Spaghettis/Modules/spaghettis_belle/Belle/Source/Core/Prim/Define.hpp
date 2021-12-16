
/* 
    Copyright (c) 2007-2013 Andi Brae.
    Copyright (c) 2013 Jojo and others.
    
*/

/* < http://opensource.org/licenses/BSD-2-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// ====================================

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if ( __cplusplus < 201103L )
    #error "C++11 required!"
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://stackoverflow.com/a/27054190 > */

#if defined ( __BIG_ENDIAN__ )
    #define PRIM_BIG_ENDIAN         1
#elif defined ( __LITTLE_ENDIAN__ )
    #define PRIM_LITTLE_ENDIAN      1
#else

#include <endian.h>

#if defined ( __BYTE_ORDER ) && ( __BYTE_ORDER == __BIG_ENDIAN ) || \
    defined ( __BIG_ENDIAN__ ) || \
    defined ( __ARMEB__ ) || \
    defined ( __THUMBEB__ ) || \
    defined ( __AARCH64EB__ ) || \
    defined ( _MIBSEB ) || defined ( __MIBSEB ) || defined ( __MIBSEB__ )

    #define PRIM_BIG_ENDIAN         1
    
#elif defined ( __BYTE_ORDER ) && ( __BYTE_ORDER == __LITTLE_ENDIAN ) || \
    defined ( __LITTLE_ENDIAN__ ) || \
    defined ( __ARMEL__ ) || \
    defined ( __THUMBEL__ ) || \
    defined ( __AARCH64EL__ ) || \
    defined ( _MIPSEL ) || defined ( __MIPSEL ) || defined ( __MIPSEL__ )

    #define PRIM_LITTLE_ENDIAN      1
    
#endif
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#ifdef PRIM_LITTLE_ENDIAN
#else
#ifdef PRIM_BIG_ENDIAN
#else
    #error "Unknown endianness!"
#endif
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
