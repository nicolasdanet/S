
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEST_CLOCKS_SIZE    32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_clock*             test_clocksA[TEST_CLOCKS_SIZE];
static t_float64Atomic      test_clocksTime;

static t_clock*             test_clocksB[TEST_CLOCKS_SIZE];
static int                  test_clocksCounter;

static t_int32Atomic        test_clocksStop;
static int                  test_clocksFails;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Clocks are triggered time-ordered? */

void test_taskCheckTime (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    test_clocksFails |= (t < atomic_float64Read (&test_clocksTime));
    
    atomic_float64Write (&test_clocksTime, t);
}

void test_taskCheckCount (void *x)
{
    test_clocksCounter++;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksInitialize (void)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
    //
    test_clocksA[i] = clock_newSafe ((void *)NULL, (t_method)test_taskCheckTime);
    test_clocksB[i] = clock_newSafe ((void *)NULL, (t_method)test_taskCheckCount);
    //
    }
}

void test_clocksRelease (void)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
    //
    clock_free (test_clocksA[i]);
    clock_free (test_clocksB[i]);
    //
    }
}

int test_clocksCheck (void)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
    //
    if (!clock_isGood (test_clocksB[i])) { return 0; }
    //
    }
    
    return 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksDoSomethingRandomly (TTTThreadProperties *p, int j)
{
    int i = ttt_getRandomInteger (p, TEST_CLOCKS_SIZE);
    
    t_clock *c = test_clocksA[i];
    
    if (ttt_getRandomInteger (p, 2)) {
        clock_set (c, ttt_getRandomInteger (p, 1000));
    } else {
        clock_unset (c);
    }
}

void test_clocksDoSomethingCounted (TTTThreadProperties *p, int j)
{
    clock_set (test_clocksB[j], ttt_getRandomInteger (p, 500));
}

void test_clocksTick (double f)
{
    atomic_float64Write (&test_clocksTime, 0); clocks_tick (instance_get()->pd_clocks, f);
}

void test_clocksDebug (void)
{
    static int i = 0;
    
    if (i++ % 1000 == 0) { ttt_stdout (TTT_COLOR_BLUE, "%s", clocks_debug (instance_get()->pd_clocks, 80)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
