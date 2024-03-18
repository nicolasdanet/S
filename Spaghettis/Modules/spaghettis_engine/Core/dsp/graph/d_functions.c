
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_int *perform_zero                    (t_int *);
t_int *perform_scalar                  (t_int *);
t_int *perform_copy                    (t_int *);
t_int *perform_copyZero                (t_int *);
t_int *perform_squareRoot              (t_int *);
t_int *perform_inverseSquareRoot       (t_int *);
t_int *perform_plusAliased             (t_int *);
t_int *perform_plusScalar              (t_int *);
t_int *perform_subtractAliased         (t_int *);
t_int *perform_subtractScalar          (t_int *);
t_int *perform_multiplyAliased         (t_int *);
t_int *perform_multiplyScalar          (t_int *);
t_int *perform_divideAliased           (t_int *);
t_int *perform_divideScalar            (t_int *);
t_int *perform_maximumAliased          (t_int *);
t_int *perform_maximumScalar           (t_int *);
t_int *perform_minimumAliased          (t_int *);
t_int *perform_minimumScalar           (t_int *);
t_int *perform_greaterAliased          (t_int *);
t_int *perform_greaterScalar           (t_int *);
t_int *perform_lessAliased             (t_int *);
t_int *perform_lessScalar              (t_int *);
t_int *perform_magnitude               (t_int *);
t_int *perform_inverseMagnitude        (t_int *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_int *vPerform_zero                   (t_int *);
t_int *vPerform_scalar                 (t_int *);
t_int *vPerform_copy                   (t_int *);
t_int *vPerform_copyZero               (t_int *);
t_int *vPerform_squareRoot             (t_int *);
t_int *vPerform_inverseSquareRoot      (t_int *);
t_int *vPerform_plusAliased            (t_int *);
t_int *vPerform_plusScalar             (t_int *);
t_int *vPerform_subtractAliased        (t_int *);
t_int *vPerform_subtractScalar         (t_int *);
t_int *vPerform_multiplyAliased        (t_int *);
t_int *vPerform_multiplyScalar         (t_int *);
t_int *vPerform_divideAliased          (t_int *);
t_int *vPerform_divideScalar           (t_int *);
t_int *vPerform_maximumAliased         (t_int *);
t_int *vPerform_maximumScalar          (t_int *);
t_int *vPerform_minimumAliased         (t_int *);
t_int *vPerform_minimumScalar          (t_int *);
t_int *vPerform_greaterAliased         (t_int *);
t_int *vPerform_greaterScalar          (t_int *);
t_int *vPerform_lessAliased            (t_int *);
t_int *vPerform_lessScalar             (t_int *);
t_int *vPerform_magnitude              (t_int *);
t_int *vPerform_inverseMagnitude       (t_int *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void dsp_addZeroPerform (PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add2 (perform_zero, dest, n); }
    else {
        dsp_add2 (vPerform_zero, dest, n);
    }
}

void dsp_addScalarPerform (t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add3 (perform_scalar, f, dest, n); }
    else {
        dsp_add3 (vPerform_scalar, f, dest, n);
    }
}

void dsp_addCopyPerform (PD_RESTRICTED src, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add3 (perform_copy, src, dest, n); }
    else {
        dsp_add3 (vPerform_copy, src, dest, n);
    }
}

void dsp_addCopyZeroPerform (PD_RESTRICTED src, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add3 (perform_copyZero, src, dest, n); }
    else {
        dsp_add3 (vPerform_copyZero, src, dest, n);
    }
}

void dsp_addSquareRootPerform (PD_RESTRICTED src, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add3 (perform_squareRoot, src, dest, n); }
    else {
        dsp_add3 (vPerform_squareRoot, src, dest, n);
    }
}

void dsp_addInverseSquareRootPerform (PD_RESTRICTED src, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add3 (perform_inverseSquareRoot, src, dest, n); }
    else {
        dsp_add3 (vPerform_inverseSquareRoot, src, dest, n);
    }
}

void dsp_addPlusPerformAliased (t_sample *src1, t_sample *src2, t_sample *dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_plusAliased, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_plusAliased, src1, src2, dest, n);
    }
}

