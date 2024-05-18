
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_fifo32 *test_fifo;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void test_fifoWrite()
{
    int i, k = randMT_getInteger (test_fifoRandom0, TEST_FIFO_CHUNK);
    
    uint32_t data[TEST_FIFO_CHUNK] = { 0 };
    
    for (i = 0; i < k; i++) { data[i] = (uint32_t)PD_RAND48_NEXT (test_fifoValue0); }
    
    uint32_t *p = data;
    
    while (k > 0) {
    //
    int writted = fifo32_write (test_fifo, (const void *)p, k);
    
    k -= writted;
    p += writted;
    
    test_wCounterSucceed += writted;
    //
    }
}

static void test_fifoRead()
{
    int i, k = randMT_getInteger (test_fifoRandom1, TEST_FIFO_CHUNK);
    
    uint32_t data[TEST_FIFO_CHUNK] = { 0 };
    
    int readed = fifo32_read (test_fifo, (void *)data, k);
    
    test_rCounterSucceed += readed;
    
    for (i = 0; i < readed; i++) {
        test_fifoFailed += (data[i] != (uint32_t)PD_RAND48_NEXT (test_fifoValue1));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_fifoThread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);

    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if (n == 0) {
        for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
            test_fifoWrite();
        }
        
        atomic_int32WriteRelaxed (&test_fifoStop, 1);
    }
    
    if (n == 1) {
        while (atomic_int32ReadRelaxed (&test_fifoStop) == 0) { test_fifoRead();  }
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicFifoHeavy, "Atomic - Heavy")

    test_fifoRandom0 = randMT_new();
    test_fifoRandom1 = randMT_new();
    
    {
    //
    test_fifo             = fifo32_new();
    
    test_fifoValue0       = PD_RAND48_SEED;
    test_fifoValue1       = test_fifoValue0;
    test_wCounterSucceed  = 0;
    test_wCounterFailed   = 0;
    test_rCounterSucceed  = 0;
    test_rCounterFailed   = 0;
    
    atomic_int32WriteRelaxed (&test_fifoStop, 0);
    
    if (ttt_testThreadsLaunch (test_fifoThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        // ttt_stdout (TTT_COLOR_BLUE, "W: %d", test_wCounterSucceed);
        // ttt_stdout (TTT_COLOR_BLUE, "R: %d", test_rCounterSucceed);
        TTT_EXPECT (test_fifoFailed == 0);
    }
    
    fifo32_free (test_fifo);
    //
    }
    
    randMT_free (test_fifoRandom1);
    randMT_free (test_fifoRandom0);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
