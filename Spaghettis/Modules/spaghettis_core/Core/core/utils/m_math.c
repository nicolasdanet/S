
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_float math_midiToFrequency (t_float f)
{
    if (f <= -1500.0) { return 0.0; }
    else {
        f = (t_float)PD_MIN (f, 1499.0); return (t_float)(8.17579891564 * exp (0.0577622650 * f));
    }
}

t_float math_frequencyToMidi (t_float f)
{
    if (f <= 0.0) { return (t_float)-1500.0; }
    else { 
        return (t_float)(17.3123405046 * log (0.12231220585 * f));
    }
}

t_float math_powerToDecibel (t_float f)
{
    if (f <= 0.0) { return 0.0; }
    else {
        t_float t = (t_float)(100.0 + (10.0 / PD_LOG_TEN * log (f))); return (t < 0.0 ? 0.0 : t);
    }
}

t_float math_decibelToPower (t_float f)
{
    if (f <= 0.0) { return 0.0; }
    else {
        f = (t_float)PD_MIN (f, 870.0); return (t_float)(exp ((PD_LOG_TEN * 0.10) * (f - 100.0)));
    }
}

t_float math_rootMeanSquareToDecibel (t_float f)
{
    if (f <= 0.0) { return 0.0; }
    else {
        t_float t = (t_float)(100.0 + (20.0 / PD_LOG_TEN * log (f))); return (t < 0.0 ? 0.0 : t);
    }
}

t_float math_decibelToRootMeanSquare (t_float f)
{
    if (f <= 0.0) { return 0.0; }
    else {
        f = (t_float)PD_MIN (f, 485.0); return (t_float)(exp ((PD_LOG_TEN * 0.05) * (f - 100.0)));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int math_compareFloat (t_float a, t_float b)
{
    /* Basic implementation of comparaison function. */
    
    if (a < b) { return -1; } else if (b > a) { return 1; } else { return 0; }
}

t_float math_euclideanDistance (t_float a, t_float b, t_float c, t_float d)
{
    double x = ((double)c - a);
    double y = ((double)d - b);
    
    return (t_float)sqrt (x * x + y * y);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
