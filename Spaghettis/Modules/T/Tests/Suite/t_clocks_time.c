
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Clocks are triggered time-ordered? */

void test_timeA (void *x)
{
    test_clocksCounter++;
}

void test_timeB (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    test_clocksFails |= (t < atomic_float64Read (&test_clocksTime));
    
    atomic_float64Write (&test_clocksTime, t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (ClocksTime, "Clocks - Time")

    if (test_clocksRun ((t_method)test_timeA, (t_method)test_timeB, 1)) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_clocksCounter == TEST_LOOP_CLOCKS * TEST_CLOCKS_SIZE);
        TTT_EXPECT (test_clocksFails   == 0);
        TTT_EXPECT (test_clocksCheck() == 1);
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
