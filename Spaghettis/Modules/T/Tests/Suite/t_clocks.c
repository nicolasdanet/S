
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEST_CLOCKS_SIZE    32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_clock*             test_clocksA[TEST_CLOCKS_SIZE];     /* Main thread. */
static t_clock*             test_clocksB[TEST_CLOCKS_SIZE];

static t_int32Atomic        test_clocksStop;
static int                  test_clocksFails;

static t_float64Atomic      test_clocksTime;
static int                  test_clocksCounter;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksTaskCounter (void *x)
{
    test_clocksCounter++;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksSetSmallRange (t_clock *x, double f)
{
    clock_set (x, f * 500);
}

void test_clocksSetBigRange (t_clock *x, double f)
{
    clock_set (x, f * 1500);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksInitialize (t_method f, int safe)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
        test_clocksA[i] = clock_new ((void *)NULL, (t_method)test_clocksTaskCounter, safe);
        test_clocksB[i] = clock_new ((void *)NULL, f, 0);
    }
}

void test_clocksRelease (void)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
        clock_free (test_clocksA[i]);
        clock_free (test_clocksB[i]);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int test_clocksCheck (void)
{
    int i; for (i = 0; i < TEST_CLOCKS_SIZE; i++) { if (!clock_isGood (test_clocksA[i])) { return 0; } }
    
    return 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksTick (double f)
{
    atomic_float64Write (&test_clocksTime, 0); clocks_tick (instance_get()->pd_clocks, f);
}

void test_clocksDebug (int i)
{
    if (i % 1000 == 0) { ttt_stdout (TTT_COLOR_BLUE, "%s", clocks_debug (instance_get()->pd_clocks, 80)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksDoSomethingInMainThread (TTTThreadProperties *p, int j)
{
    test_clocksSetSmallRange (test_clocksA[j], ttt_getRandom (p));
}

void test_clocksDoSomethingConcurrently (TTTThreadProperties *p, int i, int j)
{
    if (i % 4 == j % 4) { test_clocksSetBigRange (test_clocksB[j], ttt_getRandom (p)); }
}

void *test_clocksTask (void *x)
{
    TTTThreadProperties *p = (TTTThreadProperties *)x;
    int n = ttt_threadGetCurrent (p);
    int i = 0;
    int j = 0;
    
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch (p);
    
    if (n == 0) {
    
        for (i = 0; i < TEST_LOOP_CLOCKS; i++) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomethingInMainThread (p, j);
                ttt_wasteTime (&w);
            }
                        
            test_clocksTick (250.0);
            test_clocksTick (750.0);
            
            /* All counted clocks are triggered. */
            
            // test_clocksDebug (i);
        }
        
        atomic_int32Write (&test_clocksStop, 1);
    }
    
    if (n == 1) {
        
        while (atomic_int32Read (&test_clocksStop) == 0) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomethingConcurrently (p, i, j);
                ttt_wasteTime (&w);
            }
            i++;
        }
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int test_clocksRun (t_method f, int safe)
{
    int err = 0;
    
    if (ttt_testGetNumberOfThreads() >= 2) {
    //
    test_clocksInitialize (f, safe);
    
    if (ttt_testThreadsLaunch (test_clocksTask) != TTT_GOOD) { err = 1; }
    else {
    //
    test_clocksTick (2000.0);   /* All remaining clocks fired. */
    //
    }
    
    test_clocksRelease();
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
