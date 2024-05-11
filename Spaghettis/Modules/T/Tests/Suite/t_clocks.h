
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define TEST_CLOCKS_SIZE    32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_clock              test_clocksA[TEST_CLOCKS_SIZE];
static t_float64Atomic      test_clocksTime;

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

void clock_set_ (t_clock *x, double delay)
{
    clock_unset_ (x);
    
    atomic_float64Write (&x->c_systime, delay);
    
    clocks_add (test_clocksManager, x);
}

void clocks_tick_ (double f)
{
    atomic_float64Write (&test_clocksTime, 0);
    
    clocks_tick (test_clocksManager, f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksTaskCheckTime (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    test_clocksFails |= (t < atomic_float64Read (&test_clocksTime));
    
    atomic_float64Write (&test_clocksTime, t);
}

void test_clocksTaskCheckDone (void *x)
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
    t_clock *c = NULL;
    
    if (i < TEST_CLOCKS_SIZE) {
        c = &test_clocksA[i]; c->c_fn = test_clocksTaskCheckTime;
        c = &test_clocksB[i]; c->c_fn = test_clocksTaskCheckDone;
    }
    //
    }
}

int test_clocksCheck (void)
{
    int i;
    
    for (i = 0; i < TEST_CLOCKS_SIZE; i++) {
    //
    t_clock *c = NULL;
    
    if (i < TEST_CLOCKS_SIZE) {
        c = &test_clocksA[i]; if (!clock_isGood (c)) { return 0; }
        c = &test_clocksB[i]; if (!clock_isGood (c)) { return 0; }
    }

        c = &test_clocksB[i]; if (!clock_isGood (c)) { return 0; }
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

void test_clocksDoSomething (t_clock *x, double delay)
{
    if (test_random (2)) { clock_set_ (x, delay); } else { clock_unset_ (x); }
}

t_clock *test_clocksGetRandomA (void)
{
    int i = test_random (TEST_CLOCKS_SIZE); return &test_clocksA[i];
}

t_clock *test_clocksGetB (int i)
{
    return &test_clocksB[i];
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
