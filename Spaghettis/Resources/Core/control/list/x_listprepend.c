
/* Copyright (c) 1997-2020 Miller Puckette and others. */

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

static t_class *listprepend_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void listprepend_list (t_listprepend *x, t_symbol *s, int argc, t_atom *argv)
{
    t_atom *t = NULL; int count = listinlet_listSize (&x->x_h.lh_listinlet) + argc;
    
    if (!count) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    PD_ATOMS_ALLOCA (t, count);
    
    atom_copyAtoms (argv, argc, t + listinlet_listSize (&x->x_h.lh_listinlet), argc);
    listinlet_copyAtomsUnchecked (&x->x_h.lh_listinlet, t);
    outlet_list (x->x_outlet, count, t);
    
    PD_ATOMS_FREEA (t, count);
    //
    }
}

static void listprepend_anything (t_listprepend *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listprepend_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listprepend_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listprepend *x = (t_listprepend *)pd_new (listprepend_class);
    
    listinlet_init (&x->x_h.lh_listinlet);
    listinlet_listSet (&x->x_h.lh_listinlet, argc, argv);
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    inlet_new (cast_object (x), cast_pd (&x->x_h.lh_listinlet), NULL, NULL);
    
    return x;
}

static void listprepend_free (t_listprepend *x)
{
    listinlet_free (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listprepend_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__prepend,
            (t_newmethod)listprepend_new,
            (t_method)listprepend_free,
            sizeof (t_listprepend),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listprepend_list);
    class_addAnything (c, (t_method)listprepend_anything);
    
    class_addMethod (c, (t_method)listhelper_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, listhelper_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    listprepend_class = c;
}

PD_LOCAL void listprepend_destroy (void)
{
    class_free (listprepend_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

