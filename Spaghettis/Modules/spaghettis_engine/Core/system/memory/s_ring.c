
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_ring *ring_new (int size, int bytes)
{
    t_ring *x = (t_ring *)PD_MEMORY_GET (sizeof (t_ring));
    
    PD_ASSERT (PD_IS_POWER_2 (size));
    PD_ASSERT (bytes == 1 || bytes == 4);
    
    fifo_shared_initialize ((t_fifo *)x, size, bytes);
    
    x->f_size  = size;
    x->f_bytes = bytes;
    
    return x;
}

void ring_free (t_ring *x)
{
    fifo_shared_release ((t_fifo *)x);
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int ring_getAvailableRead (t_ring *x)
{
    return fifo_shared_getAvailableRead ((t_fifo *)x);
}

int ring_getAvailableWrite (t_ring *x)
{
    return fifo_shared_getAvailableWrite ((t_fifo *)x, x->f_size);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int ring_write (t_ring *x, const void *data, int n)
{
    return fifo_shared_write ((t_fifo *)x, x->f_size, x->f_bytes, data, n);
}

int ring_read (t_ring *x, void *data, int n)
{
    return fifo_shared_read ((t_fifo *)x, x->f_size, x->f_bytes, data, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
