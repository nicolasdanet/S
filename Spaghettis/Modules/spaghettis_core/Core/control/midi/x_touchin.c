
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *touchin_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _touchin {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_channel;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_touchin;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void touchin_list (t_touchin *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float value   = atom_getFloatAtIndex (0, argc, argv);
    t_float channel = atom_getFloatAtIndex (1, argc, argv);
    
    if (x->x_channel) {
        if (x->x_channel == channel) { outlet_float (x->x_outletLeft, value); }
        
    } else {
        outlet_float (x->x_outletRight, channel);
        outlet_float (x->x_outletLeft, value);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *touchin_new (t_float f)
{
    t_touchin *x = (t_touchin *)pd_new (touchin_class);
    
    x->x_channel    = f;
    x->x_outletLeft = outlet_newFloat (cast_object (x));
    
    if (!x->x_channel) { x->x_outletRight = outlet_newFloat (cast_object (x)); }
    
    pd_bind (cast_pd (x), sym__touchin);
    
    return x;
}

static void touchin_free (t_touchin *x)
{
    pd_unbind (cast_pd (x), sym__touchin);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void touchin_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_touchin,
            (t_newmethod)touchin_new,
            (t_method)touchin_free,
            sizeof (t_touchin),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_DEFFLOAT,
            A_NULL);
            
    class_addList (c, (t_method)touchin_list);
    
    class_setHelpName (c, sym_pgmin);
    
    touchin_class = c;
}

void touchin_destroy (void)
{
    class_free (touchin_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
