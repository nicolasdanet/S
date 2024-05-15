
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_fifo32 *test_fifo32;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void test_fifo32Write()
{
    if (fifo32_getAvailableWrite (test_fifo32) >= TEST_FIFO_CHUNK) {
    //
    int i; uint32_t data[TEST_FIFO_CHUNK] = { 0 };
    
    for (i = 0; i < TEST_FIFO_CHUNK; i++) { data[i] = (uint32_t)PD_RAND48_NEXT (test_value0); }
    
    fifo32_write (test_fifo32, (const void *)data, TEST_FIFO_CHUNK);
    
    test_wCounterSucceed++;
    //
    } else { test_wCounterFailed++; }
}

static void test_fifo32Read()
{
    if (fifo32_getAvailableRead (test_fifo32) >= TEST_FIFO_CHUNK) {
    //
    int i; uint32_t data[TEST_FIFO_CHUNK] = { 0 };
    
    fifo32_read (test_fifo32, (void *)data, TEST_FIFO_CHUNK);
    
    for (i = 0; i < TEST_FIFO_CHUNK; i++) {
        test_fifoFailed += (data[i] != (uint32_t)PD_RAND48_NEXT (test_value1));
    }
    
    test_rCounterSucceed++;
    //
    } else { test_rCounterFailed++; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *test_fifo32Thread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);

    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if (n == 0) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_random0, TEST_FIFO_CHANCE) == 0) { test_fifo32Write(); }
    }
    //
    }
    
    if (n == 1) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_random1, TEST_FIFO_CHANCE) == 0) { test_fifo32Read(); }
    }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicFifo32, "Atomic - Fifo32")

    test_random0 = randMT_new();
    test_random1 = randMT_new();
    
    {
    //
    test_fifo32  = fifo32_new();
    
    test_value0           = PD_RAND48_SEED;
    test_value1           = test_value0;
    test_wCounterSucceed  = 0;
    test_wCounterFailed   = 0;
    test_rCounterSucceed  = 0;
    test_rCounterFailed   = 0;
    
    if (ttt_testThreadsLaunch (test_fifo32Thread) != TTT_GOOD) { TTT_FAIL; }
    else {
        // ttt_stdout (TTT_COLOR_BLUE, "W: %d / %d", test_wCounterSucceed, test_wCounterFailed);
        // ttt_stdout (TTT_COLOR_BLUE, "R: %d / %d", test_rCounterSucceed, test_rCounterFailed);
        TTT_EXPECT (test_fifoFailed == 0);
    }
    
    fifo32_free (test_fifo32);
    //
    }
    
    randMT_free (test_random1);
    randMT_free (test_random0);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
