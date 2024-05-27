
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_clocks.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Clocks are triggered time-ordered? */

void test_taskTime (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    test_clocksFails |= (t < atomic_float64Read (&test_clocksTime));
    
    atomic_float64Write (&test_clocksTime, t);
    
    clock_set ((t_clock *)x, PD_RAND48_DOUBLE (test_clocksSeed) * 1500);        /* Rescheduled. */
}

void test_taskCount (void *x)
{
    test_clocksCounter++;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksDoSomethingTimed (TTTThreadProperties *p, int j)
{
    int i = ttt_getRandomInteger (p, TEST_CLOCKS_SIZE);
    
    if (ttt_getRandomInteger (p, 2)) {
        clock_set (test_clocksA[i], ttt_getRandomInteger (p, 1500));
    } else {
        clock_unset (test_clocksA[i]);
    }
}

void test_clocksDoSomethingCounted (TTTThreadProperties *p, int j)
{
    clock_set (test_clocksB[j], ttt_getRandomInteger (p, 500));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
