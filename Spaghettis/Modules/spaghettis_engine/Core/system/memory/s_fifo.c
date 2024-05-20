
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// TODO: Optimize?

/* https://github.com/CharlesFrasch/cppcon2023 */

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
// MARK: -

/* https://en.cppreference.com/w/cpp/language/reinterpret_cast#Type_aliasing */

static void fifo_writeProceed (t_fifo *x, int size, int bytes, const void *data, uint64_t index, int writted)
{
    #if defined ( __cplusplus )
    const char *p = static_cast<const char*> (data);
    #else
    const char *p = data;
    #endif
    
    while (writted--) {
    //
    int t = (index & (size - 1)) * bytes;
    
    memcpy (x->f_vector + t, p, bytes);
    
    p     += bytes;
    index += 1;
    //
    }
}

int fifo_shared_write (t_fifo *x, int size, int bytes, const void *data, int n)
{
    int available  = fifo_shared_getAvailableWrite (x, size);
    int writted    = PD_CLAMP (n, 0, available);
    
    uint64_t index = atomic_uInt64ReadRelaxed (&x->f_write);
    
    fifo_writeProceed (x, size, bytes, data, index, writted);
    
    atomic_uInt64WriteRelease (&x->f_write, index + writted);
    
    return writted;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void fifo_readProceed (t_fifo *x, int size, int bytes, void *data, uint64_t index, int readed)
{
    #if defined ( __cplusplus )
    char *p = static_cast<char*> (data);
    #else
    char *p = data;
    #endif
    
    while (readed--) {
    //
    int t = (index & (size - 1)) * bytes;
    
    memcpy (p, x->f_vector + t, bytes);
    
    p     += bytes;
    index += 1;
    //
    }
}

int fifo_shared_read (t_fifo *x, int size, int bytes, void *data, int n)
{
    int available  = fifo_shared_getAvailableRead (x);
    int readed     = PD_CLAMP (n, 0, available);
    
    uint64_t index = atomic_uInt64ReadRelaxed (&x->f_read);
    
    fifo_readProceed (x, size, bytes, data, index, readed);
    
    atomic_uInt64WriteRelease (&x->f_read, index + readed);
    
    return readed;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
