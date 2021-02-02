
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_list.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listchange_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void listchange_list (t_listchange *x, t_symbol *s, int argc, t_atom *argv)
{
    int changed = !listinlet_listIsEqualTo (&x->x_h.lh_listinlet, argc, argv);
    
    if (changed) { listinlet_listSet (&x->x_h.lh_listinlet, argc, argv); }
    
    outlet_float (x->x_outletRight, changed);
    
    if (changed) { outlet_list (x->x_outlet, argc, argv); }
}

static void listchange_anything (t_listchange *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listchange_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listchange_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listchange *x = (t_listchange *)pd_new (listchange_class);
    
    listinlet_init (&x->x_h.lh_listinlet);
    listinlet_listSet (&x->x_h.lh_listinlet, argc, argv);
    
    x->x_outlet      = outlet_newList (cast_object (x));
    x->x_outletRight = outlet_newFloat (cast_object (x));
    
    inlet_new (cast_object (x), cast_pd (&x->x_h.lh_listinlet), NULL, NULL);
    
    return x;
}

static void listchange_free (t_listchange *x)
{
    listinlet_free (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listchange_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__change,
            (t_newmethod)listchange_new,
            (t_method)listchange_free,
            sizeof (t_listchange),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listchange_list);
    class_addAnything (c, (t_method)listchange_anything);
    
    class_addMethod (c, (t_method)listhelper_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, listhelper_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    listchange_class = c;
}

PD_LOCAL void listchange_destroy (void)
{
    class_free (listchange_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

