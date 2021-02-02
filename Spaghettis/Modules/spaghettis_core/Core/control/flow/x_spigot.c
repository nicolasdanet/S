
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *spigot_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _spigot {
    t_object    x_obj;              /* Must be the first. */
    t_float     x_state;
    t_outlet    *x_outlet;
    } t_spigot;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void spigot_bang (t_spigot *x)
{
    if (x->x_state) { outlet_bang (x->x_outlet); }
}

static void spigot_float (t_spigot *x, t_float f)
{
    if (x->x_state) { outlet_float (x->x_outlet, f); }
}

static void spigot_symbol (t_spigot *x, t_symbol *s)
{
    if (x->x_state) { outlet_symbol (x->x_outlet, s); }
}

static void spigot_list (t_spigot *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->x_state) { outlet_list (x->x_outlet, argc, argv); }
}

static void spigot_anything (t_spigot *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->x_state) { outlet_anything (x->x_outlet, s, argc, argv); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *spigot_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_spigot *x = (t_spigot *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_state);
    
    return b;
    //
    }
    
    return NULL;
}

static void spigot_restore (t_spigot *x, t_float f)
{
    t_spigot *old = (t_spigot *)instance_pendingFetch (cast_object (x));

    x->x_state = old ? old->x_state : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *spigot_new (t_float f)
{
    t_spigot *x = (t_spigot *)pd_new (spigot_class);
    
    x->x_state  = f;
    x->x_outlet = outlet_newMixed (cast_object (x));
        
    inlet_newFloat (cast_object (x), &x->x_state);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void spigot_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_spigot,
            (t_newmethod)spigot_new,
            NULL,
            sizeof (t_spigot),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addBang (c, (t_method)spigot_bang);
    class_addFloat (c, (t_method)spigot_float);
    class_addSymbol (c, (t_method)spigot_symbol);
    class_addList (c, (t_method)spigot_list);
    class_addAnything (c, (t_method)spigot_anything);
    
    class_addMethod (c, (t_method)spigot_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, spigot_functionData);
    class_requirePending (c);

    spigot_class = c;
}

PD_LOCAL void spigot_destroy (void)
{
    class_free (spigot_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
