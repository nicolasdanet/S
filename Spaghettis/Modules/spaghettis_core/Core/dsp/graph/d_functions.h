
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef D_PERFORM_H_
#define D_PERFORM_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void dsp_addZeroPerform                (PD_RESTRICTED d, int n);
void dsp_addScalarPerform              (t_float64Atomic *f, PD_RESTRICTED d, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void dsp_addCopyPerform                (PD_RESTRICTED s, PD_RESTRICTED d, int n);
void dsp_addCopyZeroPerform            (PD_RESTRICTED s, PD_RESTRICTED d, int n);
void dsp_addSquareRootPerform          (PD_RESTRICTED s, PD_RESTRICTED d, int n);
void dsp_addInverseSquareRootPerform   (PD_RESTRICTED s, PD_RESTRICTED d, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void dsp_addPlusPerformAliased         (t_sample *s1, t_sample *s2, t_sample *d, int n);
void dsp_addSubtractPerformAliased     (t_sample *s1, t_sample *s2, t_sample *d, int n);
void dsp_addMultiplyPerformAliased     (t_sample *s1, t_sample *s2, t_sample *d, int n);
void dsp_addDividePerformAliased       (t_sample *s1, t_sample *s2, t_sample *d, int n);
void dsp_addMaximumPerformAliased      (t_sample *s1, t_sample *s2, t_sample *d, int n);
void dsp_addMinimumPerformAliased      (t_sample *s1, t_sample *s2, t_sample *d, int n);
void dsp_addGreaterPerformAliased      (t_sample *s1, t_sample *s2, t_sample *d, int n);
void dsp_addLessPerformAliased         (t_sample *s1, t_sample *s2, t_sample *d, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void dsp_addPlusScalarPerform          (PD_RESTRICTED s, t_float64Atomic *f, PD_RESTRICTED d, int n);
void dsp_addSubtractScalarPerform      (PD_RESTRICTED s, t_float64Atomic *f, PD_RESTRICTED d, int n);
void dsp_addMultiplyScalarPerform      (PD_RESTRICTED s, t_float64Atomic *f, PD_RESTRICTED d, int n);
void dsp_addDivideScalarPerform        (PD_RESTRICTED s, t_float64Atomic *f, PD_RESTRICTED d, int n);
void dsp_addMaximumScalarPerform       (PD_RESTRICTED s, t_float64Atomic *f, PD_RESTRICTED d, int n);
void dsp_addMinimumScalarPerform       (PD_RESTRICTED s, t_float64Atomic *f, PD_RESTRICTED d, int n);
void dsp_addGreaterScalarPerform       (PD_RESTRICTED s, t_float64Atomic *f, PD_RESTRICTED d, int n);
void dsp_addLessScalarPerform          (PD_RESTRICTED s, t_float64Atomic *f, PD_RESTRICTED d, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void dsp_addMagnitudePerform           (PD_RESTRICTED s1, PD_RESTRICTED s2, PD_RESTRICTED d, int n);
void dsp_addInverseMagnitudePerform    (PD_RESTRICTED s1, PD_RESTRICTED s2, PD_RESTRICTED d, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // D_PERFORM_H_
