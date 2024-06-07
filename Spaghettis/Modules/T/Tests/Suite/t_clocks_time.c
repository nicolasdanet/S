
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Clocks are triggered time-ordered? */

/* All tasks are handled in the main thread. */

void test_timeTask (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    if (t < test_clocksTime) { test_clocksFails = 1; }
    
    test_clocksTime = t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (ClocksTime, "Clocks - Time")

    if (test_clocksRun ((t_method)test_counterTask, (t_method)test_timeTask, 1)) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_clocksCounter == TEST_LOOP_CLOCKS * TEST_CLOCKS_SIZE);
        TTT_EXPECT (test_clocksFails   == 0);
        TTT_EXPECT (test_clocksCheck   == 0);
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
