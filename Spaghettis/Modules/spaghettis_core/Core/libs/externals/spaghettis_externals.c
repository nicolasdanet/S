
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

PD_EXPORT void spaghettis_classAddMethod (t_class *c, t_method fn, t_symbol *s)
{
    class_addMethod (c, fn, s, A_NULL);
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

PD_EXPORT t_symbol *spaghettis_symbol (const char *s)
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

PD_EXPORT t_outlet *spaghettis_objectOutletNewBang (t_object *x)
{
    return outlet_newBang (x);
}

PD_EXPORT t_outlet *spaghettis_objectOutletNewFloat (t_object *x)
{
    return outlet_newFloat (x);
}

PD_EXPORT t_outlet *spaghettis_objectOutletNewSymbol (t_object *x)
{
    return outlet_newSymbol (x);
}

PD_EXPORT t_outlet *spaghettis_objectOutletNewList (t_object *x)
{
    return outlet_newList (x);
}

PD_EXPORT t_outlet *spaghettis_objectOutletNewAnything (t_object *x)
{
    return outlet_newAnything (x);
}

PD_EXPORT t_outlet *spaghettis_objectOutletNewMixed (t_object *x)
{
    return outlet_newMixed (x);
}

PD_EXPORT t_outlet *spaghettis_objectOutletNewSignal (t_object *x)
{
    return outlet_newSignal (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT void spaghettis_outletBang (t_outlet *x)
{
    outlet_bang (x);
}

PD_EXPORT void spaghettis_outletFloat (t_outlet *x, t_float f)
{
    outlet_float (x, f);
}

PD_EXPORT void spaghettis_outletSymbol (t_outlet *x, t_symbol *s)
{
    outlet_symbol (x, s);
}

PD_EXPORT void spaghettis_outletList (t_outlet *x, int argc, t_atom *argv)
{
    outlet_list (x, argc, argv);
}

PD_EXPORT void spaghettis_outletAnything (t_outlet *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_anything (x, s, argc, argv);
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
// MARK: -

PD_EXPORT t_buffer *spaghettis_bufferNew (void)
{
    return buffer_new();
}

PD_EXPORT t_atom *spaghettis_bufferGetAtoms (t_buffer *x)
{
    return buffer_getAtoms (x);
}

PD_EXPORT void spaghettis_bufferFree (t_buffer *x)
{
    buffer_free (x);
}

PD_EXPORT int spaghettis_bufferGetSize (t_buffer *x)
{
    return buffer_getSize (x);
}

PD_EXPORT void spaghettis_bufferClear (t_buffer *x)
{
    buffer_clear (x);
}

PD_EXPORT void spaghettis_bufferAppend (t_buffer *x, int argc, t_atom *argv)
{
    buffer_append (x, argc, argv);
}

PD_EXPORT void spaghettis_bufferAppendFloat (t_buffer *x, t_float f)
{
    buffer_appendFloat (x, f);
}

PD_EXPORT void spaghettis_bufferAppendSymbol (t_buffer *x, t_symbol *s)
{
    buffer_appendSymbol (x, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_EXPORT char *spaghettis_atomsToString (int argc, t_atom *argv)
{
    return atom_atomsToString (argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
