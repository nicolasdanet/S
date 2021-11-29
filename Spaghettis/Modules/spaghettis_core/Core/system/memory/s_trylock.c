
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if TRYLOCK_EXPERIMENTAL

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void trylock_init (t_trylock *t)
{
    atomic_flag_clear (t);
}

PD_LOCAL void trylock_destroy (t_trylock *t)
{

}

 PD_LOCAL int trylock_trylock (t_trylock *t)
{
    #if defined ( __cplusplus )

    using std::memory_order_acquire;

    #endif

    int b = atomic_flag_test_and_set_explicit (t, memory_order_acquire);
    
    if (b) { PD_LOG ("*?*"); }
    
    return b;
}

PD_LOCAL void trylock_lock (t_trylock *t)
{
    while (trylock_trylock (t)) { nano_sleep (PD_MILLISECONDS_TO_NANOSECONDS (0.1)); }
}

PD_LOCAL void trylock_unlock (t_trylock *t)
{
    #if defined ( __cplusplus )
    
    using std::memory_order_release;

    #endif

    atomic_flag_clear_explicit (t, memory_order_release);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#else

PD_LOCAL void trylock_init (t_trylock *t)
{
    pthread_mutex_init (t, NULL);
}

PD_LOCAL void trylock_destroy (t_trylock *t)
{
    pthread_mutex_destroy (t);
}

PD_LOCAL int trylock_trylock (t_trylock *t)
{
    return pthread_mutex_trylock (t);
}

PD_LOCAL void trylock_lock (t_trylock *t)
{
    pthread_mutex_lock (t);
}

PD_LOCAL void trylock_unlock (t_trylock *t)
{
    pthread_mutex_unlock (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // TRYLOCK_EXPERIMENTAL

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
