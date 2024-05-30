
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_clocks.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Clocks are triggered time-ordered? */

void test_taskTime (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    test_clocksFails |= (t < atomic_float64Read (&test_clocksTime));
    
    atomic_float64Write (&test_clocksTime, t);
}

void test_taskCount (void *x)
{
    test_clocksCounter++;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (ClocksTime, "Clocks - Time")

    if (ttt_testGetNumberOfThreads() >= 2) {
    //
    test_clocksInitialize ((t_method)test_taskTime, (t_method)test_taskCount, 1);
    
    if (ttt_testThreadsLaunch (test_clocksTask) != TTT_GOOD) { TTT_FAIL; }
    else {
    //
    test_clocksTick (2000.0);   /* All remaining clocks fired. */
    
    TTT_EXPECT (test_clocksCounter == TEST_LOOP_CLOCKS * TEST_CLOCKS_SIZE);
    TTT_EXPECT (test_clocksFails   == 0);
    TTT_EXPECT (test_clocksCheck() == 1);
    //
    }
    
    test_clocksRelease();
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
