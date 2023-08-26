
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define TOGGLE_SIZE_DEFAULT         18
#define TOGGLE_SIZE_MINIMUM         8
#define TOGGLE_SIZE_MAXIMUM         256

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *toggle_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _toggle {
    t_object    x_obj;              /* MUST be the first. */
    t_float     x_nonZero;
    int         x_state;
    int         x_size;
    t_outlet    *x_outlet;
    } t_toggle;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int toggle_updateState (t_toggle *x, int n, int notify);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_output (t_toggle *x)
{
    outlet_float (x->x_outlet, x->x_state ? x->x_nonZero : 0.0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_bang (t_toggle *x)
{
    toggle_updateState (x, !x->x_state, 1);
    
    toggle_output (x);
}

static void toggle_float (t_toggle *x, t_float f)
{
    toggle_updateState (x, (int)f, 1);
    
    toggle_output (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int toggle_updateState (t_toggle *x, int n, int notify)
{
    int t = (n != 0);
    
    if (x->x_state != t) {
    //
    x->x_state = t;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::State));
        #endif
    }
    
    return 1;
    //
    }
    
    return 0;
}

static void toggle_updateNonZero (t_toggle *x, t_float f, int notify)
{
    if (x->x_nonZero != f && f != 0.0) {
    //
    x->x_nonZero = f;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::NonZero));
        #endif
    }
    //
    }
}

static void toggle_updateSize (t_toggle *x, int n, int notify)
{
    n = PD_CLAMP (n, TOGGLE_SIZE_MINIMUM, TOGGLE_SIZE_MAXIMUM);
    
    if (x->x_size != n) {
    //
    x->x_size = n;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Width));
        #endif
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_size (t_toggle *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { toggle_updateSize (x, (int)atom_getFloatAtIndex (0, argc, argv), 1); }
}

static void toggle_set (t_toggle *x, t_float f)
{
    toggle_updateState (x, f, 1);
}

static void toggle_nonZero (t_toggle *x, t_float f)
{
    toggle_updateNonZero (x, f, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static void toggle_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    t_toggle *x = (t_toggle *)o;
    
    static DelegateCache delegate;
    
    if (t.contains (Tag::State)) {
        group.addParameter (Tag::State,
            NEEDS_TRANS ("State"),
            NEEDS_TRANS ("Toggle state on/off"),
            static_cast<bool> (x->x_state),
            delegate);
    }
    
    if (t.contains (Tag::NonZero)) {
        group.addParameter (Tag::NonZero,
            NEEDS_TRANS ("Non-zero"),
            NEEDS_TRANS ("Value for non-zero state"),
            x->x_nonZero,
            delegate);
    }
    
    if (t.contains (Tag::Width)) {
        group.addParameter (Tag::Width,
            NEEDS_TRANS ("Width"),
            NEEDS_TRANS ("Border size of the object"),
            x->x_size,
            delegate).setRange (juce::Range<int> (TOGGLE_SIZE_MINIMUM, TOGGLE_SIZE_MAXIMUM));
    }
}

static void toggle_functionSetParameters (t_object *o, const core::Group& group)
{
    t_toggle *x = (t_toggle *)o;
    
    jassert (group.hasParameter (Tag::State));
    jassert (group.hasParameter (Tag::NonZero));
    jassert (group.hasParameter (Tag::Width));
    
    toggle_updateNonZero (x, group.getParameter (Tag::NonZero).getValueTyped<t_float>(), 1);
    toggle_updateSize (x, group.getParameter (Tag::Width).getValueTyped<int>(), 1);
    
    if (toggle_updateState (x, group.getParameter (Tag::State).getValueTyped<bool>() ? 1 : 0, 1)) {
        toggle_output (x);
    }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_toggle *x = (t_toggle *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (z));
    buffer_appendFloat (b,  object_getY (z));
    buffer_appendSymbol (b, sym_tgl);
    buffer_appendFloat (b,  x->x_size);
    buffer_appendFloat (b,  x->x_nonZero);
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_state); }
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void toggle_restore (t_toggle *x)
{
    t_toggle *old = (t_toggle *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    /* ??? */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *toggle_new (t_symbol *s, int argc, t_atom *argv)
{
    t_toggle *x = (t_toggle *)pd_new (toggle_class);
    
    int size        = (argc > 1) ? (int)atom_getFloat (argv + 0) : TOGGLE_SIZE_DEFAULT;
    t_float nonZero = (argc > 1) ? atom_getFloat (argv + 1) : (t_float)1.0;
    int state       = (argc > 2) ? (int)atom_getFloat (argv + 2) : 0;

    toggle_updateState (x, state, 0);
    toggle_updateNonZero (x, nonZero, 0);
    toggle_updateSize (x, size, 0);
    
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

static void toggle_free (t_toggle *x)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void toggle_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tgl,
            (t_newmethod)toggle_new,
            (t_method)toggle_free,
            sizeof (t_toggle),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)toggle_bang);
    class_addFloat (c, (t_method)toggle_float);
    
    class_addMethod (c, (t_method)toggle_size,      sym_size,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)toggle_set,       sym_set,        A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)toggle_nonZero,   sym_nonzero,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)toggle_restore,   sym__restore,   A_NULL);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, toggle_functionGetParameters, toggle_functionSetParameters);
    
    #endif
    
    class_setSaveFunction (c, toggle_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);
    
    toggle_class = c;
}

PD_LOCAL void toggle_destroy (void)
{
    class_free (toggle_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
