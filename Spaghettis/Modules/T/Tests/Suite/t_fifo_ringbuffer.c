
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_ring *test_ring;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void test_ringWrite()
{
    if (ring_getAvailableWrite (test_ring) >= TEST_FIFO_CHUNK) {
    //
    int i; uint64_t data[TEST_FIFO_CHUNK] = { 0 };
    
    for (i = 0; i < TEST_FIFO_CHUNK; i++) { data[i] = PD_RAND48_NEXT (test_fifoValue0); }
    
    ring_write (test_ring, (const void *)data, TEST_FIFO_CHUNK);
    
    test_wCounterSucceed++;
    //
    }
}

static void test_ringRead()
{
    if (ring_getAvailableRead (test_ring) >= TEST_FIFO_CHUNK) {
    //
    int i; uint64_t data[TEST_FIFO_CHUNK] = { 0 };
    
    ring_read (test_ring, (void *)data, TEST_FIFO_CHUNK);
    
    for (i = 0; i < TEST_FIFO_CHUNK; i++) {
        test_fifoFailed += (data[i] != PD_RAND48_NEXT (test_fifoValue1));
    }
    
    test_rCounterSucceed++;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_ringThread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if (n == 0) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_fifoRandom0, TEST_FIFO_CHANCE) == 0) { test_ringWrite(); }
        ttt_wasteTime (&w);
    }
    //
    }
    
    if (n == 1) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_fifoRandom1, TEST_FIFO_CHANCE) == 0) { test_ringRead(); }
        ttt_wasteTime (&w);
    }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicRing, "Atomic - Ring")

    test_fifoRandom0 = randMT_new();
    test_fifoRandom1 = randMT_new();
    
    {
    //
    int i, k = 21;
    
    for (i = 0; i < k; i++) {
    //
    test_ring               = ring_new ((1 << i), sizeof (uint64_t));
    
    test_fifoValue0         = PD_RAND48_SEED;
    test_fifoValue1         = test_fifoValue0;
    test_wCounterSucceed    = 0;
    test_rCounterSucceed    = 0;
    
    if (ttt_testThreadsLaunch (test_ringThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        ttt_stdout (TTT_COLOR_BLUE, "W: %d", test_wCounterSucceed);
        ttt_stdout (TTT_COLOR_BLUE, "R: %d", test_rCounterSucceed);
        TTT_EXPECT (test_fifoFailed == 0);
    }
    
    ring_free (test_ring);
    //
    }
    //
    }
    
    randMT_free (test_fifoRandom1);
    randMT_free (test_fifoRandom0);
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
