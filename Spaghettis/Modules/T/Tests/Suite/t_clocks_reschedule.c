
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_rand48 test_clocksSeed;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Reschedule clocks in the main thread? */

void test_rescheduleTask (void *x)
{
    test_clocksSetBigRange ((t_clock *)x, PD_RAND48_DOUBLE (test_clocksSeed));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (ClocksReschedule, "Clocks - Reschedule")

    test_clocksSeed = PD_RAND48_SEED;
    
    if (test_clocksRun ((t_method)test_rescheduleTask, (t_method)test_timeTask, 1)) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_clocksFails == 0);
        TTT_EXPECT (test_clocksCheck == TEST_CLOCKS_SIZE);
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------