
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_systime scheduler_addMillisecondsToLogicalTime (t_systime, double);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error clock_parseUnit (t_float f, t_symbol *s, t_float *n, int *isSamples)
{
    t_error err = (f <= 0.0);
    
    *n = 1; *isSamples = 0;
    
    if (!err) {
    //
    if (s == sym_permillisecond)    { *n = (t_float)(1.0 / f);     }
    else if (s == sym_persecond)    { *n = (t_float)(1000.0 / f);  }
    else if (s == sym_perminute)    { *n = (t_float)(60000.0 / f); }
    else if (s == sym_millisecond)  { *n = f;                      }
    else if (s == sym_second)       { *n = (t_float)(1000.0 * f);  }
    else if (s == sym_minute)       { *n = (t_float)(60000.0 * f); }
    else if (s == sym_sample)       { *n = f; *isSamples = 1;      }
    else {
        err = PD_ERROR;
    }
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void clock_setUnit (t_clock *x, double unit, int isSamples)
{
    double d = isSamples ? -unit : unit; atomic_float64Write (&x->c_unit, d);
}

t_error clock_setUnitParsed (t_clock *x, t_float f, t_symbol *unitName)
{
    t_float n; int isSamples;
    t_error err = clock_parseUnit (f, unitName, &n, &isSamples);
    
    if (!err) {
        if (n <= 0.0)  { n = 1.0; }
        if (isSamples) { clock_setUnit (x, n, 1); }
        else {
            clock_setUnit (x, n, 0);
        }
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_systime clock_getLogicalTime (t_clock *x)
{
    return (t_systime)atomic_float64Read (&x->c_systime);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Copy of systime used only while executed. */

t_systime clock_getExecuteTime (t_clock *x)
{
    return x->c_t;
}

void clock_setExecuteTime (t_clock *x, t_systime t)
{
    x->c_t = t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Proper order of operation for increment and decrement is not guaranted. */

/*

For instance:

1. Clock is removed while consumed in thread A.
2. Clock is added by thread B.
3. Counter is incremented by thread B (it is equal to  2).
4. Counter is decremented by thread A (it is equal to  1).

1. Clock is added by thread B.
2. Clock is removed while consumed in thread A.
3. Counter is decremented by thread A (it is equal to -1).
4. Counter is incremented by thread B (it is equal to  0).

*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void clock_increment (t_clock *x)
{
    int t = atomic_int32Increment (&x->c_count);
    
    PD_UNUSED (t); PD_ASSERT (t == 0 || t == 1 || t == 2);
}

void clock_decrement (t_clock *x)
{
    int t = atomic_int32Decrement (&x->c_count);
    
    PD_UNUSED (t); PD_ASSERT (t == 1 || t == 0 || t == -1);
}

int clock_count (t_clock *x)
{
    return atomic_int32Read (&x->c_count);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clock_inhibit (t_clock *x)
{
    x->c_fn = NULL;
}

void clock_execute (t_clock *x)
{
    if (x->c_fn) { (*x->c_fn)(x->c_owner); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_clock *clock_new (void *owner, t_method fn, int safe)
{
    t_clock *x = (t_clock *)PD_MEMORY_GET (sizeof (t_clock));
    
    x->c_fn    = (t_clockfn)fn;
    x->c_owner = owner ? owner : x;
    x->c_safe  = safe;

    atomic_float64Write (&x->c_unit, 1.0);
    
    return x;
}

t_clock *clock_newSingle (void *owner, t_method fn)
{
    return clock_new (owner, fn, 0);
}

t_clock *clock_newSafe (void *owner, t_method fn)
{
    return clock_new (owner, fn, 1);
}

void clock_free (t_clock *x)
{
    instance_clocksDestroy (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clock_unset (t_clock *x)
{
    instance_clocksRemove (x);
}

void clock_set (t_clock *x, t_systime t)
{
    clock_unset (x);
    
    {
    //
    atomic_float64Write (&x->c_systime, t);
    
    instance_clocksAdd (x);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static double clock_quantum (t_clock *x, double t)
{
    double d = 0.0;
    double u = atomic_float64Read (&x->c_unit);
    
    if (u > 0.0) { d = u; }
    else {
        d = -(u * (1000.0 / audio_getSampleRate()));
    }
    
    d *= PD_MAX (0.0, t);
    
    return d;
}

void clock_delay (t_clock *x, double delay)       /* Could be in milliseconds or in samples. */
{
    clock_set (x, scheduler_getLogicalTimeAfter (clock_quantum (x, delay)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error clock_reschedule (t_clock *x, double delay, double ms, t_systime t)
{
    t_systime now = scheduler_getLogicalTime();
    
    if (t < now) {
    //
    double u = clock_quantum (x, delay);
        
    if (now - t > ms) { return PD_ERROR; }                  /* Abort if it is too old. */
    if (u < 1.0)      { return PD_ERROR; }                  /* Abort if it is too small. */
   
    while (t < now) { t = scheduler_addMillisecondsToLogicalTime (t, u); }
    //
    }

    clock_set (x, t); return PD_ERROR_NONE;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
