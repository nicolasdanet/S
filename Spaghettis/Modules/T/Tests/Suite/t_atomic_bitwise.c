
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/mintomic/mintomic/tree/master/tests > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static uint32_t test_uInt32Masks[32];

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_bitwiseThread (void *x)
{
    int i, j, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    int start = (n + 0) * (32 / ttt_threadGetNumberOfThreads ((TTTThreadProperties *)x));
    int end   = (n + 1) * (32 / ttt_threadGetNumberOfThreads ((TTTThreadProperties *)x));
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
        
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
    //
    for (j = start; j < end; j++) {
        atomic_uInt32Set (&test_uInt32Shared, test_uInt32Masks[j]);
        // test_uInt32Shared |= test_uInt32Masks[j];
        ttt_wasteTime (&w);
    }
    for (j = start; j < end; j++) {
        if (atomic_uInt32False (&test_uInt32Shared, test_uInt32Masks[j])) { test_atomicFailed = 1; }
    }
    for (j = start; j < end; j++) {
        atomic_uInt32Unset (&test_uInt32Shared, test_uInt32Masks[j]);
        ttt_wasteTime (&w);
    }
    for (j = start; j < end; j++) {
        if (atomic_uInt32True (&test_uInt32Shared, test_uInt32Masks[j]))  { test_atomicFailed = 1; }
    }
    //
    }

    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicBitwise, "Atomic - Bitwise")

    t_rand48 seed; PD_RAND48_INIT (seed);

    int i;
        
    for (i = 0; i < 32; i++) { test_uInt32Masks[i] = (1U << i); }

    /* < https://en.wikipedia.org/wiki/Fisher-Yates_shuffle > */
    
    for (i = (32 - 1); i > 0; i--) {
        int rnd = (int)(PD_RAND48_DOUBLE (seed) * (i + 1));
        uint32_t t = test_uInt32Masks[rnd];
        test_uInt32Masks[rnd] = test_uInt32Masks[i];
        test_uInt32Masks[i]   = t;
    }

    if (ttt_testThreadsLaunch (test_bitwiseThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_atomicFailed == 0);
    }

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
