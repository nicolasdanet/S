
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define FIFO32_SIZE     8192    /* Must be a power of two. */
#define FIFO32_BYTES    4

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_fifo32 *fifo32_new (void)
{
    return (t_fifo32 *)fifo_shared_new (FIFO32_SIZE, FIFO32_BYTES);
}

void fifo32_free (t_fifo32 *x)
{
    fifo_shared_free ((t_fifo *)x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int fifo32_getAvailableRead (t_fifo32 *x)
{
    return fifo_shared_getAvailableRead ((t_fifo *)x);
}

int fifo32_getAvailableWrite (t_fifo32 *x)
{
    return fifo_shared_getAvailableWrite ((t_fifo *)x, FIFO32_SIZE);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int fifo32_write (t_fifo32 *x, const void *data, int n)
{
    return fifo_shared_write ((t_fifo *)x, FIFO32_SIZE, FIFO32_BYTES, data, n);
}

int fifo32_read (t_fifo32 *x, void *data, int n)
{
    return fifo_shared_read ((t_fifo *)x, FIFO32_SIZE, FIFO32_BYTES, data, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------