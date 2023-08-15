
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define ATOM_WIDTH_MAXIMUM      80

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum {
    ATOM_NONE       = 0,
    ATOM_NOTIFY     = 1,
    ATOM_OUTPUT     = 2
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_class *gatom_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct _gatom {
    t_object    a_obj;          /* MUST be the first. */
    t_atom      a_atom;
    int         a_width;
    t_float     a_lowRange;
    t_float     a_highRange;
    t_outlet    *a_outlet;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_bang (t_gatom *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static void gatom_updateValue (t_gatom *x, t_float f, int notify)
{
    if (x->a_lowRange != 0.0 || x->a_highRange != 0.0) { f = PD_CLAMP (f, x->a_lowRange, x->a_highRange); }
    
    if (GET_FLOAT (&x->a_atom) != f) {

        SET_FLOAT (&x->a_atom, f);
        
        if (notify >= ATOM_NOTIFY) { outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Value)); }
        if (notify == ATOM_OUTPUT) { gatom_bang (x); }
    }
}

static void gatom_updateRange (t_gatom *x, t_float low, t_float high, int notify)
{
    const t_float l = x->a_lowRange;
    const t_float h = x->a_highRange;

    x->a_lowRange   = PD_MIN (low, high);
    x->a_highRange  = PD_MAX (low, high);
    
    if (notify) {
        if (l != x->a_lowRange)  { outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Low));  }
        if (h != x->a_highRange) { outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::High)); }
    }
}

static void gatom_updateWidth (t_gatom *x, int width, int notify)
{
    const int n = x->a_width;
    
    x->a_width = PD_CLAMP (width, 0, ATOM_WIDTH_MAXIMUM);
    
    if (notify) {
        if (n != x->a_width) { outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Digits)); }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void gatom_updateValue (t_gatom *x, t_float f, int notify)
{
    if (x->a_lowRange != 0.0 || x->a_highRange != 0.0) { f = PD_CLAMP (f, x->a_lowRange, x->a_highRange); }
    
    SET_FLOAT (&x->a_atom, f);
}

static void gatom_updateRange (t_gatom *x, t_float low, t_float high, int notify)
{
    x->a_lowRange  = PD_MIN (low, high);
    x->a_highRange = PD_MAX (low, high);
}

static void gatom_updateWidth (t_gatom *x, int width, int notify)
{
    x->a_width = PD_CLAMP (width, 0, ATOM_WIDTH_MAXIMUM);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_bang (t_gatom *x)
{
    outlet_float (x->a_outlet, GET_FLOAT (&x->a_atom));
}

static void gatom_float (t_gatom *x, t_float f)
{
    gatom_updateValue (x, f, ATOM_NOTIFY); gatom_bang (x);
}

static void gatom_set (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { gatom_updateValue (x, atom_getFloat (argv), ATOM_NOTIFY); }
}

static void gatom_range (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float low  = atom_getFloatAtIndex (0, argc, argv);
    t_float high = atom_getFloatAtIndex (1, argc, argv);
    
    gatom_updateRange (x, low, high, 1);
    gatom_updateValue (x, GET_FLOAT (&x->a_atom), ATOM_NOTIFY);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_gatom *x = (t_gatom *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_floatatom);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendFloat (b,  x->a_width);
    buffer_appendFloat (b,  x->a_lowRange);
    buffer_appendFloat (b,  x->a_highRange);
    if (SAVED_DEEP (flags)) { buffer_appendAtom (b, &x->a_atom); }
    buffer_appendSemicolon (b);
    
    object_serializeLabel (cast_object (x), b);
    
    object_saveIdentifiers (z, b, flags);
}

static t_buffer *gatom_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);

    return b;
    //
    }
    
    return NULL;
}

/* Encapsulation. */

