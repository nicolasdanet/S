
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* No aliasing. */

t_int *perform_zero (t_int *w)
{
    PD_RESTRICTED s = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    int n = (int)(w[2]);
    
    memset (s, 0, n * sizeof (t_sample));
    
    return (w + 3);
}

/* No aliasing. */

t_int *perform_scalar (t_int *w)
{
    t_float64Atomic *t = (t_float64Atomic *)(w[1]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n--) { *s++ = f; }
    
    return (w + 4);
}

/* No aliasing. */

t_int *perform_copy (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n--) { *s2 = *s1; s2++; s1++; }
    
    return (w + 4);
}

/* No aliasing. */

t_int *perform_copyZero (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n--) { *s2 = *s1; *s1 = 0; s2++; s1++; }
    
    return (w + 4);
}

/* No aliasing. */

t_int *perform_squareRoot (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n--) { *s2 = sqrt_fast (*s1); s2++; s1++; }
    
    return (w + 4);
}

/* No aliasing. */

t_int *perform_inverseSquareRoot (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n--) { *s2 = rsqrt_fast (*s1); s2++; s1++; }
    
    return (w + 4);
}

/* Aliasing. */

t_int *perform_plusAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = f1 + f2;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_plusScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) { *s2 = *s1 + f; s2++; s1++; }
    
    return (w + 5);
}

/* Aliasing. */

t_int *perform_subtractAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = f1 - f2;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_subtractScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) { *s2 = *s1 - f; s2++; s1++; }
    
    return (w + 5);
}

/* Aliasing. */

t_int *perform_multiplyAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = f1 * f2;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_multiplyScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) { *s2 = *s1 * f; s2++; s1++; }
    
    return (w + 5);
}

/* Aliasing. */

t_int *perform_divideAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    if (f2) { *s3++ = f1 / f2; }
    else {
        *s3++ = 0;
    }
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_divideScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    if (f) { *s2 = *s1 / f; }
    else {
        *s2 = 0;
    }
    
    s2++; s1++;
    //
    }
    
    return (w + 5);
}

/* Aliasing. */

t_int *perform_maximumAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = PD_MAX (f1, f2);
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_maximumScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) { *s2 = PD_MAX (*s1, f); s2++; s1++; }
    
    return (w + 5);
}

/* Aliasing. */

t_int *perform_minimumAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = PD_MIN (f1, f2);
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_minimumScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) { *s2 = PD_MIN (*s1, f); s2++; s1++; }
    
    return (w + 5);
}

/* Aliasing. */

t_int *perform_greaterAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = (t_sample)(f1 > f2);
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_greaterScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) { *s2 = (t_sample)(*s1 > f); s2++; s1++; }
    
    return (w + 5);
}

/* Aliasing. */

t_int *perform_lessAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = (t_sample)(f1 < f2);
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_lessScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) { *s2 = (t_sample)(*s1 < f); s2++; s1++; }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_magnitude (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    PD_RESTRICTED s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = sqrt_fast (f1 * f1 + f2 * f2);
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

t_int *perform_inverseMagnitude (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    PD_RESTRICTED s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f1 = *s1++;
    t_sample f2 = *s2++;
    
    *s3++ = rsqrt_fast (f1 * f1 + f2 * f2);
    //
    }
    
    return (w + 5);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
