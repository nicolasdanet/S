
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* In debug build memory leaks for externals are globally tracked. */

PD_EXPORT void *memory_getForExternal (size_t n)
{
    return PD_MEMORY_GET (n);
}

PD_EXPORT void *memory_getResizeForExternal (void *ptr, size_t oldSize, size_t newSize)
{
    return PD_MEMORY_RESIZE (ptr, oldSize, newSize);
}

PD_EXPORT void memory_freeForExternal (void *ptr)
{
    PD_MEMORY_FREE (ptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
