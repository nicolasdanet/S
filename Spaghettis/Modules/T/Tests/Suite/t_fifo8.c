
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_fifo8 *test_fifo8;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void test_fifo8Write()
{
    if (fifo8_getAvailableWrite (test_fifo8) >= TEST_FIFO_CHUNK) {
    //
    int i; uint8_t data[TEST_FIFO_CHUNK] = { 0 };
    
    for (i = 0; i < TEST_FIFO_CHUNK; i++) { data[i] = (uint8_t)PD_RAND48_NEXT (test_fifoValue0); }
    
    fifo8_write (test_fifo8, (const void *)data, TEST_FIFO_CHUNK);
    
    test_wCounterSucceed++;
    //
    } else { test_wCounterFailed++; }
}

static void test_fifo8Read()
{
    if (fifo8_getAvailableRead (test_fifo8) >= TEST_FIFO_CHUNK) {
    //
    int i; uint8_t data[TEST_FIFO_CHUNK] = { 0 };
    
    fifo8_read (test_fifo8, (void *)data, TEST_FIFO_CHUNK);
    
    for (i = 0; i < TEST_FIFO_CHUNK; i++) {
        test_fifoFailed += (data[i] != (uint8_t)PD_RAND48_NEXT (test_fifoValue1));
    }
    
    test_rCounterSucceed++;
    //
    } else { test_rCounterFailed++; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_fifo8Thread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);

    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if (n == 0) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_fifoRandom0, TEST_FIFO_CHANCE) == 0) { test_fifo8Write(); }
    }
    //
    }
    
    if (n == 1) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_fifoRandom1, TEST_FIFO_CHANCE) == 0) { test_fifo8Read(); }
    }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicFifo8, "Atomic - Fifo8")

    test_fifoRandom0 = randMT_new();
    test_fifoRandom1 = randMT_new();
    
    {
    //
    test_fifo8            = fifo8_new();
    
    test_fifoValue0       = PD_RAND48_SEED;
    test_fifoValue1       = test_fifoValue0;
    test_wCounterSucceed  = 0;
    test_wCounterFailed   = 0;
    test_rCounterSucceed  = 0;
    test_rCounterFailed   = 0;
    
    if (ttt_testThreadsLaunch (test_fifo8Thread) != TTT_GOOD) { TTT_FAIL; }
    else {
        // ttt_stdout (TTT_COLOR_BLUE, "W: %d / %d", test_wCounterSucceed, test_wCounterFailed);
        // ttt_stdout (TTT_COLOR_BLUE, "R: %d / %d", test_rCounterSucceed, test_rCounterFailed);
        TTT_EXPECT (test_fifoFailed == 0);
    }
    
    fifo8_free (test_fifo8);
    //
    }
    
    randMT_free (test_fifoRandom1);
    randMT_free (test_fifoRandom0);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
