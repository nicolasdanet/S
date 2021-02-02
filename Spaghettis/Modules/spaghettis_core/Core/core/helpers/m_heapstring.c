
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define HEAPSTRING_DEFAULT_SIZE     4096

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void heapstring_reserve (t_heapstring *x, size_t size)
{
    if (size > x->hs_allocated) {
    //
    size_t newSize  = (size_t)PD_NEXT_POWER_2 (size);
    size_t oldSize  = x->hs_allocated;

    x->hs_raw       = (char *)PD_MEMORY_RESIZE (x->hs_raw, oldSize, newSize);
    x->hs_allocated = newSize;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL char *heapstring_getRaw (t_heapstring *x)
{
    return x->hs_raw;
}

PD_LOCAL int heapstring_getSize (t_heapstring *x)
{
    int size = (int)x->hs_size; PD_ASSERT (size >= 0); return size;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Caller acquires string ownership. */

PD_LOCAL char *heapstring_clearBorrow (t_heapstring *x)
{
    char *borrowed = x->hs_raw;
    
    x->hs_size = 0;
    x->hs_raw  = (char *)PD_MEMORY_GET (x->hs_allocated);
    
    return borrowed;
}

/* Caller acquires string ownership. */

PD_LOCAL char *heapstring_freeBorrowUnzeroed (t_heapstring *x)
{
    char *borrowed = (char *)PD_MEMORY_RESIZE (x->hs_raw, x->hs_allocated, x->hs_size);
    
    x->hs_raw = NULL;
    
    heapstring_free (x);
    
    return borrowed;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int heapstring_containsCharacterAtEnd (t_heapstring *x, char c)
{
    if (x->hs_size) {
    //
    if (x->hs_raw[x->hs_size - 1] == c) { return 1; }
    //
    }
    
    return 0;
}

PD_LOCAL void heapstring_removeCharacterIfContainsAtEnd (t_heapstring *x, char c)
{
    if (heapstring_containsCharacterAtEnd (x, c)) {
    //
    if (x->hs_raw[x->hs_size - 1] == c) { x->hs_raw[x->hs_size - 1] = 0; x->hs_size--; }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error heapstring_add (t_heapstring *x, const char *src)
{
    return heapstring_append (x, src, -1);
}

PD_LOCAL t_error heapstring_append (t_heapstring *x, const char *src, int n)
{
    size_t size = 0;
        
    if (n < 0) { size = strlen (src); }
    else {
        const char *t = src; while (*t && size < (size_t)n) { size++; t++; }
    }
    
    if (size) {
    //
    heapstring_reserve (x, x->hs_size + size + 1);
    memmove (x->hs_raw + x->hs_size, src, size);
    x->hs_size += size;
    x->hs_raw[x->hs_size] = 0;
    //
    }
    
    return PD_ERROR_NONE;
}

#if defined ( PD_BUILDING_TESTS )

PD_LOCAL t_error heapstring_addSprintf (t_heapstring *x, const char *format, ...)
{
    size_t required = x->hs_size + (x->hs_allocated / 4);
    t_error err = PD_ERROR_NONE;
    va_list args;
    
    do {
    //
    heapstring_reserve (x, required);
    
    {
        size_t remains = x->hs_allocated - x->hs_size;
        int t;
        
        va_start (args, format);
        t = vsnprintf (x->hs_raw + x->hs_size, remains, format, args);
        va_end (args);
    
        if (t < 0) { PD_BUG; return PD_ERROR; }
        else {
            if ((err = ((size_t)t >= remains))) { required = x->hs_size + (size_t)t + 1; }
            else { 
                x->hs_size += (size_t)t;
            }
        }
    }
    //
    } while (err);
    
    return err;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void heapstring_clear (t_heapstring *x)
{
    x->hs_size = 0; x->hs_raw[0] = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_heapstring *heapstring_new (int size)
{
    t_heapstring *x = (t_heapstring *)PD_MEMORY_GET (sizeof (t_heapstring));
 
    x->hs_allocated = (size <= 0) ? HEAPSTRING_DEFAULT_SIZE : size;
    x->hs_size      = 0;
    x->hs_raw       = (char *)PD_MEMORY_GET (x->hs_allocated);
    
    return x;
}

PD_LOCAL void heapstring_free (t_heapstring *x)
{
    if (x->hs_raw) { PD_MEMORY_FREE (x->hs_raw); }
    
    PD_MEMORY_FREE (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
