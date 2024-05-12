
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_clocks.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_clocksAtomicTask (void *x)
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
                test_clocksDoSomethingCounted (p, j);
                ttt_wasteTime (&w);
            }
            
            test_clocksTick (250.0);
            test_clocksTick (750.0);        /* All clocks are triggered. */
            
            // if (i % 1000 == 0) { test_clocksDebug (128); }
        }
        
        atomic_int32Write (&test_clocksStop, 1);
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (ClocksAtomic, "Atomic - Clocks")

    if (ttt_testGetNumberOfThreads() >= 2) {
    //
    test_clocksInitialize();
    
    if (ttt_testThreadsLaunch (test_clocksAtomicTask) != TTT_GOOD) { TTT_FAIL; }
    else {
    //
    test_clocksTick (1000.0);
    
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
