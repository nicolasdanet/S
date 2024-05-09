
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "t_randomMT64.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define FIRST_FLOATS_IN_STATIC_ARRAY   10

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double test_floatGet (void)
{
    static uint64_t i = 0; return test_genrand64_real2[(++i) % FIRST_FLOATS_IN_STATIC_ARRAY];
}

int test_floatIsValid (double f)
{
    int i;
    
    for (i = 0; i < FIRST_FLOATS_IN_STATIC_ARRAY; i++) {
        if (f == test_genrand64_real2[i]) {
            return 1;
        }
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_floatThread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if ((n % 2) == 0) {
        
        for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        //
        double f = test_floatGet();
        
        atomic_float64WriteRelaxed (&test_float64Shared, f);
        // test_float64Shared = f;
        
        PD_MEMORY_BARRIER;      /* Prevent hoisting the store out of the loop. */
        
        ttt_wasteTime (&w);
        //
        }

    } else {
    
        for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        //
        PD_MEMORY_BARRIER;      /* Prevent hoisting the load out of the loop. */
        
        double f = atomic_float64ReadRelaxed (&test_float64Shared);
        // double f = test_float64Shared;
        
        if (!test_floatIsValid (f)) { test_atomicFailed = 1; }
        
        ttt_wasteTime (&w);
        //
        }
    }

    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicFloat, "Atomic - Float")

    TTT_EXPECT (sizeof (t_float64Atomic) == 8);
    TTT_EXPECT (sizeof (double) == sizeof (uint64_t));
    TTT_EXPECT (sizeof (double) == sizeof (t_float64Atomic));
    
    atomic_float64WriteRelaxed (&test_float64Shared, test_floatGet());
    
    if (ttt_testThreadsLaunch (test_floatThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_atomicFailed == 0);
    }

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
