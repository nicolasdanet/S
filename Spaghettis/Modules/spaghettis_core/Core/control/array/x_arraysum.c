
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

static t_class *arraysum_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _arraysum {
    t_arrayrange    x_arrayrange;       /* Must be the first. */
    t_outlet        *x_outlet;
    } t_arraysum;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void arraysum_bang (t_arraysum *x)
{
    int start, n; t_garray *a = arrayrange_getRange (&x->x_arrayrange, &start, &n);
    
    if (a) {
    //
    int i; double sum = 0.0;

    for (i = 0; i < n; i++) {
        sum += garray_getFloatAtIndex (a, start + i);
    }
    
    outlet_float (x->x_outlet, (t_float)sum);
    //
    }
}

static void arraysum_float (t_arraysum *x, t_float f)
{
    arrayrange_setOnset (&x->x_arrayrange, f); arraysum_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void *arraysum_new (t_symbol *s, int argc, t_atom *argv)
{
    t_arraysum *x = (t_arraysum *)arrayrange_new (arraysum_class, argc, argv, 0, 1);
    
    if (ARRAYRANGE_GOOD (x)) { x->x_outlet = outlet_newFloat (cast_object (x)); }
    else {
        error_invalidArguments (cast_object (x), sym_array__space__sum, argc, argv);
        pd_free (cast_pd (x)); x = NULL; 
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void arraysum_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_array__space__sum,
            (t_newmethod)arraysum_new,
            (t_method)arrayclient_free,
            sizeof (t_arraysum),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addBang (c, (t_method)arraysum_bang);
    class_addFloat (c, (t_method)arraysum_float);
    
    class_addMethod (c, (t_method)arrayrange_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, arrayrange_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_array);
    
    arraysum_class = c;
}

void arraysum_destroy (void)
{
    class_free (arraysum_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
