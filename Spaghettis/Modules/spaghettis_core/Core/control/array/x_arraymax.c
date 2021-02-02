
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

static t_class *arraymax_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _arraymax {
    t_arrayrange    x_arrayrange;           /* Must be the first. */
    t_outlet        *x_outletLeft;
    t_outlet        *x_outletRight;
    } t_arraymax;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void arraymax_bang (t_arraymax *x)
{
    int start, n; t_garray *a = arrayrange_getRange (&x->x_arrayrange, &start, &n);
    
    if (a) {
    //
    int i;
    
    t_float maxIndex = (t_float)-1.0;
    t_float maxValue = -PD_FLT_MAX;
    
    for (i = 0; i < n; i++) {
        t_float t = garray_getFloatAtIndex (a, start + i);
        if (t > maxValue) { maxValue = t; maxIndex = start + i; }
    }
    
    outlet_float (x->x_outletRight, maxIndex);
    outlet_float (x->x_outletLeft, maxValue);
    //
    }
}

static void arraymax_float (t_arraymax *x, t_float f)
{
    arrayrange_setOnset (&x->x_arrayrange, f); arraymax_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *arraymax_new (t_symbol *s, int argc, t_atom *argv)
{
    t_arraymax *x = (t_arraymax *)arrayrange_new (arraymax_class, argc, argv, 0, 1);
    
    if (ARRAYRANGE_GOOD (x)) {
        x->x_outletLeft  = outlet_newFloat (cast_object (x));
        x->x_outletRight = outlet_newFloat (cast_object (x));

    } else {
        error_invalidArguments (sym_array__space__max, argc, argv);
        pd_free (cast_pd (x)); x = NULL; 
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void arraymax_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_array__space__max,
            (t_newmethod)arraymax_new,
            (t_method)arrayclient_free,
            sizeof (t_arraymax),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)arraymax_bang);
    class_addFloat (c, (t_method)arraymax_float);

    class_addMethod (c, (t_method)arrayrange_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, arrayrange_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_array);
    
    arraymax_class = c;
}

PD_LOCAL void arraymax_destroy (void)
{
    class_free (arraymax_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
