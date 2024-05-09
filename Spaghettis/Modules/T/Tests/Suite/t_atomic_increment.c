
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/mintomic/mintomic/tree/master/tests > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_incrementThread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if ((n % 2) == 0) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        atomic_int32Increment (&test_int32Shared);
        //test_int32Shared++;
        ttt_wasteTime (&w);
    }
    //
    } else {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        atomic_int32Decrement (&test_int32Shared);
        //test_int32Shared--;
        ttt_wasteTime (&w);
    }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicIncrement, "Atomic - Increment")

    if (ttt_testThreadsLaunch (test_incrementThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        int t = atomic_int32Read (&test_int32Shared); TTT_EXPECT (t == 0);
    }

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
