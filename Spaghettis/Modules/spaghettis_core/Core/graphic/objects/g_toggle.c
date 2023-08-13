
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define TOGGLE_SIZE_DEFAULT         15
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
    t_float     x_state;
    t_float     x_nonZero;
    int         x_size;
    t_outlet    *x_outlet;
    } t_toggle;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_set (t_toggle *, t_float);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_bang (t_toggle *x)
{
    toggle_set (x, (x->x_state == 0.0) ? x->x_nonZero : 0.0); outlet_float (x->x_outlet, x->x_state);
}

static void toggle_float (t_toggle *x, t_float f)
{
    toggle_set (x, f); outlet_float (x->x_outlet, x->x_state);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_updateSize (t_toggle *x, int n)
{
    n = PD_CLAMP (n, TOGGLE_SIZE_MINIMUM, TOGGLE_SIZE_MAXIMUM);
    
    if (x->x_size != n) {
    //
    x->x_size = n;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_size (t_toggle *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { toggle_updateSize (x, (int)atom_getFloatAtIndex (0, argc, argv)); }
}

static void toggle_set (t_toggle *x, t_float f)
{
    x->x_state = f;
}

static void toggle_nonZero (t_toggle *x, t_float f)
{
    if (f != 0.0) { x->x_nonZero = f; }
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
            static_cast<bool> (x->x_state != 0.0),
            delegate);
    }
    
    if (t.contains (Tag::NonZero)) {
        group.addParameter (Tag::NonZero,
            NEEDS_TRANS ("Non-zero"),
            NEEDS_TRANS ("Value used for non-zero state"),
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
    t_float state   = (argc > 2) ? atom_getFloat (argv + 2) : 0.0;

    x->x_size       = PD_CLAMP (size, TOGGLE_SIZE_MINIMUM, TOGGLE_SIZE_MAXIMUM);
    x->x_state      = (state != 0.0) ? nonZero : 0.0;
    x->x_nonZero    = nonZero;
    x->x_outlet     = outlet_newFloat (cast_object (x));
    
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
