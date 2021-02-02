
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* No aliasing. */

PD_LOCAL t_int *vPerform_zero (t_int *w)
{
    PD_RESTRICTED s = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    int n = (int)(w[2]);
    
    while (n) {
    //
    s[0] = 0;
    s[1] = 0;
    s[2] = 0;
    s[3] = 0;
    s[4] = 0;
    s[5] = 0;
    s[6] = 0;
    s[7] = 0;
    n -= 8;
    s += 8;
    //
    }
    
    return (w + 3);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_scalar (t_int *w)
{
    t_float64Atomic *t = (t_float64Atomic *)(w[1]);
    t_sample f = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n) {
    //
    s[0] = f;
    s[1] = f;
    s[2] = f;
    s[3] = f;
    s[4] = f;
    s[5] = f;
    s[6] = f;
    s[7] = f;
    
    n -= 8;
    s += 8;
    //
    }
    
    return (w + 4);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_copy (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];
    
    s2[0] = f0;
    s2[1] = f1;
    s2[2] = f2;
    s2[3] = f3;
    s2[4] = f4;
    s2[5] = f5;
    s2[6] = f6;
    s2[7] = f7;
    
    n -= 8; 
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 4);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_copyZero (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];
    
    s2[0] = f0;
    s2[1] = f1;
    s2[2] = f2;
    s2[3] = f3;
    s2[4] = f4;
    s2[5] = f5;
    s2[6] = f6;
    s2[7] = f7;

    s1[0] = 0;
    s1[1] = 0;
    s1[2] = 0;
    s1[3] = 0; 
    s1[4] = 0;
    s1[5] = 0;
    s1[6] = 0;
    s1[7] = 0; 
   
    n -= 8; 
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 4);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_squareRoot (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];
    
    s2[0] = sqrt_fast (f0);
    s2[1] = sqrt_fast (f1);
    s2[2] = sqrt_fast (f2);
    s2[3] = sqrt_fast (f3);
    s2[4] = sqrt_fast (f4);
    s2[5] = sqrt_fast (f5);
    s2[6] = sqrt_fast (f6);
    s2[7] = sqrt_fast (f7);

    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 4);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_inverseSquareRoot (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    int n = (int)(w[3]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];
    
    s2[0] = rsqrt_fast (f0);
    s2[1] = rsqrt_fast (f1);
    s2[2] = rsqrt_fast (f2);
    s2[3] = rsqrt_fast (f3);
    s2[4] = rsqrt_fast (f4);
    s2[5] = rsqrt_fast (f5);
    s2[6] = rsqrt_fast (f6);
    s2[7] = rsqrt_fast (f7);

    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 4);
}

/* Aliasing. */

PD_LOCAL t_int *vPerform_plusAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = f0 + g0;
    s3[1] = f1 + g1;
    s3[2] = f2 + g2;
    s3[3] = f3 + g3;
    s3[4] = f4 + g4;
    s3[5] = f5 + g5;
    s3[6] = f6 + g6;
    s3[7] = f7 + g7;
    
    n -= 8; 
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_plusScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample g = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    s2[0] = f0 + g;
    s2[1] = f1 + g;
    s2[2] = f2 + g;
    s2[3] = f3 + g;
    s2[4] = f4 + g;
    s2[5] = f5 + g;
    s2[6] = f6 + g;
    s2[7] = f7 + g;
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 5);
}

/* Aliasing. */

PD_LOCAL t_int *vPerform_subtractAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = f0 - g0;
    s3[1] = f1 - g1;
    s3[2] = f2 - g2;
    s3[3] = f3 - g3;
    s3[4] = f4 - g4;
    s3[5] = f5 - g5;
    s3[6] = f6 - g6;
    s3[7] = f7 - g7;
    
    n -= 8; 
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_subtractScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample g = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    s2[0] = f0 - g;
    s2[1] = f1 - g;
    s2[2] = f2 - g;
    s2[3] = f3 - g;
    s2[4] = f4 - g;
    s2[5] = f5 - g;
    s2[6] = f6 - g;
    s2[7] = f7 - g;
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 5);
}

/* Aliasing. */

