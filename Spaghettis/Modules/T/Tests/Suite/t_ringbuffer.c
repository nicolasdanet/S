
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_ringbuffer *test_ringbuffer;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_randMT *test_random0;
static t_randMT *test_random1;

static uint64_t test_value0;
static uint64_t test_value1;

static int test_counter0;
static int test_counter1;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int test_ringFailed;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEST_RINGBUFFER_CHANCE      11
#define TEST_RINGBUFFER_CHUNK       17
#define TEST_RINGBUFFER_MAXIMUM     21

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void test_ringWrite()
{
    if (ringbuffer_getAvailableWrite (test_ringbuffer) >= TEST_RINGBUFFER_CHUNK) {
    //
    int i; uint64_t data[TEST_RINGBUFFER_CHUNK] = { 0 };
    
    for (i = 0; i < TEST_RINGBUFFER_CHUNK; i++) { data[i] = PD_RAND48_NEXT (test_value0); }
    
    ringbuffer_write (test_ringbuffer, (const void *)data, TEST_RINGBUFFER_CHUNK);
    
    test_counter0++;
    //
    }
}

static void test_ringRead()
{
    if (ringbuffer_getAvailableRead (test_ringbuffer) >= TEST_RINGBUFFER_CHUNK) {
    //
    int i; uint64_t data[TEST_RINGBUFFER_CHUNK] = { 0 };
    
    ringbuffer_read (test_ringbuffer, (void *)data, TEST_RINGBUFFER_CHUNK);
    
    for (i = 0; i < TEST_RINGBUFFER_CHUNK; i++) {
        test_ringFailed += (data[i] != PD_RAND48_NEXT (test_value1));
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
        if (randMT_getInteger (test_random0, TEST_RINGBUFFER_CHANCE) == 0) { test_ringWrite(); }
        ttt_wasteTime (&w);
    }
    //
    }
    
    if (n == 1) {
    //
    for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        if (randMT_getInteger (test_random1, TEST_RINGBUFFER_CHANCE) == 0) { test_ringRead(); }
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
    int i;
    
    for (i = 0; i < TEST_RINGBUFFER_MAXIMUM; i++) {
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
        TTT_EXPECT (test_ringFailed == 0);
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
