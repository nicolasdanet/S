
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *ctlin_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _ctlin {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_control;
    t_float     x_channel;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletMiddle;
    t_outlet    *x_outletRight;
    } t_ctlin;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void ctlin_list (t_ctlin *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float control = atom_getFloatAtIndex (0, argc, argv);
    t_float value   = atom_getFloatAtIndex (1, argc, argv);
    t_float channel = atom_getFloatAtIndex (2, argc, argv);
    
    if (x->x_control >= 0.0 && x->x_control != control) { return; }
    if (x->x_channel > 0.0  && x->x_channel != channel) { return; }
    
    if (x->x_outletRight)  { outlet_float (x->x_outletRight, channel);  }
    if (x->x_outletMiddle) { outlet_float (x->x_outletMiddle, control); }
    
    outlet_float (x->x_outletLeft, value);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *ctlin_new (t_symbol *s, int argc, t_atom *argv)
{
    t_ctlin *x = (t_ctlin *)pd_new (ctlin_class);
    
    x->x_control    = argc > 0 ? atom_getFloatAtIndex (0, argc, argv) : (t_float)-1.0;
    x->x_channel    = argc > 1 ? atom_getFloatAtIndex (1, argc, argv) : 0.0;
    x->x_outletLeft = outlet_newFloat (cast_object (x));
    
    if (x->x_control < 0.0)  { x->x_outletMiddle = outlet_newFloat (cast_object (x)); }
    if (x->x_channel <= 0.0) { x->x_outletRight  = outlet_newFloat (cast_object (x)); }
    
    pd_bind (cast_pd (x), sym__ctlin);
    
    if (argc > 2) { warning_unusedArguments (s, argc - 2, argv + 2); }
    
    return x;
}

static void ctlin_free (t_ctlin *x)
{
    pd_unbind (cast_pd (x), sym__ctlin);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void ctlin_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_ctlin,
            (t_newmethod)ctlin_new, 
            (t_method)ctlin_free,
            sizeof (t_ctlin),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)ctlin_list);
    
    ctlin_class = c;
}

PD_LOCAL void ctlin_destroy (void)
{
    class_free (ctlin_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
