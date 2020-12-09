
/* Copyright (c) 2017-2020 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef TTT_T_H
#define TTT_T_H

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A new process is launched for each test. Shared memory is used for IPC. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://en.wikipedia.org/wiki/Shared_memory > */
/* < https://en.wikipedia.org/wiki/Inter-process_communication > */
/* < http://space.wccnet.edu/~chasselb/linux275/ClassNotes/ipc/shared_mem.htm > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( LINUX ) || defined ( __linux__ )
    #define TTT_LINUX                   1
#elif defined ( __APPLE__ )
    #define TTT_APPLE                   1
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( __GNUC__ ) || defined ( __clang__ )
    #define TTT_CONSTRUCTOR             1
#else
    #error "Unsupported compiler!"
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if TTT_LINUX
#else
#if TTT_APPLE
#else
    #error "Unsupported platform!"
#endif
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if TTT_LINUX

    #define TTT_POSIX_TIME              1

#else
#if TTT_APPLE

    #include "Availability.h"

    #if __MAC_OS_X_VERSION_MAX_ALLOWED >= 101200
        #define TTT_POSIX_TIME          1
    #else
        #define TTT_MAC_TIME            1
    #endif

#endif // TTT_APPLE
#endif // TTT_LINUX

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if TTT_LINUX
    #ifndef _GNU_SOURCE
    #define _GNU_SOURCE                 /* Required to use pthread_setaffinity_np function. */
    #endif
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef int                             TTTError;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TTT_GOOD                        0
#define TTT_ERROR                       1

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TTT_COLOR_NONE                  0
#define TTT_COLOR_RED                   1
#define TTT_COLOR_GREEN                 2
#define TTT_COLOR_BLUE                  3

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TTT_MAX(a,b)                    ((a)>(b)?(a):(b))
#define TTT_MIN(a,b)                    ((a)<(b)?(a):(b))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _TTTWaste {
    int step_;
    int index_;
    } TTTWaste;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ttt_wasteInit      (TTTWaste *q, int n);
void ttt_wasteTime      (TTTWaste *q);              /* Waste CPU cycles randomly. */
void ttt_wasteMoreTime  (TTTWaste *q);              /* Waste hundred more CPU cycles randomly. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef void (*TTTFnTest) (int testId);
typedef void *(*TTTFnTestThread) (void *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

double   ttt_timeTrigger                (void);

TTTError ttt_stdout                     (int color, const char *format, ...);
TTTError ttt_stderr                     (int color, const char *format, ...);

int      ttt_testThreadsNumber          (void);
TTTError ttt_testThreadsLaunch          (TTTFnTestThread test);

TTTError ttt_testAll                    (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TTT_BEGIN(name, priority, comment)              TTT_BEGIN_MACRO (name, priority, comment, 0)
#define TTT_BEGIN_ALLOW(name, priority, comment)        TTT_BEGIN_MACRO (name, priority, comment, 1)

#define TTT_END                                         TTT_END_MACRO()

#define TTT_EXPECT(expression)                          TTT_EXPECT_MACRO (expression, __LINE__)
#define TTT_FAIL                                        TTT_EXPECT_MACRO (0, __LINE__)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Function below are private. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TTT_JOIN(a, b)                  TTT_JOIN_MACRO (a, b)
#define TTT_JOIN_MACRO(a, b)            a ## b

#define TTT_STRINGIFY(a)                TTT_STRINGIFY_MACRO (a)
#define TTT_STRINGIFY_MACRO(a)          #a

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define TTT_SHORT_FILE                  (strrchr (__FILE__, '/') ? strrchr (__FILE__, '/') + 1 : __FILE__)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if TTT_CONSTRUCTOR

#define TTT_BEGIN_MACRO(name, priority, comment, allow)    \
    extern TTTError ttt_unitError; \
    extern int ttt_unitAllowed; \
    void TTT_JOIN (ttt_unitTest, name) (int testId);  \
    void TTT_JOIN (TTT_JOIN (ttt_unitTest, name), Ctor) (void)  __attribute__ ((constructor)); \
    void TTT_JOIN (TTT_JOIN (ttt_unitTest, name), Ctor) (void)  {  \
        ttt_testAdd (TTT_JOIN (ttt_unitTest, name), priority, comment); \
    } \
    void TTT_JOIN (ttt_unitTest, name) (int testId_) {  \
        ttt_testBegin (testId_, allow); \
        ttt_unitAllowed = allow;

#define TTT_END_MACRO() \
        ttt_testEnd (testId_, (ttt_unitError != TTT_GOOD)); \
    }

#define TTT_EXPECT_MACRO(expression, line)    \
    if (!(expression)) {    \
        ttt_unitError |= TTT_ERROR;    \
        int color = (ttt_unitAllowed == 1 ? TTT_COLOR_GREEN : TTT_COLOR_RED);  \
        ttt_stderr (color, ">>> False / %s / line %d", TTT_SHORT_FILE, line);  \
    }

#endif // TTT_CONSTRUCTOR

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ttt_testAdd    (TTTFnTest test, int priority, const char *comment);
void ttt_testBegin  (int testId, int allow);
void ttt_testEnd    (int testId, int hasFailed);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // TTT_T_H
