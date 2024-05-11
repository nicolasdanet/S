
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TEST_CLOCKS_SIZE    32
#define TEST_CLOCKS_LESS    2

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_int32Atomic        test_clocksStop;
static int                  test_clocksCounterB;
static t_float64Atomic      test_clocksSystime;
static int                  test_clocksFails;

static t_clock              test_clocksA[TEST_CLOCKS_SIZE];
static t_clock              test_clocksB[TEST_CLOCKS_SIZE];

static t_clocks             *test_clocksManager;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int test_clocksRandom (int n)
{
    static t_rand48 seed; static int once = 0; if (!once) { PD_RAND48_INIT (seed); once = 1; }
    
    int k = (int)(PD_RAND48_DOUBLE (seed) * n);
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Mimic the behavior of application clocks. */

void test_clocksUnset (t_clock *x)
{
    clocks_remove (test_clocksManager, x);
}

void test_clocksDelay (t_clock *x, double delay)
{
    test_clocksUnset (x);
    
    atomic_float64Write (&x->c_systime, delay);
    
    clocks_add (test_clocksManager, x);
}

void test_clocksTick (t_systime t)
{
    clocks_tick (test_clocksManager, t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void test_clocksTaskA (void *x)
{
    t_systime t = scheduler_getLogicalTime();
    
    test_clocksFails |= (t < atomic_float64Read (&test_clocksSystime));
    
    atomic_float64Write (&test_clocksSystime, t);
}

void test_clocksTaskB (void *x)
{
    test_clocksCounterB++;
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
        c = &test_clocksA[i]; c->c_fn = test_clocksTaskA;
        c = &test_clocksB[i]; c->c_fn = test_clocksTaskB;
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
    if (test_clocksRandom (2)) { test_clocksDelay (x, delay); } else { test_clocksUnset (x); }
}

t_clock *test_clocksGetRandomA (void)
{
    int i = test_clocksRandom (TEST_CLOCKS_SIZE); return &test_clocksA[i];
}

t_clock *test_clocksGetB (int i)
{
    return &test_clocksB[i];
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *test_clocksAtomicTask (void *x)
{
    int i, j, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if (n == 0) {
        
        while (atomic_int32Read (&test_clocksStop) == 0) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDoSomething (test_clocksGetRandomA(), test_clocksRandom (1000));
                ttt_wasteTime (&w);
            }
        }
    }
    
    if (n == 1) {
    
        for (i = 0; i < TEST_LOOP_CLOCKS; i++) {
            for (j = 0; j < TEST_CLOCKS_SIZE; j++) {
                test_clocksDelay (test_clocksGetB (j), test_clocksRandom (500));
                ttt_wasteTime (&w);
            }
            
            atomic_float64Write (&test_clocksSystime, 0); test_clocksTick (250.0);
            atomic_float64Write (&test_clocksSystime, 0); test_clocksTick (750.0);
        }
        
        atomic_int32Write (&test_clocksStop, 1);
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (ClocksAtomic, "Atomic - Clocks")

    if (ttt_testGetNumberOfThreads() >= 2) {
    //
    test_clocksInitialize();
    
    if (ttt_testThreadsLaunch (test_clocksAtomicTask) != TTT_GOOD) { TTT_FAIL; }
    else {
    //
    atomic_float64Write (&test_clocksSystime, 0); test_clocksTick (1000.0);
    
    TTT_EXPECT (test_clocksCounterB == TEST_CLOCKS_SIZE * TEST_LOOP_CLOCKS);
    TTT_EXPECT (test_clocksFails    == 0);
    TTT_EXPECT (test_clocksCheck()  == 1);
    //
    }
    
    test_clocksRelease();
    //
    }
    
TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
