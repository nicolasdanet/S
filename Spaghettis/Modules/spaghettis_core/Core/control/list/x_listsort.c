
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

static t_class *listsort_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _listsort {
    t_object    x_obj;                  /* Must be the first. */
    t_outlet    *x_outlet;
    } t_listsort;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void listsort_list (t_listsort *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!argc) { outlet_list (x->x_outlet, 0, NULL); }
    else {
    //
    t_atom *t = NULL;
    
    PD_ATOMS_ALLOCA (t, argc);
    
    atom_copyAtoms (argv, argc, t, argc); atom_sort (argc, t);
    
    outlet_list (x->x_outlet, argc, t);
    
    PD_ATOMS_FREEA (t, argc);
    //
    }
}

static void listsort_anything (t_listsort *x, t_symbol *s, int argc, t_atom *argv)
{
    utils_anythingToList (cast_pd (x), (t_listmethod)listsort_list, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *listsort_new (t_symbol *s, int argc, t_atom *argv)
{
    t_listsort *x = (t_listsort *)pd_new (listsort_class);
    
    x->x_outlet = outlet_newList (cast_object (x));
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void listsort_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_list__space__sort,
            (t_newmethod)listsort_new,
            NULL,
            sizeof (t_listsort),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)listsort_list);
    class_addAnything (c, (t_method)listsort_anything);
    
    class_setHelpName (c, &s_list);
    
    listsort_class = c;
}

PD_LOCAL void listsort_destroy (void)
{
    class_free (listsort_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

