
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://graphics.stanford.edu/~seander/bithacks.html > */
/* < http://aggregate.org/MAGIC/ > */
/* < http://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers > */
/* < http://hackage.haskell.org/package/bits-0.3.3/src/cbits/debruijn.c > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_32BIT

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

static const int randMT_DeBruijn32[] =      /* Static. */
    {
        0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
        8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
    };

static int randMT_uInt32LogBase2Index (uint32_t v)
{
    if (!v) { return 0; }
    else {
    //
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    
    return (randMT_DeBruijn32[(uint32_t)(v * 0x07c4acddU) >> 27]);
    //
    }
}

static int randMT_uInt32NextPower2Index (uint32_t v)
{
    if (PD_IS_POWER_2 (v)) {
        return randMT_uInt32LogBase2Index (v);
    } else {
        return randMT_uInt32LogBase2Index (v) + 1;
    }
}

#endif // PD_BUILDING_TESTS

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_randMT *randMT_new (void)
{
    return genrand32_new();
}

PD_LOCAL void randMT_free (t_randMT *x)
{
    genrand32_free (x);
}

PD_LOCAL double randMT_getDouble (t_randMT *x)
{
    return genrand32_res53 (x);
}

#if defined ( PD_BUILDING_TESTS )

/* < http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/efaq.html > */

PD_LOCAL long randMT_getInteger (t_randMT *x, long v)
{
    if (v <= 1) { return 0; }
    else {
    //
    long r = 0;
    long k = randMT_uInt32NextPower2Index ((uint32_t)v);
    
    do {
        r = (long)(genrand32_int32 (x) >> (32 - k));
    } while (r >= v);
    
    return r;
    //
    }
}

#endif // PD_BUILDING_TESTS

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_32BIT

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if PD_64BIT

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

static const int randMT_DeBruijn64[] =      /* Static. */
    {
        63, 0, 58, 1, 59, 47, 53, 2, 60, 39, 48, 27, 54, 33, 42, 3,
        61, 51, 37, 40, 49, 18, 28, 20, 55, 30, 34, 11, 43, 14, 22, 4,
        62, 57, 46, 52, 38, 26, 32, 41, 50, 36, 17, 19, 29, 10, 13, 21,
        56, 45, 25, 31, 35, 16, 9, 12, 44, 24, 15, 8, 23, 7, 6, 5
    };
    
static int randMT_uInt64LogBase2Index (uint64_t v)
{
    if (!v) { return 0; }
    else {
    //
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    
    return (randMT_DeBruijn64[((uint64_t)((v - (v >> 1)) * 0x07edd5e59a4e28c2ULL)) >> 58]);
    //
    }
}

static int randMT_uInt64NextPower2Index (uint64_t v)
{
    if (PD_IS_POWER_2 (v)) {
        return randMT_uInt64LogBase2Index (v);
    } else {
        return randMT_uInt64LogBase2Index (v) + 1;
    }
}

#endif // PD_BUILDING_TESTS

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_randMT *randMT_new (void)
{
    return genrand64_new();
}

PD_LOCAL void randMT_free (t_randMT *x)
{
    genrand64_free (x);
}

PD_LOCAL double randMT_getDouble (t_randMT *x)
{
    return genrand64_real2 (x);
}

#if defined ( PD_BUILDING_TESTS )

/* < http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/efaq.html > */

PD_LOCAL long randMT_getInteger (t_randMT *x, long v)
{
    if (v <= 1) { return 0; }
    else {
    //
    long r = 0;
    long k = randMT_uInt64NextPower2Index ((uint64_t)v);
    
    do {
        r = (long)(genrand64_int64 (x) >> (64 - k));
    } while (r >= v);
    
    return r;
    //
    }
}

#endif // PD_BUILDING_TESTS

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_64BIT

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
