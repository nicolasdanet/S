
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *polytouchin_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _polytouchin {
    t_object    x_obj;                      /* Must be the first. */
    t_float     x_channel;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletMiddle;
    t_outlet    *x_outletRight;
    } t_polytouchin;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void polytouchin_list (t_polytouchin *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float pitch   = atom_getFloatAtIndex (0, argc, argv);
    t_float value   = atom_getFloatAtIndex (1, argc, argv);
    t_float channel = atom_getFloatAtIndex (2, argc, argv);
    
    if (x->x_channel) {
        if (x->x_channel == channel) { 
            outlet_float (x->x_outletMiddle, pitch);
            outlet_float (x->x_outletLeft,   value);
        }
        
    } else {
        outlet_float (x->x_outletRight,  channel);
        outlet_float (x->x_outletMiddle, pitch);
        outlet_float (x->x_outletLeft,   value);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *polytouchin_new (t_float f)
{
    t_polytouchin *x = (t_polytouchin *)pd_new (polytouchin_class);
    
    x->x_channel      = f;
    x->x_outletLeft   = outlet_newFloat (cast_object (x));
    x->x_outletMiddle = outlet_newFloat (cast_object (x));
    
    if (!x->x_channel) { x->x_outletRight = outlet_newFloat (cast_object (x)); }
    
    pd_bind (cast_pd (x), sym__polytouchin);
    
    return x;
}

static void polytouchin_free (t_polytouchin *x)
{
    pd_unbind (cast_pd (x), sym__polytouchin);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void polytouchin_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_polytouchin,
            (t_newmethod)polytouchin_new,
            (t_method)polytouchin_free,
            sizeof (t_polytouchin),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_DEFFLOAT,
            A_NULL);
            
    class_addList (c, (t_method)polytouchin_list);
    
    class_setHelpName (c, sym_pgmin);
    
    polytouchin_class = c;
}

void polytouchin_destroy (void)
{
    class_free (polytouchin_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
