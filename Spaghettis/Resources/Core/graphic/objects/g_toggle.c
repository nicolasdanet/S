
/* Copyright (c) 1997-2020 Miller Puckette and others. */

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

static void toggle_size (t_toggle *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) { int n = atom_getFloatAtIndex (0, argc, argv); x->x_size = PD_MAX (n, TOGGLE_SIZE_MINIMUM); }
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
    
    if (old) { }    // -- ???
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

    x->x_size       = PD_MAX (size, TOGGLE_SIZE_MINIMUM);
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
