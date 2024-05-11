
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_clocks.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *test_clocksAtomicTask (void *x)
{
    int i, j, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if (n == 0) {
        
        while (atomic_int32Read (&test_clocksStop) == 0) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomethingConcurrently (j);
                ttt_wasteTime (&w);
            }
        }
    }
    
    if (n == 1) {
    
        for (i = 0; i < TEST_LOOP_CLOCKS; i++) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomething (j);
                ttt_wasteTime (&w);
            }
            
            tests_clocksTick (250.0);
            tests_clocksTick (750.0);
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
    tests_clocksTick (1000.0);
    
    TTT_EXPECT (test_clocksCounter == TEST_CLOCKS_SIZE * TEST_LOOP_CLOCKS);
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
