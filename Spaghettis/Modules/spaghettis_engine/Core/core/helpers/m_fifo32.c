
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

// TODO: Optimize?

/* https://github.com/CharlesFrasch/cppcon2023 */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define FIFO32_MASK     (FIFO32_SIZE - 1)
#define FIFO32_BYTES    (4)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_fifo32 *fifo32_new (void)
{
    t_fifo32 *x = (t_fifo32 *)PD_MEMORY_GET (sizeof (t_fifo32));
    
    x->f_vector = (char*)PD_MEMORY_GET (FIFO32_SIZE * FIFO32_BYTES);
    x->f_read   = 0;
    x->f_write  = 0;
    
    return x;
}

void fifo32_free (t_fifo32 *x)
{
    PD_MEMORY_FREE (x->f_vector);
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int fifo32_getAvailableRead (t_fifo32 *x)
{
    uint64_t a = atomic_uInt64ReadAcquire (&x->f_write);
    uint64_t b = atomic_uInt64ReadRelaxed (&x->f_read);
    
    return (int)(a - b);
}

int fifo32_getAvailableWrite (t_fifo32 *x)
{
    uint64_t b = atomic_uInt64ReadAcquire (&x->f_read);
    uint64_t a = atomic_uInt64ReadRelaxed (&x->f_write);
    
    return (FIFO32_SIZE - (int)(a - b));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* https://en.cppreference.com/w/cpp/language/reinterpret_cast#Type_aliasing */

static void fifo32_writeProceed (t_fifo32 *x, const void *data, uint64_t index, int writted)
{
    #if defined ( __cplusplus )
    
    const char *p = static_cast<const char*> (data);
    
    #else
    
    const char *p = data;
    
    #endif
    
    while (writted--) {
    //
    int t = (index & FIFO32_MASK) * FIFO32_BYTES;
    
    memcpy (x->f_vector + t, p, FIFO32_BYTES);
    
    p     += FIFO32_BYTES;
    index += 1;
    //
    }
}

int fifo32_write (t_fifo32 *x, const void *data, int n)
{
    int available  = fifo32_getAvailableWrite (x);
    int writted    = PD_CLAMP (n, 0, available);
    
    uint64_t index = atomic_uInt64ReadRelaxed (&x->f_write);
    
    fifo32_writeProceed (x, data, index, writted);
    
    atomic_uInt64WriteRelease (&x->f_write, index + writted);
    
    return writted;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void fifo32_readProceed (t_fifo32 *x, void *data, uint64_t index, int readed)
{
    #if defined ( __cplusplus )
    
    char *p = static_cast<char*> (data);
    
    #else
    
    char *p = data;
    
    #endif
    
    while (readed--) {
    //
    int t = (index & FIFO32_MASK) * FIFO32_BYTES;
    
    memcpy (p, x->f_vector + t, FIFO32_BYTES);
    
    p     += FIFO32_BYTES;
    index += 1;
    //
    }
}

int fifo32_read (t_fifo32 *x, void *data, int n)
{
    int available  = fifo32_getAvailableRead (x);
    int readed     = PD_CLAMP (n, 0, available);
    
    uint64_t index = atomic_uInt64ReadRelaxed (&x->f_read);
    
    fifo32_readProceed (x, data, index, readed);
    
    atomic_uInt64WriteRelease (&x->f_read, index + readed);
    
    return readed;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
