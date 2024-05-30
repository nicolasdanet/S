
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef TEST_CLOCKS_H_
#define TEST_CLOCKS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEST_CLOCKS_SIZE    32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_clock*             test_clocksA[TEST_CLOCKS_SIZE];
static t_clock*             test_clocksB[TEST_CLOCKS_SIZE];

static t_int32Atomic        test_clocksStop;
static int                  test_clocksFails;
static t_rand48             test_clocksSeed;

static t_float64Atomic      test_clocksTime;
static int                  test_clocksCounter;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksInitialize (t_method taskA, t_method taskB, int safe)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
        test_clocksA[i] = clock_new ((void *)NULL, taskA, 0);
        test_clocksB[i] = clock_new ((void *)NULL, taskB, safe);
    }
    
    test_clocksSeed = PD_RAND48_SEED;
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
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
        if (!clock_isGood (test_clocksB[i])) { return 0; }
    }
    
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

void test_clocksDoSomethingConcurrently (TTTThreadProperties *p, int j)
{
    int i = ttt_getRandomInteger (p, TEST_CLOCKS_SIZE);
    
    if (ttt_getRandomInteger (p, 2)) {
        clock_set (test_clocksA[i], ttt_getRandomInteger (p, 1500));
    } else {
        clock_unset (test_clocksA[i]);
    }
}

void test_clocksDoSomethingInMainThread (TTTThreadProperties *p, int j)
{
    clock_set (test_clocksB[j], ttt_getRandomInteger (p, 500));
}

void *test_clocksTask (void *x)
{
    TTTThreadProperties *p = (TTTThreadProperties *)x;
    int i, j, n = ttt_threadGetCurrent (p);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch (p);
    
    if (n == 0) {
        
        while (atomic_int32Read (&test_clocksStop) == 0) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomethingConcurrently (p, j);
                ttt_wasteTime (&w);
            }
        }
    }
    
    if (n == 1) {
    
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
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // TEST_CLOCKS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
