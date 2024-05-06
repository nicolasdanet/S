
/* Copyright (c) 2017 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "T.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if TTT_APPLE
    #include <mach/mach.h>
    #include <mach/mach_time.h>
    #include <sys/sysctl.h>
#endif

#if TTT_LINUX
    #include <sched.h>
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _TTTTestElement {
    TTTFnTest       method_;
    const char      *comment_;
    } TTTTestElement;

typedef struct _TTTTest {
    int             id_;
    int             size_;
    int             index_;
    TTTTestElement  *tests_;
    int             failed_;
    int             allow_;
    TTTError        error_;
    } TTTTest;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TTT_MAXIMUM_THREADS         32              /* Power of two. */
#define TTT_MAXIMUM_SIZE            1024

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __cplusplus )
    #include <atomic>
#else
    #include <stdatomic.h>
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef int32_t __attribute__ ((__aligned__ (4))) TTTLatch;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ttt_latchSet (TTTLatch *p, int n)
{
    __atomic_store_n (p, n, __ATOMIC_SEQ_CST);
}

void ttt_latchDecrement (TTTLatch *p)
{
    __atomic_sub_fetch (p, 1, __ATOMIC_SEQ_CST);
}

int ttt_latchGetCount (TTTLatch *p)
{
    return __atomic_load_n (p, __ATOMIC_SEQ_CST);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _TTTThreadProperties {
    int         current_;
    int         threads_;
    TTTLatch    *latch_;
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

TTTError    ttt_unitError;
int         ttt_unitAllowed;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static TTTTestElement       ttt_testElements[TTT_MAXIMUM_SIZE];
static int                  ttt_testSize;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static TTTLatch             ttt_latch;
static TTTThreadProperties  ttt_testProperties[TTT_MAXIMUM_THREADS];

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define TTT_SYSTEM_ERROR    ttt_stderr (TTT_COLOR_RED, ">>> Error / %s / line %d", TTT_SHORT_FILE, __LINE__)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ttt_threadSetProperties (TTTThreadProperties *p, int i, int n)
{
    ttt_latchSet (&ttt_latch, n);
    
    p->current_ = i;
    p->threads_ = n;
    p->latch_   = &ttt_latch;
}

int ttt_threadGetCurrent (TTTThreadProperties *p)
{
    return p->current_;
}

int ttt_threadGetNumberOfThreads (TTTThreadProperties *p)
{
    return p->threads_;
}

void ttt_threadWaitOnLatch (TTTThreadProperties *p)
{
    ttt_latchDecrement (p->latch_);
    
    while (ttt_latchGetCount (p->latch_) > 0) { ttt_timeSleep (0.01); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static uint64_t ttt_timeGet (void)
{
    struct timespec time;
    
    clock_gettime (CLOCK_MONOTONIC, &time);
    
    uint64_t seconds     = (uint64_t)time.tv_sec;
    uint64_t nanoseconds = (uint64_t)time.tv_nsec;
    
    return (seconds * 1000000000ULL) + nanoseconds;
}

static double ttt_timeElapsed (uint64_t t0, uint64_t t1)
{
    uint64_t elapsed = 0ULL;
    
    if (t1 > t0) { elapsed = t1 - t0; }
    
    return ((double)(elapsed) * 1e-6);
}

double ttt_timeTrigger (void)
{
    static int once = 0;
    static uint64_t start, now;
    
    double ms = 0.0;
    
    __sync_synchronize();
    
    if (!once) { start = ttt_timeGet(); once = 1; }
    else {
        now = ttt_timeGet();
        ms  = ttt_timeElapsed (start, now);
        start = now;
    }
    
    __sync_synchronize();
    
    return ms;
}

void ttt_timeSleep (double ms)
{
    uint64_t ns = (uint64_t)(TTT_MAX (0.0, ms) * 1000000.0);
        
    if (ns != 0ULL) {
    //
    struct timespec t0, t1;
    struct timespec *ptrA = &t0;
    struct timespec *ptrB = &t1;
    struct timespec *temp = NULL;

    uint64_t nsec_per_sec = 1000000000ULL;
    
    t0.tv_sec  = (time_t)(ns / nsec_per_sec);
    t0.tv_nsec = (long)(ns % nsec_per_sec);

    while ((nanosleep (ptrA, ptrB) == -1) && (errno == EINTR)) {
        temp = ptrA;
        ptrA = ptrB;
        ptrB = temp;
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://en.wikipedia.org/wiki/ANSI_color > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static TTTError ttt_stdcolor (int color, FILE *stream)
{
    TTTError err = TTT_GOOD;
    
    switch (color) {
        case TTT_COLOR_RED      : err = (fputs ("\033[22;31m", stream) < 0); break;
        case TTT_COLOR_GREEN    : err = (fputs ("\033[22;32m", stream) < 0); break;
        case TTT_COLOR_BLUE     : err = (fputs ("\033[22;34m", stream) < 0); break;
        default                 : break;
    }
    
    return err;
}

TTTError ttt_stdout (int color, const char *format, ...)
{
    va_list args;
    TTTError err = ttt_stdcolor (color, stdout);

    va_start (args, format);
    err |= (vfprintf (stdout, format, args) < 0);
    va_end (args);

    if (color) { err |= (fputs ("\033[0m", stdout) < 0); }
    err |= (fputs ("\n", stdout) < 0);

    return err;
}

TTTError ttt_stderr (int color, const char *format, ...)
{
    va_list args;
    TTTError err = ttt_stdcolor (color, stderr);

    va_start (args, format);
    err |= (vfprintf (stderr, format, args) < 0);
    va_end (args);

    if (color) { err |= (fputs ("\033[0m", stderr) < 0); }
    err |= (fputs ("\n", stderr) < 0);

    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static uint32_t ttt_previousPowerOfTwo (uint32_t v)
{
    v |= (v >> 1);
    v |= (v >> 2);
    v |= (v >> 4);
    v |= (v >> 8);
    v |= (v >> 16);
    
    return v - (v >> 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if TTT_APPLE

static TTTError ttt_systemGetCPUsNumber (int *cpus)
{
    static int n = 0;
    size_t size = sizeof (int);
    
    if (n == 0) {
        if (sysctlbyname ("machdep.cpu.core_count", &n, &size, NULL, 0L) != 0) { n = 0; TTT_SYSTEM_ERROR; }
    }
    
    if (n != 0) { (*cpus) = n; return TTT_GOOD; }
    
    (*cpus) = 1;
    
    return TTT_ERROR;
}

#endif // TTT_APPLE

#if TTT_LINUX

static TTTError ttt_systemGetCPUsNumber (int *cpus)
{
    static int n = 0;
    
    if (n == 0) { n = sysconf (_SC_NPROCESSORS_ONLN); }
    if (n != 0) { (*cpus) = n; return TTT_GOOD; }
    
    (*cpus) = 1;
    
    return TTT_ERROR;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int ttt_testGetNumberOfThreads (void)            /* The (power of 2) number of cpus or 2 if less. */
{
    static int ttt_testThreads = 0;
    
    if (!ttt_testThreads) {
    //
    int cpus = 0;
    if (ttt_systemGetCPUsNumber (&cpus)) { ttt_testThreads = 2; }
    else {
        ttt_testThreads = (int)ttt_previousPowerOfTwo ((uint32_t)cpus);
        ttt_testThreads = TTT_MIN (ttt_testThreads, TTT_MAXIMUM_THREADS);
        ttt_testThreads = TTT_MAX (2, ttt_testThreads);
    }
    //
    }
    
    return ttt_testThreads;
}

TTTError ttt_testThreadsLaunch (TTTFnTestThread test)
{
    TTTError err = TTT_GOOD;
    
    pthread_attr_t attr;
    err |= (pthread_attr_init (&attr) != 0);
    
    if (!err) {
    //
    pthread_t threads[TTT_MAXIMUM_THREADS];
    TTTError  isError[TTT_MAXIMUM_THREADS];
    
    int i, cpus, n = ttt_testGetNumberOfThreads();
    
    pthread_attr_setscope (&attr, PTHREAD_SCOPE_PROCESS);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedpolicy (&attr, SCHED_OTHER);
    
    ttt_systemGetCPUsNumber (&cpus);
    
    for (i = 0; i < n; i++) { ttt_threadSetProperties (ttt_testProperties + i, i, n); }
    
    for (i = 0; i < n; i++) {
    //
    err |= (isError[i] = (pthread_create (threads + i, &attr, test, (void *)(ttt_testProperties + i)) != 0));
    
    /* Should place one thread per core. */
    
    if ((n == cpus) && (isError[i] == TTT_GOOD)) {
    //
    #if TTT_APPLE
    mach_port_t mach = pthread_mach_thread_np (threads[i]);
    thread_affinity_policy_data_t policy = { (integer_t)i };
    thread_policy_set (mach, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, THREAD_AFFINITY_POLICY_COUNT);
    #endif // TTT_APPLE
    #if TTT_LINUX
    cpu_set_t set;
    CPU_ZERO (&set);
    CPU_SET (i, &set);
    pthread_setaffinity_np (threads[i], sizeof (cpu_set_t), &set);
    #endif // TTT_LINUX
    }
    //
    }
    
    for (i = 0; i < n; i++) {
        if (isError[i] == TTT_GOOD) {
            err |= (pthread_join (threads[i], NULL) != 0);
        }
    }
    
    pthread_attr_destroy (&attr);
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static TTTTest *ttt_testNew (int argc, TTTTestElement *argv)
{
    int id;
    void *shared = NULL;
    TTTTest *x = NULL;

    if ((id = shmget (IPC_PRIVATE, sizeof (TTTTest), SHM_R | SHM_W)) == -1) { 
        TTT_SYSTEM_ERROR;
    } else if ((shared = shmat (id, NULL, 0)) == (void *)-1) { 
        TTT_SYSTEM_ERROR; if (shmctl (id, IPC_RMID, NULL) == -1) { TTT_SYSTEM_ERROR; } 
    } else {
    //
    x = (TTTTest *)shared;
    
    x->id_    = id;
    x->size_  = argc;
    x->index_ = 0;
    x->tests_ = argv;
    //
    }
    
    return x;
}

static void ttt_testFree (TTTTest *x)
{
    if (x) {
        int id = x->id_;
        if (shmdt (x) == -1) { TTT_SYSTEM_ERROR; }
        if (shmctl (id, IPC_RMID, NULL) == -1) { TTT_SYSTEM_ERROR; }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static TTTFnTest ttt_testPerformRunStart (TTTTest *x)
{
    int m = x->index_;
    int n = x->size_;
    
    x->failed_ = 1;
    x->allow_  = 0;
    x->error_  = TTT_ERROR;
    
    if (x->tests_[m].comment_) {
        ttt_stdout (TTT_COLOR_NONE, "### %03ld / %ld / %s",  m + 1, n, x->tests_[m].comment_);
    } else {
        ttt_stdout (TTT_COLOR_NONE, "### %03ld / %ld", m + 1, n);
    }
    
    return x->tests_[m].method_;
}

static TTTError ttt_testPerformRunEnd (TTTTest *x)
{
    if (x->failed_ || x->error_) {
    //
    int m = x->index_;
    int n = x->size_;
    
    int color = TTT_COLOR_RED;
    
    if (x->allow_) { color = TTT_COLOR_GREEN; }
    
    if (x->tests_[m].comment_) {
        ttt_stderr (color, ">>> %03ld / %ld / %s", m + 1, n, x->tests_[m].comment_);
    } else {
        ttt_stderr (color, ">>> %03ld / %ld", m + 1, n);
    }
    
    if (!x->allow_) { return TTT_ERROR; }
    //
    }
    
    return TTT_GOOD;
}

static TTTError ttt_testPerformRun (TTTTest *x)
{
    pid_t pid;
    int result;
    TTTError err = TTT_ERROR;
    TTTFnTest test = NULL;
    
    if ((test = ttt_testPerformRunStart (x))) {
    //
    int id = x->id_;
    
    /* Need to loop on waitpid for use in debugger. */
    
    if ((pid = fork()) == -1) { TTT_SYSTEM_ERROR; }
    else {
    //
    if (pid == 0) {
        (*test) (id); _exit (EXIT_SUCCESS);
    } else {
        pid_t p;
        do { p = waitpid (pid, &result, 0); } while (p == -1 && errno == EINTR);
        if (p == -1) { TTT_SYSTEM_ERROR; }
        else {
            if (WIFSIGNALED (result)) {
                ttt_stderr (TTT_COLOR_RED, ">>> Fails / Process");
            } else if (WIFEXITED (result) && WEXITSTATUS (result) == 0) {
                x->error_ = TTT_GOOD;
            }
        }
    }
    //
    }
    
    err = ttt_testPerformRunEnd (x);
    //
    }
    
    x->index_++;
    
    return err;
}

static TTTError ttt_testRunAll (TTTTest *x)
{
    TTTError err = TTT_ERROR;
    
    if (x) {
        err = TTT_GOOD; while (x->index_ < x->size_) { err |= ttt_testPerformRun (x); }
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

TTTError ttt_testAll (void)
{
    TTTError err = TTT_ERROR;
    TTTTest *tests = NULL;
    
    ttt_stdout (TTT_COLOR_NONE, "### Begin");
    
    if ((tests = ttt_testNew (ttt_testSize, ttt_testElements))) {
        err = TTT_GOOD | ttt_testRunAll (tests);
        ttt_testFree (tests);
    }
    
    if (err) { ttt_stdout (TTT_COLOR_RED, "### FAIL"); }
    else {
        ttt_stdout (TTT_COLOR_NONE, "### PASS");
    }
    
    return err;
}

void ttt_testAdd (TTTFnTest test, const char *comment)
{
    int n = ttt_testSize++;
    
    if (n < TTT_MAXIMUM_SIZE) {
        ttt_testElements[n].method_  = test;
        ttt_testElements[n].comment_ = comment;
    }
}

void ttt_testBegin (int testId, int allow)
{
    TTTTest *x = NULL;

    if ((x = (TTTTest *)shmat (testId, NULL, 0)) == (void *)-1) { TTT_SYSTEM_ERROR; _exit (EXIT_FAILURE); }
    else {
        x->allow_ = allow; if (shmdt (x) == -1) { TTT_SYSTEM_ERROR; }
    }
}

void ttt_testEnd (int testId, int hasFailed)
{
    TTTTest *x = NULL;

    if ((x = (TTTTest *)shmat (testId, NULL, 0)) == (void *)-1) { TTT_SYSTEM_ERROR; _exit (EXIT_FAILURE); }
    else {
        x->failed_ = hasFailed; if (shmdt (x) == -1) { TTT_SYSTEM_ERROR; }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
