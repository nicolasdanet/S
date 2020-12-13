
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

static t_class *listequal_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void listequal_list (t_listequal *x, t_symbol *s, int argc, t_atom *argv)
{
    int equal = listinlet_listIsEqualTo (&x->x_h.lh_listinlet, argc, argv);
    
    outlet_float (x->x_outlet, equal);
}

static void listequal_anything (t_listequal *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listequal_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listequal_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listequal *x = (t_listequal *)pd_new (listequal_class);
    
    listinlet_init (&x->x_h.lh_listinlet);
    listinlet_listSet (&x->x_h.lh_listinlet, argc, argv);
    
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    inlet_new (cast_object (x), cast_pd (&x->x_h.lh_listinlet), NULL, NULL);
    
    return x;
}

static void listequal_free (t_listequal *x)
{
    listinlet_free (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listequal_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__equal,
            (t_newmethod)listequal_new,
            (t_method)listequal_free,
            sizeof (t_listequal),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listequal_list);
    class_addAnything (c, (t_method)listequal_anything);
    
    class_addMethod (c, (t_method)listhelper_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, listhelper_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    listequal_class = c;
}

PD_LOCAL void listequal_destroy (void)
{
    class_free (listequal_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

