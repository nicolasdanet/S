
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Clocks are triggered time-ordered? */

void test_timeTask (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    if (t < atomic_float64Read (&test_clocksTime)) { test_clocksFails = 1; }
    
    atomic_float64Write (&test_clocksTime, t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (ClocksTime, "Clocks - Time")

    if (test_clocksRun ((t_method)test_timeTask, 1)) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_clocksCounter == TEST_LOOP_CLOCKS * TEST_CLOCKS_SIZE);
        TTT_EXPECT (test_clocksFails   == 0);
        TTT_EXPECT (test_clocksCheck() == 1);
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
