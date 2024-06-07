
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEST_CLOCKS_SIZE    32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_clock*             test_clocks0[TEST_CLOCKS_SIZE];     /* Main thread. */
static t_clock*             test_clocks1[TEST_CLOCKS_SIZE];
static t_clock*             test_clocks2[TEST_CLOCKS_SIZE];
static t_clock*             test_clocks3[TEST_CLOCKS_SIZE];

static t_int32Atomic        test_clocksStop;
static int                  test_clocksFails;
static int                  test_clocksCheck;

static double               test_clocksTime;
static int                  test_clocksCounter;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_counterTask (void *x)
{
    test_clocksCounter++;
}

/* Clocks are triggered time-ordered? */

/* All tasks are handled in the main thread. */

void test_timeTask (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    if (t < test_clocksTime) { test_clocksFails = 1; }
    
    test_clocksTime = t;
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

void test_clocksInitialize (t_method f0, t_method f1, int safe)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
        test_clocks0[i] = clock_new ((void *)NULL, f0, safe);
        test_clocks1[i] = clock_new ((void *)NULL, f1, 0);
        test_clocks2[i] = clock_new ((void *)NULL, f1, 0);
        test_clocks3[i] = clock_new ((void *)NULL, f1, 0);
    }
}

void test_clocksRelease (void)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
    //
    if (clock_count (test_clocks0[i]) != 0) { test_clocksCheck++; }
    if (clock_count (test_clocks1[i]) != 0) { test_clocksCheck++; }
    if (clock_count (test_clocks2[i]) != 0) { test_clocksCheck++; }
    if (clock_count (test_clocks3[i]) != 0) { test_clocksCheck++; }
    //
    }
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
        clock_free (test_clocks0[i]);
        clock_free (test_clocks1[i]);
        clock_free (test_clocks2[i]);
        clock_free (test_clocks3[i]);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksTick (double f)
{
    test_clocksTime = 0; clocks_tick (instance_get()->pd_clocks, f);
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
    test_clocksSetSmallRange (test_clocks0[j], ttt_getRandom (p));
}

void test_clocksDoSomethingConcurrently (TTTThreadProperties *p, int n, int i, int j)
{
    t_clock *c = NULL;
    
    if (i % 4 == j % 4) {
    //
    if (n == 1)         { c = test_clocks1[j]; }
    else if (n == 2)    { c = test_clocks2[j]; }
    else                { c = test_clocks3[j]; }
    
    test_clocksSetBigRange (c, ttt_getRandom (p));
    //
    }
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
    
    if (n > 0 && n < 4) {
        
        while (atomic_int32Read (&test_clocksStop) == 0) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomethingConcurrently (p, n, i, j);
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

int test_clocksRun (t_method f0, t_method f1, int safe)
{
    int err = 0;
    
    if (ttt_testGetNumberOfThreads() >= 2) {
    //
    test_clocksInitialize (f0, f1, safe);
    
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
