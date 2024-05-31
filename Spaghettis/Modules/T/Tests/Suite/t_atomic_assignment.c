
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/mintomic/mintomic/tree/master/tests > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_atomic.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_loadStoreThread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if ((n % 2) == 0) {
    
        t_rand48 seed; PD_RAND48_INIT (seed);
        
        for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        //
        int k = (int)(PD_RAND48_DOUBLE (seed) * 8);
        
        atomic_uInt32Write        (&test_uInt32Shared, test_uInt32Values[k]);
        atomic_int32WriteRelaxed  (&test_int32Shared,  test_uInt32Values[k]);
        atomic_uInt64WriteRelaxed (&test_uInt64Shared, test_uInt64Values[k]);
        //test_uInt64Shared = test_uInt64Values[k];
        
        PD_MEMORY_BARRIER;      /* Prevent hoisting the store out of the loop. */
        
        ttt_wasteTime (&w);
        //
        }

    } else {
    
        for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        //
        PD_MEMORY_BARRIER;      /* Prevent hoisting the load out of the loop. */
        
        uint32_t a = atomic_uInt32Read        (&test_uInt32Shared);
        uint32_t b = atomic_int32ReadRelaxed  (&test_int32Shared);
        uint64_t c = atomic_uInt64ReadRelaxed (&test_uInt64Shared);
        
        if ((a * a) < test_uInt32Limit) { test_atomicFailed = 1; }
        if ((b * b) < test_uInt32Limit) { test_atomicFailed = 1; }
        if ((c * c) < test_uInt64Limit) { test_atomicFailed = 1; }
        
        ttt_wasteTime (&w);
        //
        }
    }

    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicAssignment, "Atomic - Assignment")

    test_int32Shared  = test_uInt32Values[0];
    test_uInt32Shared = test_uInt32Values[0];
    test_uInt64Shared = test_uInt64Values[0];

    if (ttt_testThreadsLaunch (test_loadStoreThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_atomicFailed == 0);
    }

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
