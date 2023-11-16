
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *memory_get (size_t n)
{
    void *r = calloc (n < 1 ? 1 : n, 1);

    PD_ASSERT (r != NULL);
    PD_ABORT  (r == NULL);
    
    return r;
}

void *memory_getResize (void *ptr, size_t oldSize, size_t newSize)
{
    void *r = NULL;
    
    if (oldSize < 1) { oldSize = 1; }
    if (newSize < 1) { newSize = 1; }
    
    r = realloc (ptr, newSize);
    
    PD_ASSERT (r != NULL);
    PD_ABORT  (r == NULL);
    
    if (newSize > oldSize) { memset (((char *)r) + oldSize, 0, newSize - oldSize); }
    
    return r;
}

void memory_free (void *ptr)
{
    free (ptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
