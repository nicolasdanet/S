
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_RING_H_
#define S_RING_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* A ring buffer for a single writer and a single reader. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _ring {
    t_fifo  f_fifo;          /* Must be the first. */
    int     f_size;
    int     f_bytes;
    } t_ring;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Size must be a power of two. */

t_ring  *ring_new (int size, int bytes);

void    ring_free (t_ring *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Functions below could be called concurrently. */

/* Usable in DSP. */

int ring_getAvailableWrite  (t_ring *x);
int ring_getAvailableRead   (t_ring *x);
int ring_write              (t_ring *x, const void *v, int n);
int ring_read               (t_ring *x, void *v,       int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_RING_H_
