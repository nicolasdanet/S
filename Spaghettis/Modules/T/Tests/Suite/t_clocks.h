
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEST_CLOCKS_SIZE    32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_clock              test_clocksA[TEST_CLOCKS_SIZE];
static t_float64Atomic      test_clocksTime;
static t_rand48             test_clocksSeed;

static t_clock              test_clocksB[TEST_CLOCKS_SIZE];
static int                  test_clocksCounter;

static t_int32Atomic        test_clocksStop;
static int                  test_clocksFails;

static t_clocks             *test_clocksManager;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Mimic the behavior of application clocks. */

void clock_unset_ (t_clock *x)
{
    clocks_remove (test_clocksManager, x);
}

void clock_set_ (t_clock *x, double f)
{
    clock_unset_ (x);
    
    atomic_float64Write (&x->c_systime, f);
    
    clocks_add (test_clocksManager, x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Clocks are triggered time-ordered? */

void test_taskCheckTime (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    test_clocksFails |= (t < atomic_float64Read (&test_clocksTime));
    
    atomic_float64Write (&test_clocksTime, t);
    
    clock_set_ ((t_clock *)x, PD_RAND48_DOUBLE (test_clocksSeed) * 1000);
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
    
    test_clocksManager = clocks_new();

    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
    //
    {
        t_clock *c = &test_clocksA[i];
        c->c_fn    = test_taskCheckTime;
        c->c_owner = c;
    }
    {
        t_clock *c = &test_clocksB[i];
        c->c_fn    = test_taskCheckCount;
        c->c_owner = c;
    }
    //
    }
    
    test_clocksSeed = PD_RAND48_SEED;
}

int test_clocksCheck (void)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
    //
    if (!clock_isGood (&test_clocksB[i])) { return 0; }
    //
    }
    
    return 1;
}

void test_clocksRelease (void)
{
    clocks_free (test_clocksManager);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksDoSomethingRandomly (TTTThreadProperties *p, int j)
{
    int i = ttt_getRandomInteger (p, TEST_CLOCKS_SIZE);
    
    t_clock *c = &test_clocksA[i];
    
    if (ttt_getRandomInteger (p, 2)) {
        clock_set_ (c, ttt_getRandomInteger (p, 1000));
    } else {
        clock_unset_ (c);
    }
}

void test_clocksDoSomethingCounted (TTTThreadProperties *p, int j)
{
    clock_set_ (&test_clocksB[j], ttt_getRandomInteger (p, 500));
}

void test_clocksTick (double f)
{
    atomic_float64Write (&test_clocksTime, 0); clocks_tick (test_clocksManager, f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------