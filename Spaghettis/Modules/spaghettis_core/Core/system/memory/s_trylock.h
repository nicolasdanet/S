
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_TRYLOCK_H_
#define S_TRYLOCK_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://forum.juce.com/t/is-it-safe-to-use-std-try-lock-or-scopedtrylock-on-the-realtime-thread > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0

typedef pthread_mutex_t t_trylock;

static inline void trylock_init (t_trylock *t)
{
    pthread_mutex_init (t, NULL);
}

static inline void trylock_destroy (t_trylock *t)
{
    pthread_mutex_destroy (t);
}

static inline int trylock_trylock (t_trylock *t)
{
    return pthread_mutex_trylock (t);
}

static inline void trylock_lock (t_trylock *t)
{
    pthread_mutex_lock (t);
}

static inline void trylock_unlock (t_trylock *t)
{
    pthread_mutex_unlock (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#else

#if defined ( __cplusplus )

    #include <atomic>
    
    typedef std::atomic_flag t_trylock;
        
#else

    #include <stdatomic.h>
    
    typedef atomic_flag t_trylock;
    
#endif

static inline void trylock_init (t_trylock *t)
{
    atomic_flag_clear (t);
}

static inline void trylock_destroy (t_trylock *t)
{

}

static inline int trylock_trylock (t_trylock *t)
{
    #if defined ( __cplusplus )

    using std::memory_order_acquire;

    #endif

    return atomic_flag_test_and_set_explicit (t, memory_order_acquire);
}

static inline void trylock_lock (t_trylock *t)
{
    while (trylock_trylock (t)) { nano_sleep (PD_MILLISECONDS_TO_NANOSECONDS (0.1)); PD_LOG ("*?*"); }
}

static inline void trylock_unlock (t_trylock *t)
{
    #if defined ( __cplusplus )
    
    using std::memory_order_release;

    #endif

    atomic_flag_clear_explicit (t, memory_order_release);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_TRYLOCK_H_