PD_LOCAL t_int *vPerform_multiplyAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = f0 * g0;
    s3[1] = f1 * g1;
    s3[2] = f2 * g2;
    s3[3] = f3 * g3;
    s3[4] = f4 * g4;
    s3[5] = f5 * g5;
    s3[6] = f6 * g6;
    s3[7] = f7 * g7;
    
    n -= 8; 
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_multiplyScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample g = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    s2[0] = f0 * g;
    s2[1] = f1 * g;
    s2[2] = f2 * g;
    s2[3] = f3 * g;
    s2[4] = f4 * g;
    s2[5] = f5 * g;
    s2[6] = f6 * g;
    s2[7] = f7 * g;
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 5);
}

/* Aliasing. */

PD_LOCAL t_int *vPerform_divideAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = g0 ? f0 / g0 : 0;
    s3[1] = g1 ? f1 / g1 : 0;
    s3[2] = g2 ? f2 / g2 : 0;
    s3[3] = g3 ? f3 / g3 : 0;
    s3[4] = g4 ? f4 / g4 : 0;
    s3[5] = g5 ? f5 / g5 : 0;
    s3[6] = g6 ? f6 / g6 : 0;
    s3[7] = g7 ? f7 / g7 : 0;
    
    n -= 8; 
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_divideScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample g = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    if (g) { g = (t_sample)(1.0 / g); }
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    s2[0] = f0 * g;
    s2[1] = f1 * g;
    s2[2] = f2 * g;
    s2[3] = f3 * g;
    s2[4] = f4 * g;
    s2[5] = f5 * g;
    s2[6] = f6 * g;
    s2[7] = f7 * g;
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 5);
}

/* Aliasing. */

PD_LOCAL t_int *vPerform_maximumAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = PD_MAX (f0, g0);
    s3[1] = PD_MAX (f1, g1);
    s3[2] = PD_MAX (f2, g2);
    s3[3] = PD_MAX (f3, g3);
    s3[4] = PD_MAX (f4, g4);
    s3[5] = PD_MAX (f5, g5);
    s3[6] = PD_MAX (f6, g6);
    s3[7] = PD_MAX (f7, g7);
    
    n -= 8; 
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_maximumScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample g = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    s2[0] = PD_MAX (f0, g);
    s2[1] = PD_MAX (f1, g);
    s2[2] = PD_MAX (f2, g);
    s2[3] = PD_MAX (f3, g);
    s2[4] = PD_MAX (f4, g);
    s2[5] = PD_MAX (f5, g);
    s2[6] = PD_MAX (f6, g);
    s2[7] = PD_MAX (f7, g);
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 5);
}

/* Aliasing. */

PD_LOCAL t_int *vPerform_minimumAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = PD_MIN (f0, g0);
    s3[1] = PD_MIN (f1, g1);
    s3[2] = PD_MIN (f2, g2);
    s3[3] = PD_MIN (f3, g3);
    s3[4] = PD_MIN (f4, g4);
    s3[5] = PD_MIN (f5, g5);
    s3[6] = PD_MIN (f6, g6);
    s3[7] = PD_MIN (f7, g7);
    
    n -= 8; 
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_minimumScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample g = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    s2[0] = PD_MIN (f0, g);
    s2[1] = PD_MIN (f1, g);
    s2[2] = PD_MIN (f2, g);
    s2[3] = PD_MIN (f3, g);
    s2[4] = PD_MIN (f4, g);
    s2[5] = PD_MIN (f5, g);
    s2[6] = PD_MIN (f6, g);
    s2[7] = PD_MIN (f7, g);
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 5);
}

/* Aliasing. */

PD_LOCAL t_int *vPerform_greaterAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = (t_sample)(f0 > g0);
    s3[1] = (t_sample)(f1 > g1);
    s3[2] = (t_sample)(f2 > g2);
    s3[3] = (t_sample)(f3 > g3);
    s3[4] = (t_sample)(f4 > g4);
    s3[5] = (t_sample)(f5 > g5);
    s3[6] = (t_sample)(f6 > g6);
    s3[7] = (t_sample)(f7 > g7);
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_greaterScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample g = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    s2[0] = (t_sample)(f0 > g);
    s2[1] = (t_sample)(f1 > g);
    s2[2] = (t_sample)(f2 > g);
    s2[3] = (t_sample)(f3 > g);
    s2[4] = (t_sample)(f4 > g);
    s2[5] = (t_sample)(f5 > g);
    s2[6] = (t_sample)(f6 > g);
    s2[7] = (t_sample)(f7 > g);
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 5);
}

