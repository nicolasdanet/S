
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_array.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *arrayset_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _arrayset {
    t_arrayrange    x_arrayrange;       /* Must be the first. */
    } t_arrayset;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void arrayset_list (t_arrayset *x, t_symbol *s, int argc, t_atom *argv)
{
    int start, n; t_garray *a = arrayrange_getRange (&x->x_arrayrange, &start, &n);
    
    if (a) {
    //
    int i;
    
    n = PD_MIN (argc, n);
    
    for (i = 0; i < n; i++) {
        garray_setFloatAtIndex (a, start + i, atom_getFloatAtIndex (i, argc, argv));
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *arrayset_new (t_symbol *s, int argc, t_atom *argv)
{
    t_arrayset *x = (t_arrayset *)arrayrange_new (arrayset_class, argc, argv, 1, 0);
    
    if (!ARRAYRANGE_GOOD (x)) {
        error_invalidArguments (cast_object (x), sym_array__space__set, argc, argv);
        pd_free (cast_pd (x)); x = NULL; 
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void arrayset_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_array__space__set,
            (t_newmethod)arrayset_new,
            (t_method)arrayclient_free,
            sizeof (t_arrayset),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addList (c, (t_method)arrayset_list);
    
    class_addMethod (c, (t_method)arrayrange_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, arrayrange_functionData);
    class_requirePending (c);
    class_setHelpName (c, sym_array);
    
    arrayset_class = c;
}

void arrayset_destroy (void)
{
    class_free (arrayset_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
