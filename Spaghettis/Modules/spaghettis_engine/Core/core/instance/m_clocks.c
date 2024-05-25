
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

struct _clocks {
    t_pointerAtomic     *x_clocks;
    t_buffer            *x_cache;
    t_buffer            *x_garbage;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define CLOCKS_SIZE     256     /* Arbitrary. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       scheduler_setLogicalTime    (t_systime);
t_systime  clock_getExecuteTime        (t_clock *);
void       clock_setExecuteTime        (t_clock *, t_systime);
void       clock_increment             (t_clock *);
void       clock_decrement             (t_clock *);
void       clock_inhibit               (t_clock *);
void       clock_execute               (t_clock *);
int        clock_isGood                (t_clock *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clocks_add (t_clocks *x, t_clock *c)
{
    t_error err = PD_ERROR;
    
    int i;

    for (i = 0; i < CLOCKS_SIZE; i++) {
    //
    void *t = atomic_pointerRead (x->x_clocks + i);
        
    if (t == NULL) {
        if (atomic_pointerCompareAndSwap (x->x_clocks + i, &t, (void *)c)) {
            clock_increment (c);
            err = PD_ERROR_NONE;
            break;
        }
    }
    //
    }
        
    if (err) { PD_BUG; PD_ABORT (1); }
}

void clocks_remove (t_clocks *x, t_clock *c)
{
    if (clock_isSet (c)) {
    //
    /* Possible there that a clock is concurrently removed (while/and cached for executing). */
    /* It doesn't really matter. */
    
    int i;

    for (i = 0; i < CLOCKS_SIZE; i++) {
    //
    void *t = atomic_pointerRead (x->x_clocks + i);
        
    if (t == (void *)c) {
        if (atomic_pointerCompareAndSwap (x->x_clocks + i, &t, (void *)NULL)) {
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

/* Clocks are temporary stored before to be freed. */
/* Unset a clock doesn't remove it from the cached clocks (while executed). */
/* Theoritically a clock could trigger deletion of another one already in cached. */
/* Wait that all clocks have fired before to remove them. */

void clocks_destroy (t_clocks *x, t_clock *c)
{
    t_atom a; SET_CLOCK (&a, c);
    
    clock_inhibit (c); clocks_remove (x, c);
    
    #if PD_WITH_DEBUG
        PD_ASSERT (clock_isGood (c));
    #endif
    
    buffer_appendAtom (x->x_garbage, &a);
}

static void clocks_purge (t_clocks *x)
{
    int i, n = buffer_getSize (x->x_garbage);
    
    for (i = 0; i < n; i++) {
        t_clock *c = buffer_getClockAt (x->x_garbage, i); PD_MEMORY_FREE (c);
    }
    
    buffer_clear (x->x_garbage);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Fetching clock is immune from ABA problem as clocks deletion is garbage collected. */

static void clocks_tickCheck (t_clocks *x, t_systime systime, int i)
{
    void *t = atomic_pointerRead (x->x_clocks + i);
    
    if (t) {
    //
    t_clock *c     = (t_clock *)t;
    t_systime time = clock_getLogicalTime (c);
    
    if (time <= systime) {
        if (atomic_pointerCompareAndSwap (x->x_clocks + i, &t, (void *)NULL)) {
            t_atom a;
            SET_CLOCK (&a, c);
            clock_decrement (c);
            clock_setExecuteTime (c, time);
            buffer_appendAtom (x->x_cache, &a);
            return;
        }
    }
    //
    }
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
    int i, n = buffer_getSize (x->x_cache);
    
    if (n) {
    //
    buffer_qsort (x->x_cache, clocks_tickCompare);
    
    for (i = 0; i < n; i++) {
    //
    t_clock *c = buffer_getClockAt (x->x_cache, i);
    scheduler_setLogicalTime (clock_getExecuteTime (c));
    clock_execute (c);
    //
    }
    
    buffer_clear (x->x_cache);
    //
    }
}

void clocks_tick (t_clocks *x, t_systime systime)
{
    int i; for (i = 0; i < CLOCKS_SIZE; i++) { clocks_tickCheck (x, systime, i); }
    
    clocks_tickExecute (x); clocks_purge (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_clocks *clocks_new (void)
{
    t_clocks *x  = (t_clocks *)PD_MEMORY_GET (sizeof (t_clocks));
    
    x->x_clocks  = (t_pointerAtomic *)PD_MEMORY_GET (sizeof (t_pointerAtomic) * CLOCKS_SIZE);
    x->x_cache   = buffer_new();
    x->x_garbage = buffer_new();
    
    return x;
}

void clocks_free (t_clocks *x)
{
    clocks_purge (x);
    
    buffer_free (x->x_garbage);
    buffer_free (x->x_cache);
    
    PD_MEMORY_FREE (x->x_clocks);
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_TESTS )

static char clocks_string[CLOCKS_SIZE + 1];     /* Static. */

char *clocks_debug (t_clocks *x, int n)
{
    int i;
    
    n = PD_CLAMP (n, 0, CLOCKS_SIZE);
    
    for (i = 0; i < n; i++) {
    //
    char c = '.';
    
    if (atomic_pointerReadRelaxed (x->x_clocks + i)) { c = '#'; }
    
    clocks_string[i] = c;
    //
    }

    clocks_string[n] = 0;
    
    return clocks_string;
}

#endif // PD_BUILDING_TESTS

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