/* Aliasing. */

PD_LOCAL t_int *vPerform_lessAliased (t_int *w)
{
    t_sample *s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_sample *s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    t_sample *s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = (t_sample)(f0 < g0);
    s3[1] = (t_sample)(f1 < g1);
    s3[2] = (t_sample)(f2 < g2);
    s3[3] = (t_sample)(f3 < g3);
    s3[4] = (t_sample)(f4 < g4);
    s3[5] = (t_sample)(f5 < g5);
    s3[6] = (t_sample)(f6 < g6);
    s3[7] = (t_sample)(f7 < g7);
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_lessScalar (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    t_float64Atomic *t = (t_float64Atomic *)(w[2]);
    t_sample g = PD_ATOMIC_FLOAT64_READ (t);
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    s2[0] = (t_sample)(f0 < g);
    s2[1] = (t_sample)(f1 < g);
    s2[2] = (t_sample)(f2 < g);
    s2[3] = (t_sample)(f3 < g);
    s2[4] = (t_sample)(f4 < g);
    s2[5] = (t_sample)(f5 < g);
    s2[6] = (t_sample)(f6 < g);
    s2[7] = (t_sample)(f7 < g);
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_magnitude (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    PD_RESTRICTED s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = sqrt_fast (f0 * f0 + g0 * g0);
    s3[1] = sqrt_fast (f1 * f1 + g1 * g1);
    s3[2] = sqrt_fast (f2 * f2 + g2 * g2);
    s3[3] = sqrt_fast (f3 * f3 + g3 * g3);
    s3[4] = sqrt_fast (f4 * f4 + g4 * g4);
    s3[5] = sqrt_fast (f5 * f5 + g5 * g5);
    s3[6] = sqrt_fast (f6 * f6 + g6 * g6);
    s3[7] = sqrt_fast (f7 * f7 + g7 * g7);
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

/* No aliasing. */

PD_LOCAL t_int *vPerform_inverseMagnitude (t_int *w)
{
    PD_RESTRICTED s1 = (t_sample *)PD_ALIGNED ((t_sample *)(w[1]));
    PD_RESTRICTED s2 = (t_sample *)PD_ALIGNED ((t_sample *)(w[2]));
    PD_RESTRICTED s3 = (t_sample *)PD_ALIGNED ((t_sample *)(w[3]));
    int n = (int)(w[4]);
    
    while (n) {
    //
    t_sample f0 = s1[0];
    t_sample f1 = s1[1];
    t_sample f2 = s1[2];
    t_sample f3 = s1[3];
    t_sample f4 = s1[4];
    t_sample f5 = s1[5];
    t_sample f6 = s1[6];
    t_sample f7 = s1[7];

    t_sample g0 = s2[0];
    t_sample g1 = s2[1];
    t_sample g2 = s2[2];
    t_sample g3 = s2[3];
    t_sample g4 = s2[4];
    t_sample g5 = s2[5];
    t_sample g6 = s2[6];
    t_sample g7 = s2[7];

    s3[0] = rsqrt_fast (f0 * f0 + g0 * g0);
    s3[1] = rsqrt_fast (f1 * f1 + g1 * g1);
    s3[2] = rsqrt_fast (f2 * f2 + g2 * g2);
    s3[3] = rsqrt_fast (f3 * f3 + g3 * g3);
    s3[4] = rsqrt_fast (f4 * f4 + g4 * g4);
    s3[5] = rsqrt_fast (f5 * f5 + g5 * g5);
    s3[6] = rsqrt_fast (f6 * f6 + g6 * g6);
    s3[7] = rsqrt_fast (f7 * f7 + g7 * g7);
    
    n -= 8;
    s1 += 8;
    s2 += 8;
    s3 += 8;
    //
    }
    
    return (w + 5);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
