
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_ringbuffer *test_ringbuffer;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void test_ringWrite()
{
    if (ringbuffer_getAvailableWrite (test_ringbuffer) >= TEST_FIFO_CHUNK) {
    //
    int i; uint64_t data[TEST_FIFO_CHUNK] = { 0 };
    
    for (i = 0; i < TEST_FIFO_CHUNK; i++) { data[i] = PD_RAND48_NEXT (test_value0); }
    
    ringbuffer_write (test_ringbuffer, (const void *)data, TEST_FIFO_CHUNK);
    
    test_counter0++;
    //
    }
}

static void test_ringRead()
{
    if (ringbuffer_getAvailableRead (test_ringbuffer) >= TEST_FIFO_CHUNK) {
    //
    int i; uint64_t data[TEST_FIFO_CHUNK] = { 0 };
    
    ringbuffer_read (test_ringbuffer, (void *)data, TEST_FIFO_CHUNK);
    
    for (i = 0; i < TEST_FIFO_CHUNK; i++) {
        test_fifoFailed += (data[i] != PD_RAND48_NEXT (test_value1));
    }
    
    test_counter1++;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *test_ringThread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if (n == 0) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_random0, TEST_FIFO_CHANCE) == 0) { test_ringWrite(); }
        ttt_wasteTime (&w);
    }
    //
    }
    
    if (n == 1) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_random1, TEST_FIFO_CHANCE) == 0) { test_ringRead(); }
        ttt_wasteTime (&w);
    }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicRing, "Atomic - Ring")

    test_random0 = randMT_new();
    test_random1 = randMT_new();
    
    {
    //
    int i, k = 21;
    
    for (i = 0; i < k; i++) {
    //
    test_ringbuffer = ringbuffer_new (sizeof (uint64_t), (1 << i));
    
    test_value0     = PD_RAND48_SEED;
    test_value1     = test_value0;
    test_counter0   = 0;
    test_counter1   = 0;
    
    if (ttt_testThreadsLaunch (test_ringThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        // ttt_stdout (TTT_COLOR_BLUE, "W: %d", test_counter0);
        // ttt_stdout (TTT_COLOR_BLUE, "R: %d", test_counter1);
        TTT_EXPECT (test_fifoFailed == 0);
    }
    
    ringbuffer_free (test_ringbuffer);
    //
    }
    //
    }
    
    randMT_free (test_random1);
    randMT_free (test_random0);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
