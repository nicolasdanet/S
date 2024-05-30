
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_clocks_mixed.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_clocksMixedTask (void *x)
{
    TTTThreadProperties *p = (TTTThreadProperties *)x;
    int i, j, n = ttt_threadGetCurrent (p);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch (p);
    
    if (n == 0) {
        
        while (atomic_int32Read (&test_clocksStop) == 0) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomethingRandomly (p, j);
                ttt_wasteTime (&w);
            }
        }
    }
    
    if (n == 1) {
    
        for (i = 0; i < TEST_LOOP_CLOCKS; i++) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomething (p, j);
                ttt_wasteTime (&w);
            }
            
            test_clocksTick (250.0);
            test_clocksTick (750.0);
            
            /* All counted clocks are triggered. */
            
            test_clocksDebug (i);
        }
        
        atomic_int32Write (&test_clocksStop, 1);
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (ClocksMixed, "Clocks - Mixed")

    if (ttt_testGetNumberOfThreads() >= 2) {
    //
    test_clocksInitialize ((t_method)test_taskReschedule, (t_method)test_taskCount, 0);
    
    if (ttt_testThreadsLaunch (test_clocksMixedTask) != TTT_GOOD) { TTT_FAIL; }
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
