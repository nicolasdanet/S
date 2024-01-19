
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "x_array.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *arrayget_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _arrayget {
    t_arrayrange    x_arrayrange;           /* Must be the first. */
    t_outlet        *x_outlet;
    } t_arrayget;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void arrayget_bang (t_arrayget *x)
{
    int start, n; t_garray *a = arrayrange_getRange (&x->x_arrayrange, &start, &n);
    
    if (a) {
    //
    int i; t_atom *t  = NULL;
    
    PD_ATOMS_ALLOCA (t, n);
    
    for (i = 0; i < n; i++) {
        SET_FLOAT (t + i, garray_getFloatAtIndex (a, start + i));
    }
    outlet_list (x->x_outlet, n, t);
    
    PD_ATOMS_FREEA (t, n);
    //
    }
}

static void arrayget_float (t_arrayget *x, t_float f)
{
    arrayrange_setOnset (&x->x_arrayrange, f); arrayget_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *arrayget_new (t_symbol *s, int argc, t_atom *argv)
{
    t_arrayget *x = (t_arrayget *)arrayrange_new (arrayget_class, argc, argv, 0, 1);
    
    if (ARRAYRANGE_GOOD (x)) { x->x_outlet = outlet_newList (cast_object (x)); }
    else {
        error_invalidArguments (cast_object (x), sym_array__space__get, argc, argv);
        pd_free (cast_pd (x)); x = NULL; 
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void arrayget_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_array__space__get,
            (t_newmethod)arrayget_new,
            (t_method)arrayclient_free,
            sizeof (t_arrayget),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addBang (c, (t_method)arrayget_bang);
    class_addFloat (c, (t_method)arrayget_float);
    
    class_addMethod (c, (t_method)arrayrange_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, arrayrange_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_array);
    
    arrayget_class = c;
}

void arrayget_destroy (void)
{
    class_free (arrayget_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
