
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_fifo *fifo_shared_new (int size, int bytes)
{
    t_fifo *x  = (t_fifo *)PD_MEMORY_GET (sizeof (t_fifo));
    
    x->f_vector = (char*)PD_MEMORY_GET (size * bytes);
    x->f_read   = 0;
    x->f_write  = 0;
    
    return x;
}

void fifo_shared_free (t_fifo *x)
{
    PD_MEMORY_FREE (x->f_vector);
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int fifo_shared_getAvailableRead (t_fifo *x)
{
    uint64_t a = atomic_uInt64ReadAcquire (&x->f_write);
    uint64_t b = atomic_uInt64ReadRelaxed (&x->f_read);
    
    return (int)(a - b);
}

int fifo_shared_getAvailableWrite (t_fifo *x, int size)
{
    uint64_t b = atomic_uInt64ReadAcquire (&x->f_read);
    uint64_t a = atomic_uInt64ReadRelaxed (&x->f_write);
    
    return (size - (int)(a - b));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
