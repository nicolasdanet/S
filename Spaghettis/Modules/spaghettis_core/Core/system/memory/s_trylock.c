
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* I don't use TTAS since no contention at all is expected and it doesn't spin neither. */

/* < https://rigtorp.se/spinlock/ > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int trylock_perform (t_trylock *t)
{
    #if defined ( __cplusplus )

    using std::memory_order_acquire;

    #endif

    return atomic_flag_test_and_set_explicit (t, memory_order_acquire);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Cannot use ATOMIC_FLAG INT in assignment. */

void trylock_init (t_trylock *t)
{
    atomic_flag_clear (t);
}

void trylock_destroy (t_trylock *t)
{

}

int trylock_trylock (t_trylock *t)
{
    return (trylock_perform (t) == 0);
}

void trylock_lock (t_trylock *t)
{
    while (trylock_perform (t)) { nano_sleep (PD_MILLISECONDS_TO_NANOSECONDS (0.1)); }
}

void trylock_unlock (t_trylock *t)
{
    #if defined ( __cplusplus )
    
    using std::memory_order_release;

    #endif

    atomic_flag_clear_explicit (t, memory_order_release);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
