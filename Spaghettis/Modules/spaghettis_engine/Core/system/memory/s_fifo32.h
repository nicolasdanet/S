
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_FIFO_32_H_
#define S_FIFO_32_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* A fix-sized ring buffer (4 bytes). */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_fifo32    *fifo32_new (void);

void        fifo32_free (t_fifo32 *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Functions below could be called concurrently. */

/* Usable in DSP. */

int fifo32_getAvailableWrite    (t_fifo32 *x);
int fifo32_getAvailableRead     (t_fifo32 *x);
int fifo32_write                (t_fifo32 *x, const void *v, int n);
int fifo32_read                 (t_fifo32 *x, void *v,       int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_FIFO_32_H_