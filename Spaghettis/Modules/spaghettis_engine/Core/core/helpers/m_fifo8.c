
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

#define FIFO8_MASK  (FIFO8_SIZE - 1)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_fifo8 *fifo8_new (void)
{
    t_fifo8 *x  = (t_fifo8 *)PD_MEMORY_GET (sizeof (t_fifo8));
    
    x->f_vector = (char*)PD_MEMORY_GET (FIFO8_SIZE);
    x->f_read   = 0;
    x->f_write  = 0;
    
    return x;
}

void fifo8_free (t_fifo8 *x)
{
    PD_MEMORY_FREE (x->f_vector);
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int fifo8_getAvailableRead (t_fifo8 *x)
{
    uint64_t a = atomic_uInt64ReadAcquire (&x->f_write);
    uint64_t b = atomic_uInt64ReadRelaxed (&x->f_read);
    
    return (int)(a - b);
}

int fifo8_getAvailableWrite (t_fifo8 *x)
{
    uint64_t b = atomic_uInt64ReadAcquire (&x->f_read);
    uint64_t a = atomic_uInt64ReadRelaxed (&x->f_write);
    
    return (FIFO8_SIZE - (int)(a - b));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* https://en.cppreference.com/w/cpp/language/reinterpret_cast#Type_aliasing */

static void fifo8_writeProceed (t_fifo8 *x, const void *data, uint64_t index, int writted)
{
    #if defined ( __cplusplus )
    const char *p = static_cast<const char*> (data);
    #else
    const char *p = data;
    #endif
    
    while (writted--) {
    //
    int t = (index & FIFO8_MASK);
    
    memcpy (x->f_vector + t, p, 1);
    
    p     += 1;
    index += 1;
    //
    }
}

int fifo8_write (t_fifo8 *x, const void *data, int n)
{
    int available  = fifo8_getAvailableWrite (x);
    int writted    = PD_CLAMP (n, 0, available);
    
    uint64_t index = atomic_uInt64ReadRelaxed (&x->f_write);
    
    fifo8_writeProceed (x, data, index, writted);
    
    atomic_uInt64WriteRelease (&x->f_write, index + writted);
    
    return writted;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void fifo8_readProceed (t_fifo8 *x, void *data, uint64_t index, int readed)
{
    #if defined ( __cplusplus )
    char *p = static_cast<char*> (data);
    #else
    char *p = data;
    #endif
    
    while (readed--) {
    //
    int t = (index & FIFO8_MASK);
    
    memcpy (p, x->f_vector + t, 1);
    
    p     += 1;
    index += 1;
    //
    }
}

int fifo8_read (t_fifo8 *x, void *data, int n)
{
    int available  = fifo8_getAvailableRead (x);
    int readed     = PD_CLAMP (n, 0, available);
    
    uint64_t index = atomic_uInt64ReadRelaxed (&x->f_read);
    
    fifo8_readProceed (x, data, index, readed);
    
    atomic_uInt64WriteRelease (&x->f_read, index + readed);
    
    return readed;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
