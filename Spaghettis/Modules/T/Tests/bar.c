
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <unistd.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *threadedTest (void *x)
{
    TTTThreadProperties *p = (TTTThreadProperties *)x;
    
    int i = ttt_threadGetCurrent (p);
    int n = ttt_threadGetNumberOfThreads (p);
    
    ttt_threadWaitOnLatch (p);
    
    ttt_stdout (TTT_COLOR_NONE, "### Thread %d / %d", i, n);
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

TTT_BEGIN_ALLOW (FooThread, "Thread")                       /* Allowed to fail. */

ttt_testThreadsLaunch (threadedTest);                       /* Launch one thread per CPU. */
TTT_EXPECT (0 + 0 == 1);

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

TTT_BEGIN (FooTime, "Time")

ttt_timeTrigger();
ttt_timeSleep (250);
ttt_stdout (TTT_COLOR_BLUE, "%f", ttt_timeTrigger());       /* Measure time spent in milliseconds. */

TTT_END

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------