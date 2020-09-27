
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void instance_chainSetInitialized (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct _dspthread {
    t_error             x_error;
    t_float64Atomic     x_time;
    t_uint32Atomic      x_flag;
    t_pointerAtomic     x_chain;
    pthread_cond_t      x_condition;
    pthread_mutex_t     x_lock;
    pthread_t           x_thread;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Arbitrary (based on experiments) sleep ratio (for audio vector size greater than 64). */

#if PD_APPLE

#define DSPTHREAD_SLEEP     0.8

#endif

#if PD_LINUX

#define DSPTHREAD_SLEEP     0.9

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    DSP_NONE    = 0,
    DSP_RUN     = 1,
    DSP_EXIT    = 2
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL double audio_getNanosecondsToSleep (void);

PD_LOCAL t_error priority_setPolicy (pthread_t);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int dspthread_isChainSafeToDelete (t_dspthread *x, t_chain *chainToDelete)
{
    if (x) {
    //
    if ((t_chain *)PD_ATOMIC_POINTER_READ (&x->x_chain) == chainToDelete) { return 0; }
    //
    }
    
    return 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void dspthread_run (t_dspthread *x)
{
    pthread_mutex_lock (&x->x_lock);
    
    PD_ATOMIC_UINT32_SET (DSP_RUN, &x->x_flag); pthread_cond_signal (&x->x_condition);
    
    pthread_mutex_unlock (&x->x_lock);
}

PD_LOCAL void dspthread_stop (t_dspthread *x)
{
    pthread_mutex_lock (&x->x_lock);
    
    PD_ATOMIC_UINT32_UNSET (DSP_RUN, &x->x_flag);
    
    pthread_mutex_unlock (&x->x_lock);
}

static void dspthread_exit (t_dspthread *x)
{
    pthread_mutex_lock (&x->x_lock);
    
    PD_ATOMIC_UINT32_SET (DSP_EXIT, &x->x_flag); pthread_cond_signal (&x->x_condition);
    
    pthread_mutex_unlock (&x->x_lock);
}

static void dspthread_wait (t_dspthread *x)
{
    pthread_mutex_lock (&x->x_lock);
    
    while (PD_ATOMIC_UINT32_READ (&x->x_flag) == DSP_NONE) {
        pthread_cond_wait (&x->x_condition, &x->x_lock);
    }
    
    pthread_mutex_unlock (&x->x_lock);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int dspthread_proceed (t_dspthread *x)
{
    int done = 1;
    
    PD_ATOMIC_POINTER_WRITE (instance_chainGetCurrent(), &x->x_chain);
    
    if (PD_ATOMIC_POINTER_READ (&x->x_chain) && PD_ATOMIC_UINT32_TRUE (DSP_RUN, &x->x_flag)) {
    //
    t_chain *chain = (t_chain *)PD_ATOMIC_POINTER_READ (&x->x_chain);
    
    int stuck = 0;
    
    done = 0;
    
    do {
    //
    t_time t = 0.0; int dacs = audio_poll();
    
    #if PD_WITH_DEBUG
    
    if (dacs == DACS_NO)         { PD_LOG ("#"); }
    else if (dacs == DACS_YES)   { PD_LOG ("!"); }
    else if (dacs == DACS_SLEPT) { PD_LOG ("/"); }
    
    #endif
    
    int work = (dacs != DACS_NO);
    int wait = (dacs == DACS_YES) && (audio_getVectorSize() > INTERNAL_BLOCKSIZE);
    
    if (wait) { time_set (&t); time_addNanoseconds (&t, audio_getNanosecondsToSleep() * DSPTHREAD_SLEEP); }
    if (work) { chain_tick (chain); stuck = 0; done = 1; } else { stuck++; }
    if (wait) { time_wait (&t); }
    
    if (stuck > 10 || PD_ATOMIC_UINT32_TRUE (DSP_EXIT, &x->x_flag)) { break; }
    //
    } while (chain_hasQuantumRemaining (chain));
    //
    }
    
    PD_ATOMIC_POINTER_WRITE (NULL, &x->x_chain);
    
    return done;
}

static void *dspthread_thread (void *z)
{
    t_dspthread *x = (t_dspthread *)z;
    
    privilege_check();
    
    denormal_setPolicy();   /* If inheritance is broken. */
    
    while (PD_ATOMIC_UINT32_FALSE (DSP_EXIT, &x->x_flag)) {
    //
    while (PD_ATOMIC_UINT32_FALSE (DSP_EXIT, &x->x_flag) && PD_ATOMIC_UINT32_TRUE (DSP_RUN, &x->x_flag)) {
    //
    PD_ATOMIC_FLOAT64_WRITE (0, &x->x_time);
    
    if (!dspthread_proceed (x)) { nano_sleep (audio_getNanosecondsToSleep()); }
    
    instance_clocksClean();
    //
    }
    
    PD_ATOMIC_FLOAT64_WRITE (scheduler_getLogicalTime(), &x->x_time);
    
    dspthread_wait (x);
    //
    }
    
    instance_chainSetInitialized();
    
    return (NULL);
}

PD_LOCAL t_error dspthread_create (t_dspthread *x)
{
    t_error err  = PD_ERROR_NONE;
    t_error err1 = PD_ERROR_NONE;
    t_error err2 = PD_ERROR_NONE;
    
    err1 = (pthread_cond_init (&x->x_condition, NULL) != 0);
    err2 = (pthread_mutex_init (&x->x_lock, NULL) != 0);
    err  = err1 | err2;
    
    /* Release in case of failure (doesn't really matter since the application will exit). */

    if (err) {
        if (!err1) { pthread_cond_destroy (&x->x_condition); }
        if (!err2) { pthread_mutex_destroy (&x->x_lock); }
    }
    
    /* Create the DSP thread (RT if possible). */
    
    if (!err) { err = (pthread_create (&x->x_thread, NULL, dspthread_thread, (void *)x) != 0); }
    if (!err) { priority_setPolicy (x->x_thread); }

    x->x_error = err;
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_systime dspthread_time (t_dspthread *x)
{
    return (t_systime)PD_ATOMIC_FLOAT64_READ (&x->x_time);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_dspthread *dspthread_new (void)
{
    t_dspthread *x = (t_dspthread *)PD_MEMORY_GET (sizeof (t_dspthread));
    
    x->x_error = PD_ERROR;
    
    return x;
}

PD_LOCAL void dspthread_free (t_dspthread *x)
{
    if (!x->x_error) {
    //
    dspthread_exit (x);
    
    pthread_join (x->x_thread, NULL);
    
    pthread_mutex_destroy (&x->x_lock);
    pthread_cond_destroy (&x->x_condition);
    //
    }
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
