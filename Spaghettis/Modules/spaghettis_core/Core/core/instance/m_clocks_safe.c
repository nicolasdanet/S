
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// TODO: Think about relaxing memory order constraints for efficiency.

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clock_increment    (t_clock *);
void clock_decrement    (t_clock *);
void clocks_tickAppend  (t_clocks *, t_clock *, t_systime);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clocks_addSafe (t_clocks *x, t_clock *c)
{
    t_error err = PD_ERROR;
    
    int i;

    for (i = 0; i < CLOCKS_SIZE; i++) {
    //
    void *t = atomic_pointerRead (x->x_safe + i);
        
    if (t == NULL) {
        if (atomic_pointerCompareAndSwap (x->x_safe + i, &t, (void *)c)) {
            clock_increment (c);
            err = PD_ERROR_NONE;
            break;
        }
    }
    //
    }
    
    if (err) { PD_BUG; PD_ABORT (1); }  // TODO: What?
}

void clocks_removeSafe (t_clocks *x, t_clock *c)
{
    if (clock_count (c) > 0) {
    //
    /* Possible there that a clock is concurrently consummed (while/and cached for executing). */
    /* It doesn't really matter. */
    
    int i;

    for (i = 0; i < CLOCKS_SIZE; i++) {
    //
    void *t = atomic_pointerRead (x->x_safe + i);
        
    if (t == (void *)c) {
        if (atomic_pointerCompareAndSwap (x->x_safe + i, &t, (void *)NULL)) {
            clock_decrement (c);
        }
        break;
    }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Fetching clock is immune from ABA problem as clocks deletion is garbage collected. */

void clocks_tickCheckSafe (t_clocks *x, t_systime systime)
{
    int i;
    
    for (i = 0; i < CLOCKS_SIZE; i++) {
    //
    void *t = atomic_pointerRead (x->x_safe + i);
    
    if (t) {
    //
    t_systime time = clock_getLogicalTime ((t_clock *)t);
    
    if (time <= systime) {
    
        /* The clock could have been rescheduled meantime. */
        /* Too late, still triggered with old logical time. */
        
        if (atomic_pointerCompareAndSwap (x->x_safe + i, &t, (void *)NULL)) {
            clocks_tickAppend (x, (t_clock *)t, time);
        }
    }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
