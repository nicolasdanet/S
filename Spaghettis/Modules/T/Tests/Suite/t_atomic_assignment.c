
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://github.com/mintomic/mintomic/tree/master/tests > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static uint32_t test_uInt32Limit = 0xffffff00U;
static uint64_t test_uInt64Limit = 0xffffff0000000000ULL;

/* Satisfy (x * x >= test_uInt32Limit). */

static uint32_t test_uInt32Values[8] =
    {
        0x37b91364U,
        0x1c5970efU,
        0x536c76baU,
        0x0a10207fU,
        0x71043c77U,
        0x4db84a83U,
        0x27cf0273U,
        0x74a15a69U
    };

/* Satisfy (x * x >= test_uInt64Limit). */

static uint64_t test_uInt64Values[8] =
    {
        0x3c116d2362a21633ULL,
        0x7747508552ab6bc6ULL,
        0x289a0e1528a43422ULL,
        0x15e36d0a61d326eaULL,
        0x3ccb2e8c0c6224c4ULL,
        0x074504c13a1716e1ULL,
        0x6c82417a3ad77b24ULL,
        0x3124440040454919ULL
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void *test_loadStoreThread (void *x)
{
    int i, n = ttt_threadGetCurrent ((TTTThreadProperties *)x);
    TTTWaste w;
    
    ttt_wasteInit (&w, n);
    
    ttt_threadWaitOnLatch ((TTTThreadProperties *)x);
    
    if ((n % 2) == 0) {
    
        t_rand48 seed; PD_RAND48_INIT (seed);
        
        for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        //
        int k = (int)(PD_RAND48_DOUBLE (seed) * 8);
        
        atomic_uInt32Write        (&test_uInt32Shared, test_uInt32Values[k]);
        atomic_int32WriteRelaxed  (&test_int32Shared,  test_uInt32Values[k]);
        atomic_uInt64WriteRelaxed (&test_uInt64Shared, test_uInt64Values[k]);
        //test_uInt64Shared = test_uInt64Values[k];
        
        PD_MEMORY_BARRIER;      /* Prevent hoisting the store out of the loop. */
        
        ttt_wasteTime (&w);
        //
        }

    } else {
    
        for (i = 0; i < TEST_LOOP_ATOMIC; i++) {
        //
        PD_MEMORY_BARRIER;      /* Prevent hoisting the load out of the loop. */
        
        uint32_t a = atomic_uInt32Read        (&test_uInt32Shared);
        uint32_t b = atomic_int32ReadRelaxed  (&test_int32Shared);
        uint64_t c = atomic_uInt64ReadRelaxed (&test_uInt64Shared);
        
        if ((a * a) < test_uInt32Limit) { test_atomicFailed = 1; }
        if ((b * b) < test_uInt32Limit) { test_atomicFailed = 1; }
        if ((c * c) < test_uInt64Limit) { test_atomicFailed = 1; }
        
        ttt_wasteTime (&w);
        //
        }
    }

    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

TTT_BEGIN (AtomicAssignment, "Atomic - Assignment")

    test_int32Shared  = test_uInt32Values[0];
    test_uInt32Shared = test_uInt32Values[0];
    test_uInt64Shared = test_uInt64Values[0];

    if (ttt_testThreadsLaunch (test_loadStoreThread) != TTT_GOOD) { TTT_FAIL; }
    else {
        TTT_EXPECT (test_atomicFailed == 0);
    }

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
