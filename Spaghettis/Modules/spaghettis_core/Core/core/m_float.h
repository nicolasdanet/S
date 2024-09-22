
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_FLOAT_H_
#define M_FLOAT_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Assumed IEEE 754 floating-point format. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* True if zero, denormal, infinite, or NaN. */

static inline int PD_FLOAT32_IS_INVALID_OR_ZERO (float f)
{
    t_pun32 z;
    pun32_setFloat (&z, f);
    pun32_setBitwiseAnd (&z, 0x7f800000);
    uint32_t i = pun32_getInteger (&z);
    
    return ((i == 0) || (i == 0x7f800000));
}

static inline int PD_FLOAT64_IS_INVALID_OR_ZERO (double f)
{
    t_pun64 z;
    pun64_setDouble (&z, f);
    pun64_setBitwiseAndMostSignificantBytes (&z, 0x7ff00000);
    uint32_t i = pun64_getMostSignificantBytes (&z);
    
    return ((i == 0) || (i == 0x7ff00000));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Workaround required with ffast-math flag. */

static inline int PD_FLOAT64_IS_INF (double f)
{
    t_pun64 z;
    pun64_setDouble (&z, f);
    uint64_t i = pun64_getLongInteger (&z);
    
    return (i == 0x7ff0000000000000UL || i == 0xfff0000000000000UL);
}

static inline int PD_FLOAT64_IS_NAN (double f)
{
    t_pun64 z;
    pun64_setDouble (&z, f);
    pun64_setBitwiseAndMostSignificantBytes (&z, 0x7ff00000);
    uint32_t i = pun64_getMostSignificantBytes (&z);

    return ((i == 0x7ff00000) && !PD_FLOAT64_IS_INF (f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* True if exponent falls out (-64, 64) range. */
/* Notice that it is also true for zero, denormal, infinite, or NaN. */

static inline int PD_FLOAT32_IS_BIG_OR_SMALL (float f)
{
    t_pun32 z;
    pun32_setFloat (&z, f);
    uint32_t i = pun32_getInteger (&z);
    
    return ((i & 0x20000000) == ((i >> 1) & 0x20000000));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int PD_FLOAT64_IS_INTEGER (double f)
{
    return (trunc (f) - f == 0.0);
}

static inline int PD_FLOAT32_IS_INTEGER (float f)
{
    return (truncf (f) - f == 0.0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_FLOAT_H_
