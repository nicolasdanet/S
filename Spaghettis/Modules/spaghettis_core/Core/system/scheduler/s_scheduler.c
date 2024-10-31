
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SCHEDULER_INPUTS    19                      /* Roughly 20ms period. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    SCHEDULER_RUN   = 0,
    SCHEDULER_QUIT  = 1,
    SCHEDULER_ERROR = 2
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_int32Atomic    scheduler_quit;             /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_float64Atomic  scheduler_systime;          /* Static. */

/* MUST be thread-safe (after initialization). */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

double scheduler_getRealTimeInSeconds (void)
{
    static t_time start;                    /* Static. */
    t_time now;
    t_nano elapsed = 0ULL;
    
    time_set (&now);
    
    if (start == 0ULL) { start = now; }
    else {
        time_elapsedNanoseconds (&start, &now, &elapsed);
    }
    
    return PD_NANOSECONDS_TO_SECONDS (elapsed);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void scheduler_setLogicalTime (t_systime t)
{
    atomic_float64Write (&scheduler_systime, t);
}

t_systime scheduler_getLogicalTime (void)
{
    return atomic_float64Read (&scheduler_systime);
}

t_systime scheduler_getLogicalTimeAfter (double ms)
{
    return (scheduler_getLogicalTime() + ms);
}

double scheduler_getMillisecondsSince (t_systime systime)
{
    return (scheduler_getLogicalTime() - systime);
}

double scheduler_getUnitsSince (t_systime systime, double unit, int isSamples)
{
    double d = 1.0; double elapsed = scheduler_getMillisecondsSince (systime);
    
    PD_ASSERT (elapsed >= 0.0);
    
    if (isSamples) { d = 1000.0 / audio_getSampleRate(); }
    
    return (elapsed / (d * unit));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_systime scheduler_addMillisecondsToLogicalTime (t_systime t, double ms)
{
    return (t + ms);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void scheduler_needToExit (void)
{
    atomic_int32Write (&scheduler_quit, SCHEDULER_QUIT);
}

void scheduler_needToExitWithError (void)
{
    atomic_int32Write (&scheduler_quit, SCHEDULER_ERROR);
}

int scheduler_isExiting (void)
{
    int n = atomic_int32Read (&scheduler_quit); return (n == SCHEDULER_QUIT || n == SCHEDULER_ERROR);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void scheduler_tick (void)
{
    if (!atomic_int32Read (&scheduler_quit)) {
    //
    t_systime t = scheduler_getLogicalTime() + 1.0;
    
    instance_clocksTick (t);
    
    scheduler_setLogicalTime (t);
    //
    }
}

static void scheduler_clean (void)
{
    if (!atomic_int32Read (&scheduler_quit) && !audio_isOpened()) {
    //
    instance_dspClean();
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int scheduler_mainLoopNeedToTick (double realStart, t_systime logicalStart, double *t)
{
    if (!atomic_int32Read (&scheduler_quit)) {
    //
    double now          = scheduler_getRealTimeInSeconds();
    double realLapse    = PD_SECONDS_TO_MILLISECONDS (now - realStart);
    double logicalLapse = scheduler_getMillisecondsSince (logicalStart);
    
    if (*t == 0.0) { *t = now; }
    
    return (realLapse > logicalLapse);
    //
    }
    
    return 0;
}

static void scheduler_mainLoop (void)
{
    const double realStart       = scheduler_getRealTimeInSeconds();
    const t_systime logicalStart = scheduler_getLogicalTime();
    
    int pollInputsCounter = 0;
    
    while (!atomic_int32Read (&scheduler_quit)) {
    //
    double t = 0.0;
    
    while (scheduler_mainLoopNeedToTick (realStart, logicalStart, &t)) {
        scheduler_tick();
        midi_poll();
        monitor_nonBlocking();
        if (++pollInputsCounter > SCHEDULER_INPUTS) { pollInputsCounter = 0; wrapper_poll(); }
    }
        
    if (!atomic_int32Read (&scheduler_quit)) { scheduler_clean(); }
    
    if (!atomic_int32Read (&scheduler_quit)) {
        double elapsed = PD_SECONDS_TO_MILLISECONDS (scheduler_getRealTimeInSeconds() - t);
        double monitor = (0.75 - elapsed);
        if (monitor > 0.0) { monitor_blocking (monitor); }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_error scheduler_main (void)
{
    // midi_open();
    
        instance_autoreleaseRun();
        instance_pollingRun();
    
            scheduler_mainLoop(); dsp_close();
    
        instance_pollingStop();
        instance_autoreleaseStop();
    
    audio_close();
    midi_close();
    
    return (atomic_int32Read (&scheduler_quit) == SCHEDULER_ERROR);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
