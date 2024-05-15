
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
    
    test_counter0++;
    //
    }
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
    
    test_counter1++;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *test_fifo32Thread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if (n == 0) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_random0, TEST_FIFO_CHANCE) == 0) { test_fifo32Write(); }
        ttt_wasteTime (&w);
    }
    //
    }
    
    if (n == 1) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_random1, TEST_FIFO_CHANCE) == 0) { test_fifo32Read(); }
        ttt_wasteTime (&w);
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
    test_fifo32     = fifo32_new();
    
    test_value0     = PD_RAND48_SEED;
    test_value1     = test_value0;
    test_counter0   = 0;
    test_counter1   = 0;
    
    if (ttt_testThreadsLaunch (test_fifo32Thread) != TTT_GOOD) { TTT_FAIL; }
    else {
        // ttt_stdout (TTT_COLOR_BLUE, "W: %d", test_counter0);
        // ttt_stdout (TTT_COLOR_BLUE, "R: %d", test_counter1);
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
