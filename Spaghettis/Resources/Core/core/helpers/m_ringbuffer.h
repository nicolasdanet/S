
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_RINGBUFFER_H_
#define M_RINGBUFFER_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* A basic ring buffer for a single writer and a single reader. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* The number of elements MUST be a power of two. */

PD_LOCAL t_ringbuffer   *ringbuffer_new         (int32_t sizeOfElementInBytes, int32_t numberOfElements);

PD_LOCAL void           ringbuffer_free         (t_ringbuffer *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Functions below could be called concurrently. */

/* Usable in DSP. */

PD_LOCAL int32_t ringbuffer_getAvailableWrite   (t_ringbuffer *x);
PD_LOCAL int32_t ringbuffer_getAvailableRead    (t_ringbuffer *x);
PD_LOCAL int32_t ringbuffer_write               (t_ringbuffer *x, const void *v, int32_t n);
PD_LOCAL int32_t ringbuffer_read                (t_ringbuffer *x, void *v, int32_t n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_RINGBUFFER_H_
