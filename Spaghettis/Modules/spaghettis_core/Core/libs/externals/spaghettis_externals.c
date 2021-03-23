
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT const char *spaghettis_symbolGetName (t_symbol *s)
{
    return symbol_getName (s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* In debug build memory leaks for externals are globally tracked. */

PD_EXPORT void *spaghettis_memoryGet (size_t n)
{
    return PD_MEMORY_GET (n);
}

PD_EXPORT void *spaghettis_memoryResize (void *ptr, size_t oldSize, size_t newSize)
{
    return PD_MEMORY_RESIZE (ptr, oldSize, newSize);
}

PD_EXPORT void spaghettis_memoryFree (void *ptr)
{
    PD_MEMORY_FREE (ptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT t_object *spaghettis_objectGetTemporary (t_object *x)
{
    t_object *old = NULL;
    
    if (class_hasDSP (pd_class (x))) {
        old = garbage_fetch (x);
    } else {
        old = instance_pendingFetch (x);
    }
    
    return old;
}

PD_EXPORT t_space *spaghettis_objectGetNewSpace (t_object *x)
{
    return space_new (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT int spaghettis_isUndoOrEncaspulate (t_object *x, int flags)
{
    return SAVED_DEEP (flags);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://lists.puredata.info/pipermail/pd-dev/2016-11/020878.html > */
/* < https://lists.puredata.info/pipermail/pd-list/2005-07/029490.html > */
/* < https://www.mail-archive.com/pd-list@iem.at/msg60031.html > */

PD_EXPORT t_float spaghettis_signalGetSampleRate (t_signal *s)
{
    return s->s_sampleRate;
}

PD_EXPORT int spaghettis_signalGetVectorSize (t_signal *s)
{
    return s->s_vectorSize;
}

PD_EXPORT int spaghettis_signalGetOverlap (t_signal *s)
{
    return s->s_overlap;
}

PD_EXPORT t_sample *spaghettis_signalGetVector (t_signal *s)
{
    return s->s_vector;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
