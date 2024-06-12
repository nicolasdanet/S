
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void clock_increment    (t_clock *);
void clock_decrement    (t_clock *);
void clocks_tickAppend  (t_clocks *, t_clock *, t_systime);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void clocks_removeClock (t_buffer *x, t_clock *c)
{
    int i, n = buffer_getSize (x);
    
    int k = -1;
    
    for (i = 0; i < n; i++) {
        if (buffer_getClockAt (x, i) == c) { k = i; break; }
    }
    
    if (k > 0) {
        buffer_removeAtIndex (x, k);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clocks_addSingle (t_clocks *x, t_clock *c)
{
    PD_ASSERT (sys_isControlThread());
    
    clock_increment (c); buffer_appendClock (x->x_single, c);
}

void clocks_removeSingle (t_clocks *x, t_clock *c)
{
    if (clock_count (c) > 0) {
    //
    PD_ASSERT (sys_isControlThread());
    
    clocks_removeClock (x->x_single, c); clock_decrement (c);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clocks_tickCheckSingle (t_clocks *x, t_systime systime)
{
    int i, n = buffer_getSize (x->x_single);
    
    buffer_clear (x->x_cache);
    
    for (i = 0; i < n; i++) {
    //
    t_clock *c     = buffer_getClockAt (x->x_single, i);
    t_systime time = clock_getLogicalTime (c);
    
    if (time <= systime) { clocks_tickAppend (x, c, time); }
    else {
        buffer_appendClock (x->x_cache, c);
    }
    //
    }
    
    buffer_swap (x->x_single, x->x_cache);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
