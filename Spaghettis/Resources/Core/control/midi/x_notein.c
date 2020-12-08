
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *notein_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _notein {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_channel;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletMiddle;
    t_outlet    *x_outletRight;
    } t_notein;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void notein_list (t_notein *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float pitch    = atom_getFloatAtIndex (0, argc, argv);
    t_float velocity = atom_getFloatAtIndex (1, argc, argv);
    t_float channel  = atom_getFloatAtIndex (2, argc, argv);
    
    if (x->x_channel) {
        if (x->x_channel == channel) { 
            outlet_float (x->x_outletMiddle, velocity);
            outlet_float (x->x_outletLeft, pitch);
        }
        
    } else {
        outlet_float (x->x_outletRight, channel);
        outlet_float (x->x_outletMiddle, velocity);
        outlet_float (x->x_outletLeft, pitch);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *notein_new (t_float f)
{
    t_notein *x = (t_notein *)pd_new (notein_class);
    
    x->x_channel      = PD_ABS (f);
    x->x_outletLeft   = outlet_newFloat (cast_object (x));
    x->x_outletMiddle = outlet_newFloat (cast_object (x));
    
    if (x->x_channel == 0.0) { x->x_outletRight = outlet_newFloat (cast_object (x)); }
    
    pd_bind (cast_pd (x), sym__notein);
    
    return x;
}

static void notein_free (t_notein *x)
{
    pd_unbind (cast_pd (x), sym__notein);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void notein_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_notein, 
            (t_newmethod)notein_new,
            (t_method)notein_free,
            sizeof (t_notein),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_DEFFLOAT,
            A_NULL);
            
    class_addList (c, (t_method)notein_list);
    
    notein_class = c;
}

PD_LOCAL void notein_destroy (void)
{
    class_free (notein_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
