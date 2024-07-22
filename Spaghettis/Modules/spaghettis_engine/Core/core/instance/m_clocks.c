
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        scheduler_setLogicalTime    (t_systime);
void        buffer_freeContent          (t_buffer *);
t_systime   clock_getExecuteTime        (t_clock *);
void        clock_setExecuteTime        (t_clock *, t_systime);
void        clock_increment             (t_clock *);
void        clock_decrement             (t_clock *);
void        clock_inhibit               (t_clock *);
void        clock_execute               (t_clock *);

void        clocks_addSafe              (t_clocks *, t_clock *);
void        clocks_removeSafe           (t_clocks *, t_clock *);
void        clocks_tickCheckSafe        (t_clocks *x, t_systime systime);

void        clocks_addSingle            (t_clocks *, t_clock *);
void        clocks_removeSingle         (t_clocks *, t_clock *);
void        clocks_tickCheckSingle      (t_clocks *x, t_systime systime);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clocks_add (t_clocks *x, t_clock *c)
{
    if (clock_isSingle (c)) { clocks_addSingle (x, c); } else { clocks_addSafe (x, c); }
}

void clocks_remove (t_clocks *x, t_clock *c)
{
    if (clock_isSingle (c)) { clocks_removeSingle (x, c); } else { clocks_removeSafe (x, c); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Clocks are temporary stored before to be freed. */
/* Unset a clock doesn't remove it from the cached clocks (while executed). */
/* Theoritically a clock could trigger deletion of another one already in cached. */
/* Wait that all clocks have fired before to remove them. */

void clocks_destroy (t_clocks *x, t_clock *c)
{
    clock_inhibit (c); clocks_remove (x, c);
    
    #if PD_WITH_DEBUG
        PD_ASSERT (clock_count (c) == 0);
    #endif
    
    buffer_appendAsClock (x->x_garbage, c);
}

static void clocks_purge (t_clocks *x)
{
    buffer_freeContent (x->x_garbage);
    buffer_clear (x->x_garbage);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clocks_tickAppend (t_clocks *x, t_clock *c, t_systime time)
{
    clock_decrement (c);
    clock_setExecuteTime (c, time);
    buffer_appendAsClock (x->x_executed, c);
}

static int clocks_tickCompare (const void *p1, const void *p2)
{
    t_clock *c1 = GET_CLOCK ((t_atom *)p1);
    t_clock *c2 = GET_CLOCK ((t_atom *)p2);
    t_float t1  = clock_getExecuteTime (c1);
    t_float t2  = clock_getExecuteTime (c2);
    
    if (t1 > t2) { return 1; } else if (t1 < t2) { return -1; }
    
    return 0;
}

static void clocks_tickExecute (t_clocks *x)
{
    int i, n = buffer_getSize (x->x_executed);
    
    if (n) {
    //
    buffer_qsort (x->x_executed, clocks_tickCompare);
    
    for (i = 0; i < n; i++) {
    //
    t_clock *c = buffer_getClockAt (x->x_executed, i);
    scheduler_setLogicalTime (clock_getExecuteTime (c));
    clock_execute (c);
    //
    }
    
    buffer_clear (x->x_executed);
    //
    }
}

void clocks_tick (t_clocks *x, t_systime systime)
{
    clocks_tickCheckSafe (x, systime);
    clocks_tickCheckSingle (x, systime);
    clocks_tickExecute (x);
    clocks_purge (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_clocks *clocks_new (void)
{
    t_clocks *x = (t_clocks *)PD_MEMORY_GET (sizeof (t_clocks));
    
    x->x_safe       = (t_pointerAtomic *)PD_MEMORY_GET (sizeof (t_pointerAtomic) * CLOCKS_SIZE);
    x->x_single     = buffer_new();
    x->x_cache      = buffer_new();
    x->x_executed   = buffer_new();
    x->x_garbage    = buffer_new();
    
    return x;
}

void clocks_free (t_clocks *x)
{
    clocks_purge (x);
    
    buffer_free (x->x_garbage);
    buffer_free (x->x_executed);
    buffer_free (x->x_cache);
    buffer_free (x->x_single);
    
    PD_MEMORY_FREE (x->x_safe);
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_TESTS )

static char clocks_string[CLOCKS_SIZE + 1];     /* Static. */

char *clocks_debug (t_clocks *x, int n)
{
    int i;
    
    n = PD_CLAMP (n, 0, CLOCKS_SIZE);
    
    for (i = 0; i < n; i++) {
    //
    char c = '.';
    
    if (atomic_pointerReadRelaxed (x->x_safe + i)) { c = '#'; }
    
    clocks_string[i] = c;
    //
    }

    clocks_string[n] = 0;
    
    return clocks_string;
}

#endif // PD_BUILDING_TESTS

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
