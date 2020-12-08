
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *swap_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _swap {
    t_object    x_obj;              /* Must be the first. */
    t_float     x_f1;
    t_float     x_f2;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_swap;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void swap_bang (t_swap *x)
{
    outlet_float (x->x_outletRight, x->x_f1);
    outlet_float (x->x_outletLeft,  x->x_f2);
}

static void swap_float (t_swap *x, t_float f)
{
    x->x_f1 = f; swap_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *swap_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_swap *x   = (t_swap *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_f1);
    buffer_appendFloat (b, x->x_f2);
    
    return b;
    //
    }
    
    return NULL;
}

static void swap_restore (t_swap *x, t_symbol *s, int argc, t_atom *argv)
{
    t_swap *old = (t_swap *)instance_pendingFetch (cast_object (x));

    t_float f1 = old ? old->x_f1 : atom_getFloatAtIndex (0, argc, argv);
    t_float f2 = old ? old->x_f2 : atom_getFloatAtIndex (1, argc, argv);
    
    x->x_f1 = f1;
    x->x_f2 = f2;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *swap_new (t_float f)
{
    t_swap *x = (t_swap *)pd_new (swap_class);
    
    x->x_f1 = 0;
    x->x_f2 = f;
    x->x_outletLeft  = outlet_newFloat (cast_object (x));
    x->x_outletRight = outlet_newFloat (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_f2);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void swap_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_swap,
            (t_newmethod)swap_new,
            NULL,
            sizeof (t_swap),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);

    class_addBang (c, (t_method)swap_bang);
    class_addFloat (c, (t_method)swap_float);
    
    class_addMethod (c, (t_method)swap_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, swap_functionData);
    class_requirePending (c);

    swap_class = c;
}

PD_LOCAL void swap_destroy (void)
{
    class_free (swap_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
