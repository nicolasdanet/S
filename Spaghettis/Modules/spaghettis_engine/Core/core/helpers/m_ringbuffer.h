
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_RINGBUFFER_H_
#define M_RINGBUFFER_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that it is largely hacked from PortAudio. */

/* < http://www.portaudio.com/ > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* A basic ring buffer for a single writer and a single reader. */

typedef struct _ringbuffer {
    t_int32Atomic   rb_read;
    t_int32Atomic   rb_write;
    uint32_t        rb_hiMask;
    uint32_t        rb_loMask;
    int32_t         rb_size;
    size_t          rb_bytes;
    char            *rb_vector;
    } t_ringbuffer;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* The number of elements MUST be a power of two. */

t_ringbuffer   *ringbuffer_new         (int32_t sizeOfElementInBytes, int32_t numberOfElements);

void           ringbuffer_free         (t_ringbuffer *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Functions below could be called concurrently. */

/* Usable in DSP. */

int32_t ringbuffer_getAvailableWrite   (t_ringbuffer *x);
int32_t ringbuffer_getAvailableRead    (t_ringbuffer *x);
int32_t ringbuffer_write               (t_ringbuffer *x, const void *v, int32_t n);
int32_t ringbuffer_read                (t_ringbuffer *x, void *v, int32_t n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_RINGBUFFER_H_
