
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

static t_class *arrayrandom_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _arrayrandom {
    t_arrayrange    x_arrayrange;           /* Must be the first. */
    t_rand48        x_state;
    t_outlet        *x_outlet;
    } t_arrayrandom;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void arrayrandom_bang (t_arrayrandom *x)
{
    t_float t, f = PD_RAND48_DOUBLE (x->x_state);
    
    if (!arrayrange_quantile (&x->x_arrayrange, f, &t)) { outlet_float (x->x_outlet, t); }
}

static void arrayrandom_float (t_arrayrandom *x, t_float f)
{
    arrayrange_setOnset (&x->x_arrayrange, f); arrayrandom_bang (x);
}

static void arrayrandom_seed (t_arrayrandom *x, t_float f)
{
    x->x_state = (t_rand48)f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *arrayrandom_new (t_symbol *s, int argc, t_atom *argv)
{
    t_arrayrandom *x = (t_arrayrandom *)arrayrange_new (arrayrandom_class, argc, argv, 0, 1);
    
    if (ARRAYRANGE_GOOD (x)) {
        PD_RAND48_INIT (x->x_state);
        x->x_outlet = outlet_newFloat (cast_object (x));
        
    } else {
        error_invalidArguments (cast_object (x), sym_array__space__random, argc, argv);
        pd_free (cast_pd (x)); x = NULL; 
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void arrayrandom_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_array__space__random,
            (t_newmethod)arrayrandom_new,
            (t_method)arrayclient_free,
            sizeof (t_arrayrandom),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)arrayrandom_bang);
    class_addFloat (c, (t_method)arrayrandom_float);
    
    class_addMethod (c, (t_method)arrayrandom_seed,     sym_seed,       A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)arrayrange_restore,   sym__restore,   A_GIMME, A_NULL);

    class_setDataFunction (c, arrayrange_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_array);
    
    arrayrandom_class = c;
}

PD_LOCAL void arrayrandom_destroy (void)
{
    class_free (arrayrandom_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
