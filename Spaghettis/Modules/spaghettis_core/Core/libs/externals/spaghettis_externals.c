
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT t_class *spaghettis_classNew (t_symbol *name,
    t_newmethod fnNew,
    t_method    fnFree,
    size_t      size,
    int         flags)
{
    return class_new (name, fnNew, fnFree, size, flags, A_NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT void spaghettis_classFree (t_class *c)
{
    class_free (c);
}

PD_EXPORT void spaghettis_classAddBang (t_class *c, t_method fn)
{
    class_addBang (c, fn);
}

PD_EXPORT void spaghettis_classAddFloat (t_class *c, t_method fn)
{
    class_addFloat (c, fn);
}

PD_EXPORT void spaghettis_classAddSymbol (t_class *c, t_method fn)
{
    class_addSymbol (c, fn);
}

PD_EXPORT void spaghettis_classAddList (t_class *c, t_method fn)
{
    class_addList (c, fn);
}

PD_EXPORT void spaghettis_classAddAnything (t_class *c, t_method fn)
{
    class_addAnything (c, fn);
}

PD_EXPORT void spaghettis_classSetHelpDirectory (t_class *c, t_symbol *s)
{
    class_setHelpDirectory (c, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT void spaghettis_post (const char *s)
{
    post (s);
}

PD_EXPORT void spaghettis_postWarning (const char *s)
{
    post_warning (s);
}

PD_EXPORT void spaghettis_postError (const char *s)
{
    post_error (s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT t_symbol *spaghettis_makeSymbol (const char *s)
{
    return gensym (s);
}

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

PD_EXPORT void *spaghettis_memoryResize (void *m, size_t oldSize, size_t newSize)
{
    return PD_MEMORY_RESIZE (m, oldSize, newSize);
}

PD_EXPORT void spaghettis_memoryFree (void *m)
{
    PD_MEMORY_FREE (m);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT t_object *spaghettis_objectNew (t_class *c)
{
    t_pd *x = pd_new (c); return cast_object (x);
}

PD_EXPORT void spaghettis_objectFree (t_object *x)
{
    pd_free (cast_pd (x));
}

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

PD_EXPORT int spaghettis_objectFlagIsUndoOrEncaspulate (t_object *x, int flags)
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
