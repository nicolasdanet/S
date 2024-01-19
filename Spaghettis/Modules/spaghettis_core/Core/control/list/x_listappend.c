
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_list.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *listappend_class;   /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void listappend_list (t_listappend *x, t_symbol *s, int argc, t_atom *argv)
{
    t_atom *t = NULL; int count = listinlet_listSize (&x->x_h.lh_listinlet) + argc;
    
    if (!count) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    PD_ATOMS_ALLOCA (t, count);
    
    atom_copyAtoms (argv, argc, t, argc);
    listinlet_copyAtomsUnchecked (&x->x_h.lh_listinlet, t + argc);
    outlet_list (x->x_outlet, count, t);
    
    PD_ATOMS_FREEA (t, count);
    //
    }
}

void listappend_anything (t_listappend *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listappend_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_buffer *listhelper_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_listinlethelper *x = (t_listinlethelper *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    listinlet_listGet (&x->lh_listinlet, b);
    
    return b;
    //
    }
    
    return NULL;
}

void listhelper_restore (t_listinlethelper *x, t_symbol *s, int argc, t_atom *argv)
{
    t_listinlethelper *old = (t_listinlethelper *)instance_pendingFetch (cast_object (x));

    if (old) { listinlet_listSetByCopy (&x->lh_listinlet, &old->lh_listinlet); }
    else {
        listinlet_listSet (&x->lh_listinlet, argc, argv);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *listappend_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listappend *x = (t_listappend *)pd_new (listappend_class);
    
    listinlet_init (&x->x_h.lh_listinlet);
    listinlet_listSet (&x->x_h.lh_listinlet, argc, argv);
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    inlet_new (cast_object (x), cast_pd (&x->x_h.lh_listinlet), NULL, NULL);
    
    return x;
}

static void listappend_free (t_listappend *x)
{
    listinlet_free (&x->x_h.lh_listinlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void listappend_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__append,
            (t_newmethod)listappend_new,
            (t_method)listappend_free,
            sizeof (t_listappend),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listappend_list);
    class_addAnything (c, (t_method)listappend_anything);
    
    class_addMethod (c, (t_method)listhelper_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, listhelper_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, &s_list);
    
    listappend_class = c;
}

void listappend_destroy (void)
{
    class_free (listappend_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

