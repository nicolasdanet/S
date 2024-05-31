
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/mintomic/mintomic/tree/master/tests > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_atomic.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_pointerThread (void *x)
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
        
        #if PD_32BIT
        atomic_pointerWrite (&test_pointerShared, (void *)test_uInt32Values[k]);
        #else
        atomic_pointerWrite (&test_pointerShared, (void *)test_uInt64Values[k]);
        #endif
        
        PD_MEMORY_BARRIER;      /* Prevent hoisting the store out of the loop. */
        
        ttt_wasteTime (&w);
        //
        }

    } else {
    
        for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        //
        PD_MEMORY_BARRIER;      /* Prevent hoisting the load out of the loop. */
        
        void *t = atomic_pointerRead (&test_pointerShared);
        
        #if PD_32BIT
        if (((uint32_t)t * (uint32_t)t) < test_uInt32Limit) { test_atomicFailed = 1; }
        #else
        if (((uint64_t)t * (uint64_t)t) < test_uInt64Limit) { test_atomicFailed = 1; }
        #endif
        
        ttt_wasteTime (&w);
        //
        }
    }

    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicPointer, "Atomic - Pointer")
    
    #if PD_32BIT
    atomic_pointerWrite (&test_pointerShared, (void *)test_uInt32Values[0]);
    #else
    atomic_pointerWrite (&test_pointerShared, (void *)test_uInt64Values[0]);
    #endif
    
    if (ttt_testThreadsLaunch (test_pointerThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_atomicFailed == 0);
    }

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