static void gatom_restore (t_gatom *x)
{
    t_gatom *old = (t_gatom *)instance_pendingFetch (cast_object (x));
    
    if (old) {
    //
    gatom_updateRange (x, old->a_lowRange, old->a_highRange, 1);
    gatom_updateValue (x, GET_FLOAT (&x->a_atom), ATOM_NOTIFY);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static void gatom_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    t_gatom *x = (t_gatom *)o;
    
    static DelegateCache delegate;
    
    if (t.contains (Tag::Value)) {
        group.addParameter (Tag::Value,
            NEEDS_TRANS ("Value"),
            NEEDS_TRANS ("Value of number box"),
            GET_FLOAT (&x->a_atom),
            delegate);
    }
    
    if (t.contains (Tag::Low)) {
        group.addParameter (Tag::Low,
            NEEDS_TRANS ("Low Range"),
            NEEDS_TRANS ("Minimum value"),
            x->a_lowRange,
            delegate);
    }
    
    if (t.contains (Tag::High)) {
        group.addParameter (Tag::High,
            NEEDS_TRANS ("High Range"),
            NEEDS_TRANS ("Maximum value"),
            x->a_highRange,
            delegate);
    }
    
    if (t.contains (Tag::Digits)) {
        group.addParameter (Tag::Digits,
            NEEDS_TRANS ("Digits"),
            NEEDS_TRANS ("Number of digits"),
            x->a_width,
            delegate).setRange (juce::Range<int> (0, ATOM_WIDTH_MAXIMUM));
    }
}

static void gatom_functionSetParameters (t_object *o, const core::Group& group)
{
    t_gatom *x = (t_gatom *)o;
    
    jassert (group.hasParameter (Tag::Value));
    jassert (group.hasParameter (Tag::Low));
    jassert (group.hasParameter (Tag::High));
    jassert (group.hasParameter (Tag::Digits));
    
    const t_float v = group.getParameter (Tag::Value).getValueTyped<t_float>();
    const t_float l = group.getParameter (Tag::Low).getValueTyped<t_float>();
    const t_float h = group.getParameter (Tag::High).getValueTyped<t_float>();
    const int d     = group.getParameter (Tag::Digits).getValueTyped<int>();
    
    gatom_updateWidth (x, d, 1);
    gatom_updateRange (x, l, h, 1);
    gatom_updateValue (x, v, ATOM_OUTPUT);   /* Must be done at last. */
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_makeObjectProceed (t_glist *glist, t_gatom *x, int argc, t_atom *argv)
{
    int width = (int)atom_getFloatAtIndex (2, argc, argv);
    int low   = (int)atom_getFloatAtIndex (3, argc, argv);
    int high  = (int)atom_getFloatAtIndex (4, argc, argv);
    
    t_buffer *t = buffer_new();
    
    /* The buffer content is required only for the GUI inspector. */
    
    buffer_appendSymbol (t, sym_gatom); if (argc > 2) { buffer_append (t, argc - 2, argv + 2); }
    
    object_setBuffer (cast_object (x), t);
    object_setType (cast_object (x),   TYPE_ATOM);
    object_setX (cast_object (x),      atom_getFloatAtIndex (0, argc, argv));
    object_setY (cast_object (x),      atom_getFloatAtIndex (1, argc, argv));
    
    gatom_updateWidth (x, width, 0);
    gatom_updateRange (x, low, high, 0);
    
    if (argc > 5) { gatom_updateValue (x, atom_getFloat (argv + 5), ATOM_NONE); }
    else {
        gatom_updateValue (x, 0.0, ATOM_NONE);
    }
    
    glist_objectAdd (glist, cast_object (x));
}

PD_LOCAL void gatom_makeObject (t_glist *glist, t_symbol *dummy, int argc, t_atom *argv)
{
    t_gatom *x = (t_gatom *)pd_new (gatom_class);
    
    SET_FLOAT (&x->a_atom, 0.0);
    
    x->a_lowRange  = 0;
    x->a_highRange = 0;
    x->a_outlet    = outlet_newFloat (cast_object (x));

    gatom_makeObjectProceed (glist, x, argc, argv);
    
    instance_setBoundA (cast_pd (x));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void gatom_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_gatom,
            NULL,
            NULL,
            sizeof (t_gatom),
            CLASS_DEFAULT,
            A_NULL);
            
    class_addBang (c, (t_method)gatom_bang);
    class_addFloat (c, (t_method)gatom_float);
        
    class_addMethod (c, (t_method)gatom_set,        sym_set,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)gatom_range,      sym_range,      A_GIMME, A_NULL);
    class_addMethod (c, (t_method)gatom_restore,    sym__restore,   A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, gatom_functionGetParameters, gatom_functionSetParameters);
    
    #endif
    
    class_setSaveFunction (c, gatom_functionSave);
    class_setDataFunction (c, gatom_functionData);
    class_requirePending (c);
    
    gatom_class = c;
}

PD_LOCAL void gatom_destroy (void)
{
    class_free (gatom_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
