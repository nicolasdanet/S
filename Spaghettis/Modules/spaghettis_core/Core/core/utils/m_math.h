
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_MATH_H_
#define M_MATH_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_float    math_midiToFrequency            (t_float f);
t_float    math_frequencyToMidi            (t_float f);
t_float    math_rootMeanSquareToDecibel    (t_float f);
t_float    math_decibelToRootMeanSquare    (t_float f);
t_float    math_powerToDecibel             (t_float f);
t_float    math_decibelToPower             (t_float f);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int        math_compareFloat               (t_float a, t_float b);
t_float    math_euclideanDistance          (t_float a, t_float b, t_float c, t_float d);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_MATH_H_