void dsp_addPlusScalarPerform (PD_RESTRICTED src, t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_plusScalar, src, f, dest, n); }
    else {
        dsp_add4 (vPerform_plusScalar, src, f, dest, n);
    }
}

void dsp_addSubtractPerformAliased (t_sample *src1, t_sample *src2, t_sample *dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_subtractAliased, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_subtractAliased, src1, src2, dest, n);
    }
}

void dsp_addSubtractScalarPerform (PD_RESTRICTED src, t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_subtractScalar, src, f, dest, n); }
    else {
        dsp_add4 (vPerform_subtractScalar, src, f, dest, n);
    }
}

void dsp_addMultiplyPerformAliased (t_sample *src1, t_sample *src2, t_sample *dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_multiplyAliased, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_multiplyAliased, src1, src2, dest, n);
    }
}

void dsp_addMultiplyScalarPerform (PD_RESTRICTED src, t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));

    if (n & 7) { dsp_add4 (perform_multiplyScalar, src, f, dest, n); }
    else {
        dsp_add4 (vPerform_multiplyScalar, src, f, dest, n);
    }
}

void dsp_addDividePerformAliased (t_sample *src1, t_sample *src2, t_sample *dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_divideAliased, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_divideAliased, src1, src2, dest, n);
    }
}

void dsp_addDivideScalarPerform (PD_RESTRICTED src, t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_divideScalar, src, f, dest, n); }
    else {
        dsp_add4 (vPerform_divideScalar, src, f, dest, n);
    }
}

void dsp_addMaximumPerformAliased (t_sample *src1, t_sample *src2, t_sample *dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_maximumAliased, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_maximumAliased, src1, src2, dest, n);
    }
}

void dsp_addMaximumScalarPerform (PD_RESTRICTED src, t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_maximumScalar, src, f, dest, n); }
    else {
        dsp_add4 (vPerform_maximumScalar, src, f, dest, n);
    }
}

void dsp_addMinimumPerformAliased (t_sample *src1, t_sample *src2, t_sample *dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_minimumAliased, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_minimumAliased, src1, src2, dest, n);
    }
}

void dsp_addMinimumScalarPerform (PD_RESTRICTED src, t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_minimumScalar, src, f, dest, n); }
    else {
        dsp_add4 (vPerform_minimumScalar, src, f, dest, n);
    }
}

void dsp_addGreaterPerformAliased (t_sample *src1, t_sample *src2, t_sample *dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_greaterAliased, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_greaterAliased, src1, src2, dest, n);
    }
}

void dsp_addGreaterScalarPerform (PD_RESTRICTED src, t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_greaterScalar, src, f, dest, n); }
    else {
        dsp_add4 (vPerform_greaterScalar, src, f, dest, n);
    }
}

void dsp_addLessPerformAliased (t_sample *src1, t_sample *src2, t_sample *dest, int n)
{
    PD_ASSERT (n > 0);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_lessAliased, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_lessAliased, src1, src2, dest, n);
    }
}

void dsp_addLessScalarPerform (PD_RESTRICTED src, t_float64Atomic *f, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_lessScalar, src, f, dest, n); }
    else {
        dsp_add4 (vPerform_lessScalar, src, f, dest, n);
    }
}

void dsp_addMagnitudePerform (PD_RESTRICTED src1, PD_RESTRICTED src2, PD_RESTRICTED dest, int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src1 != dest);
    PD_ASSERT (src2 != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_magnitude, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_magnitude, src1, src2, dest, n);
    }
}

void dsp_addInverseMagnitudePerform (PD_RESTRICTED src1,
    PD_RESTRICTED src2,
    PD_RESTRICTED dest,
    int n)
{
    PD_ASSERT (n > 0);
    PD_ASSERT (src1 != dest);
    PD_ASSERT (src2 != dest);
    
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src1));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (src2));
    PD_ASSERT (!PD_ASSUME_ALIGNED || PD_IS_ALIGNED_16 (dest));
    
    if (n & 7) { dsp_add4 (perform_inverseMagnitude, src1, src2, dest, n); }
    else {
        dsp_add4 (vPerform_inverseMagnitude, src1, src2, dest, n);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
