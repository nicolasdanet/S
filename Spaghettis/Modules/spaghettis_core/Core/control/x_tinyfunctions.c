
/*
 * TINYEXPR - Tiny recursive descent parser and evaluation engine in C
 *
 * Copyright (c) 2015-2019 Lewis Van Winkle
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgement in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/* http://CodePlea.com */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Notice that this file is a largely modified version of original linked below. */

/* < https://github.com/codeplea/tinyexpr/blob/ffb0d41b13e5f8d318db95feb071c220c134fe70/tinyexpr.c > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_randMT *fn_randMT;         /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static double fn_abs (double f)
{
    return fabs (f);
}

static double fn_acos (double f)
{
    return acos (f);
}

static double fn_asin (double f)
{
    return asin (f);
}

static double fn_atan (double f)
{
    return atan (f);
}

static double fn_atan2 (double f, double g)
{
    return atan2 (f, g);
}

static double fn_ceil (double f)
{
    return ceil (f);
}

static double fn_cos (double f)
{
    return cos (f);
}

static double fn_cosh (double f)
{
    return cosh (f);
}

static double fn_e (void)
{
    return PD_E;
}

double fn_eq (double f, double g)
{
    return (f == g);
}

static double fn_exp (double f)
{
    return exp (f);
}

static double fn_fac (double f)
{
    if (f < 0.0)           { return TE_NAN; }
    else if (f > UINT_MAX) { return TE_INFINITY; }
    else {
    //
    uint32_t a = (uint32_t)f;
    uint64_t k = 1;
    uint64_t i;
    
    for (i = 1; i <= a; i++) {
        if (i > ULONG_MAX / k) { return TE_INFINITY; }
        else {
            k *= i;
        }
    }
    
    return (double)k;
    //
    }
}

static double fn_floor (double f)
{
    return floor (f);
}

static double fn_fmod (double f, double g)
{
    return fmod (f, g);
}

static double fn_ftz (double f)
{
    if (PD_FLOAT64_IS_INVALID_OR_ZERO (f)) { return 0.0; } else { return f; }
}

static double fn_ge (double f, double g)
{
    return (f >= g);
}

static double fn_gt (double f, double g)
{
    return (f > g);
}

static double fn_isinf (double f)
{
    return (double)PD_FLOAT64_IS_INF (f);
}

static double fn_isnan (double f)
{
    return (double)PD_FLOAT64_IS_NAN (f);
}

static double fn_le (double f, double g)
{
    return (f <= g);
}

static double fn_log (double f)
{
    return log (f);
}

static double fn_log10 (double f)
{
    return log10 (f);
}

static double fn_lt (double f, double g)
{
    return (f < g);
}

static double fn_max (double f, double g)
{
    return PD_MAX (f, g);
}

static double fn_min (double f, double g)
{
    return PD_MIN (f, g);
}

static double fn_ncr (double f, double g)
{
    if (f < 0.0 || g < 0.0 || f < g)       { return TE_NAN; }
    else if (f > UINT_MAX || g > UINT_MAX) { return TE_INFINITY; }
    else {
    //
    uint64_t n = (uint64_t)f;
    uint64_t r = (uint64_t)g;
    uint64_t k = 1;
    uint64_t i;
    
    if (r > n / 2) { r = n - r; }
    
    for (i = 1; i <= r; i++) {
        if (k > ULONG_MAX / (n - r + i)) { return TE_INFINITY; }
        else {
            k *= n - r + i;
            k /= i;
        }
    }
    
    return (double)k;
    //
    }
}

static double fn_ne (double f, double g)
{
    return (f != g);
}

static double fn_npr (double f, double g)
{
    return (fn_ncr (f, g) * fn_fac (g));
}

static double fn_pi (void)
{
    return PD_PI;
}

static double fn_pow (double f, double g)
{
    return pow (f, g);
}

static double fn_rand (void)
{
    static int once = 0; static t_rand48 seed = 0;      /* Static. */
    
    if (!once) { once = 1; seed = PD_RAND48_SEED; }
    
    return PD_RAND48_DOUBLE (seed);
}

static double fn_randmt (void)
{
    PD_ASSERT (fn_randMT); return randMT_getDouble (fn_randMT);
}

static double fn_sin (double f)
{
    return sin (f);
}

static double fn_sinh (double f)
{
    return sinh (f);
}

static double fn_sqrt (double f)
{
    return sqrt (f);
}

static double fn_tan (double f)
{
    return tan (f);
}

static double fn_tanh (double f)
{
    return tanh (f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static double fn_add (double f, double g)
{
    return (f + g);
}

static double fn_substract (double f, double g)
{
    return (f - g);
}
static double fn_multiply (double f, double g)
{
    return (f * g);
}

static double fn_divide (double f, double g)
{
    return (f / g);
}

static double fn_negate (double f)
{
    return (-f);
}

static double fn_comma (double f, double g)
{
    (void)f; return g;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void fn_initialize (void)
{
    fn_randMT = randMT_new();
}

void fn_release (void)
{
    randMT_free (fn_randMT);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
