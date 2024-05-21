
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef M_FIFO_H_
#define M_FIFO_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// TODO: Optimize?

/* https://github.com/CharlesFrasch/cppcon2023 */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _fifo {
    char            *f_vector;
    t_uint64Atomic  f_read;
    t_uint64Atomic  f_write;
    } t_fifo;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_fifo  *fifo_shared_new    (int size, int bytes);

void    fifo_shared_free    (t_fifo *x);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int fifo_shared_getAvailableRead (t_fifo *x)
{
    uint64_t a = atomic_uInt64ReadAcquire (&x->f_write);
    uint64_t b = atomic_uInt64ReadRelaxed (&x->f_read);
    
    return (int)(a - b);
}

static inline int fifo_shared_getAvailableWrite (t_fifo *x, int size)
{
    uint64_t b = atomic_uInt64ReadAcquire (&x->f_read);
    uint64_t a = atomic_uInt64ReadRelaxed (&x->f_write);
    
    return (size - (int)(a - b));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* https://en.cppreference.com/w/cpp/language/reinterpret_cast#Type_aliasing */

static inline void fifo_shared_writeProceed (t_fifo *x,
    int size,
    int bytes,
    const void *data,
    uint64_t index,
    int writted)
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

static inline int fifo_shared_write (t_fifo *x, int size, int bytes, const void *data, int n)
{
    int available  = fifo_shared_getAvailableWrite (x, size);
    int writted    = PD_CLAMP (n, 0, available);
    
    uint64_t index = atomic_uInt64ReadRelaxed (&x->f_write);
    
    fifo_shared_writeProceed (x, size, bytes, data, index, writted);
    
    atomic_uInt64WriteRelease (&x->f_write, index + writted);
    
    return writted;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void fifo_shared_readProceed (t_fifo *x,
    int size,
    int bytes,
    void *data,
    uint64_t index,
    int readed)
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

static inline int fifo_shared_read (t_fifo *x, int size, int bytes, void *data, int n)
{
    int available  = fifo_shared_getAvailableRead (x);
    int readed     = PD_CLAMP (n, 0, available);
    
    uint64_t index = atomic_uInt64ReadRelaxed (&x->f_read);
    
    fifo_shared_readProceed (x, size, bytes, data, index, readed);
    
    atomic_uInt64WriteRelease (&x->f_read, index + readed);
    
    return readed;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _fifo8  { t_fifo f_fifo; } t_fifo8;
typedef struct _fifo32 { t_fifo f_fifo; } t_fifo32;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // M_FIFO_H_
