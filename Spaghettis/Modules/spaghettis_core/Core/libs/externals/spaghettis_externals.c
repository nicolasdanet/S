
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_clock *spaghettis_clockNew (void *x, t_method fn)
{
    return clock_new (x, fn);
}

PD_DLL void spaghettis_clockFree (t_clock *x)
{
    clock_free (x);
}

PD_DLL void spaghettis_clockUnset (t_clock *x)
{
    clock_unset (x);
}

PD_DLL void spaghettis_clockDelay (t_clock *x, double delay)
{
    clock_delay (x, delay);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_class *spaghettis_classNew (t_symbol *name,
    t_newmethod fnNew,
    t_method    fnFree,
    size_t      size,
    int         flags)
{
    return class_new (name, fnNew, fnFree, size, flags, A_NULL);
}

PD_DLL t_class *spaghettis_classNewWithArguments (t_symbol *name,
    t_newmethod fnNew,
    t_method    fnFree,
    size_t      size,
    int         flags)
{
    return class_new (name, fnNew, fnFree, size, flags, A_GIMME, A_NULL);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void spaghettis_classFree (t_class *c)
{
    class_free (c);
}

PD_DLL void spaghettis_classAddMethodWithArguments (t_class *c, t_method fn, t_symbol *s)
{
    class_addMethod (c, fn, s, A_GIMME, A_NULL);
}

PD_DLL void spaghettis_classAddMethod (t_class *c, t_method fn, t_symbol *s)
{
    class_addMethod (c, fn, s, A_NULL);
}

PD_DLL void spaghettis_classAddBang (t_class *c, t_method fn)
{
    class_addBang (c, fn);
}

PD_DLL void spaghettis_classAddFloat (t_class *c, t_method fn)
{
    class_addFloat (c, fn);
}

PD_DLL void spaghettis_classAddSymbol (t_class *c, t_method fn)
{
    class_addSymbol (c, fn);
}

PD_DLL void spaghettis_classAddList (t_class *c, t_method fn)
{
    class_addList (c, fn);
}

PD_DLL void spaghettis_classAddAnything (t_class *c, t_method fn)
{
    class_addAnything (c, fn);
}

PD_DLL void spaghettis_classAddDsp (t_class *c, t_method fn)
{
    class_addDsp (c, fn);
}

PD_DLL void spaghettis_classSetHelpDirectory (t_class *c, t_symbol *s)
{
    class_setHelpDirectory (c, s);
}

PD_DLL void spaghettis_classSetDataFunction (t_class *c, t_datafn f)
{
    class_setDataFunction (c, f);
}

PD_DLL void spaghettis_classSetDismissFunction (t_class *c, t_dismissfn f)
{
    class_setDismissFunction (c, f);
}

PD_DLL void spaghettis_classRequirePending (t_class *c)
{
    class_requirePending (c);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void spaghettis_post (t_object *x, const char *s)
{
    post (x, s);
}

PD_DLL void spaghettis_postWarning (t_object *x, const char *s)
{
    post_warning (x, s);
}

PD_DLL void spaghettis_postError (t_object *x, const char *s)
{
    post_error (x, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void spaghettis_bind (t_pd *x, t_symbol *s)
{
    pd_bind (x, s);
}

PD_DLL void spaghettis_unbind (t_pd *x, t_symbol *s)
{
    pd_unbind (x, s);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void spaghettis_handleBang (t_pd *x)
{
    pd_bang (x);
}

PD_DLL void spaghettis_handleFloat (t_pd *x, t_float f)
{
    pd_float (x, f);
}

PD_DLL void spaghettis_handleSymbol (t_pd *x, t_symbol *s)
{
    pd_symbol (x, s);
}

PD_DLL void spaghettis_handleList (t_pd *x, int argc, t_atom *argv)
{
    pd_list (x, argc, argv);
}

PD_DLL void spaghettis_handleMessage (t_pd *x, t_symbol *s, int argc, t_atom *argv)
{
    pd_message (x, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_symbol *spaghettis_symbol (const char *s)
{
    return gensym (s);
}

PD_DLL const char *spaghettis_symbolGetName (t_symbol *s)
{
    return symbol_getName (s);
}

PD_DLL t_pd *spaghettis_symbolGetThing (t_symbol *s)
{
    return symbol_getThing (s);
}

PD_DLL int spaghettis_symbolHasThing (t_symbol *s)
{
    return symbol_hasThing (s);
}

PD_DLL int spaghettis_symbolHasThingQuiet (t_symbol *s)
{
    return symbol_hasThingQuiet (s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_symbol *spaghettis_getRestoreSymbol (void)
{
    return gensym ("_restore");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* In debug build memory leaks for externals are globally tracked. */

PD_DLL void *spaghettis_memoryGet (size_t n)
{
    return PD_MEMORY_GET (n);
}

PD_DLL void *spaghettis_memoryResize (void *m, size_t oldSize, size_t newSize)
{
    return PD_MEMORY_RESIZE (m, oldSize, newSize);
}

PD_DLL void spaghettis_memoryFree (void *m)
{
    PD_MEMORY_FREE (m);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_object *spaghettis_objectNew (t_class *c)
{
    t_pd *x = pd_new (c); return cast_object (x);
}

PD_DLL void spaghettis_objectFree (t_object *x)
{
    pd_free (cast_pd (x));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_object *spaghettis_objectGetTemporary (t_object *x)
{
    t_object *old = NULL;
    
    if (class_hasDsp (pd_class (x))) {
        old = garbage_fetch (x);
    } else {
        old = instance_pendingFetch (x);
    }
    
    return old;
}

PD_DLL t_space *spaghettis_objectGetNewSpace (t_object *x)
{
    return space_new (x);
}

PD_DLL int spaghettis_objectDspNeedInitializer (t_object *x)
{
    return object_dspNeedInitializer (x);
}

PD_DLL void spaghettis_objectCopySignalValues (t_object *x, t_object *old)
{
    object_copySignalValues (x, old);
}

PD_DLL void spaghettis_objectGetSignalValues (t_object *x, t_buffer *b)
{
    object_getSignalValues (x, b);
}

PD_DLL void spaghettis_objectFetchAndCopySignalValuesIfRequired (t_object *x)
{
    object_fetchAndCopySignalValuesIfRequired (x);
}

PD_DLL int spaghettis_objectFlagIsUndoOrEncaspulate (t_object *x, int flags)
{
    return SAVED_DEEP (flags);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_inlet *spaghettis_objectInletNewFloat (t_object *x, t_float *fp)
{
    return inlet_newFloat (x, fp);
}

PD_DLL t_inlet *spaghettis_objectInletNewSymbol (t_object *x, t_symbol **sp)
{
    return inlet_newSymbol (x, sp);
}

PD_DLL t_inlet *spaghettis_objectInletNewSignal (t_object *x)
{
    return inlet_newSignal (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_outlet *spaghettis_objectOutletNewBang (t_object *x)
{
    return outlet_newBang (x);
}

PD_DLL t_outlet *spaghettis_objectOutletNewFloat (t_object *x)
{
    return outlet_newFloat (x);
}

PD_DLL t_outlet *spaghettis_objectOutletNewSymbol (t_object *x)
{
    return outlet_newSymbol (x);
}

PD_DLL t_outlet *spaghettis_objectOutletNewList (t_object *x)
{
    return outlet_newList (x);
}

PD_DLL t_outlet *spaghettis_objectOutletNewAnything (t_object *x)
{
    return outlet_newAnything (x);
}

PD_DLL t_outlet *spaghettis_objectOutletNewMixed (t_object *x)
{
    return outlet_newMixed (x);
}

PD_DLL t_outlet *spaghettis_objectOutletNewSignal (t_object *x)
{
    return outlet_newSignal (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void spaghettis_outletBang (t_outlet *x)
{
    outlet_bang (x);
}

PD_DLL void spaghettis_outletFloat (t_outlet *x, t_float f)
{
    outlet_float (x, f);
}

PD_DLL void spaghettis_outletSymbol (t_outlet *x, t_symbol *s)
{
    outlet_symbol (x, s);
}

PD_DLL void spaghettis_outletList (t_outlet *x, int argc, t_atom *argv)
{
    outlet_list (x, argc, argv);
}

PD_DLL void spaghettis_outletAnything (t_outlet *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_anything (x, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void spaghettis_dspAdd1 (t_perform fn, t_int a)
{
    dsp_add1 (fn, a);
}

PD_DLL void spaghettis_dspAdd2 (t_perform fn, t_int a, t_int b)
{
    dsp_add2 (fn, a, b);
}

PD_DLL void spaghettis_dspAdd3 (t_perform fn, t_int a, t_int b, t_int c)
{
    dsp_add3 (fn, a, b, c);
}

PD_DLL void spaghettis_dspAdd4 (t_perform fn, t_int a, t_int b, t_int c, t_int d)
{
    dsp_add4 (fn, a, b, c, d);
}

PD_DLL void spaghettis_dspAdd5 (t_perform fn, t_int a, t_int b, t_int c, t_int d, t_int e)
{
    dsp_add5 (fn, a, b, c, d, e);
}

PD_DLL void spaghettis_dspAdd6 (t_perform fn, t_int a, t_int b, t_int c, t_int d, t_int e, t_int f)
{
    dsp_add6 (fn, a, b, c, d, e, f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://lists.puredata.info/pipermail/pd-dev/2016-11/020878.html > */
/* < https://lists.puredata.info/pipermail/pd-list/2005-07/029490.html > */
/* < https://www.mail-archive.com/pd-list@iem.at/msg60031.html > */

PD_DLL t_float spaghettis_signalGetSampleRate (t_signal *s)
{
    return s->s_sampleRate;
}

PD_DLL int spaghettis_signalGetVectorSize (t_signal *s)
{
    return s->s_vectorSize;
}

PD_DLL int spaghettis_signalGetOverlap (t_signal *s)
{
    return s->s_overlap;
}

PD_DLL t_sample *spaghettis_signalGetVector (t_signal *s)
{
    return s->s_vector;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_buffer *spaghettis_bufferNew (void)
{
    return buffer_new();
}

PD_DLL t_atom *spaghettis_bufferGetAtoms (t_buffer *x)
{
    return buffer_getAtoms (x);
}

PD_DLL void spaghettis_bufferFree (t_buffer *x)
{
    buffer_free (x);
}

PD_DLL int spaghettis_bufferGetSize (t_buffer *x)
{
    return buffer_getSize (x);
}

PD_DLL void spaghettis_bufferClear (t_buffer *x)
{
    buffer_clear (x);
}

PD_DLL void spaghettis_bufferAppend (t_buffer *x, int argc, t_atom *argv)
{
    buffer_append (x, argc, argv);
}

PD_DLL void spaghettis_bufferAppendFloat (t_buffer *x, t_float f)
{
    buffer_appendFloat (x, f);
}

PD_DLL void spaghettis_bufferAppendSymbol (t_buffer *x, t_symbol *s)
{
    buffer_appendSymbol (x, s);
}

PD_DLL void spaghettis_bufferAppendSemicolon (t_buffer *x)
{
    buffer_appendSemicolon (x);
}

PD_DLL void spaghettis_bufferAppendComma (t_buffer *x)
{
    buffer_appendComma (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL char *spaghettis_atomsToString (int argc, t_atom *argv)
{
    return atom_atomsToString (argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL void spaghettis_atomSetSymbol (t_atom *a, t_symbol *s)
{
    atom_setSymbol (a, s);
}

PD_DLL void spaghettis_atomSetFloat (t_atom *a, t_float f)
{
    atom_setFloat (a, f);
}

PD_DLL t_float spaghettis_atomGetFloat (t_atom *a)
{
    return atom_getFloat (a);
}

PD_DLL t_float spaghettis_atomGetFloatAtIndex (int n, int argc, t_atom *argv)
{
    return atom_getFloatAtIndex (n, argc, argv);
}

PD_DLL t_symbol *spaghettis_atomGetSymbol (t_atom *a)
{
    return atom_getSymbol (a);
}

PD_DLL t_symbol *spaghettis_atomGetSymbolAtIndex (int n, int argc, t_atom *argv)
{
    return atom_getSymbolAtIndex (n, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_DLL t_initializer *spaghettis_initializerNew (t_initializerfn fn, void *lhs, void *rhs)
{
    return initializer_new (fn, lhs, rhs);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
