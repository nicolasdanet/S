
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *random_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _random {
    t_object    x_obj;              /* Must be the first. */
    t_float     x_range;
    t_rand48    x_state;
    t_outlet    *x_outlet;
    } t_random;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static double random_getNextFloat (t_random *x)
{
    return PD_RAND48_DOUBLE (x->x_state);
}

static int random_getNextInteger (t_random *x, int n)
{
    int k = (int)(n * random_getNextFloat (x));
    
    PD_ASSERT (k < n);
    
    return k;
}

static t_float random_getNext (t_random *x, int n)
{
    if (n > 0) { return (t_float)random_getNextInteger (x, n); }
    else {
        return (t_float)random_getNextFloat (x);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void random_bang (t_random *x)
{
    outlet_float (x->x_outlet, random_getNext (x, PD_MAX (0, (int)x->x_range)));
}

static void random_float (t_random *x, t_float f)
{
    int i, argc  = PD_MAX (1, (int)f);
    t_atom *argv = NULL;
    
    PD_ATOMS_ALLOCA (argv, argc);
    
    for (i = 0; i < argc; i++) { SET_FLOAT (argv + i, random_getNext (x, PD_MAX (0, (int)x->x_range))); }
    
    outlet_list (x->x_outlet, argc, argv);
    
    PD_ATOMS_FREEA (argv, argc);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void random_seed (t_random *x, t_float f)
{
    x->x_state = (t_rand48)f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *random_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_random *x = (t_random *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_range);
    
    return b;
    //
    }
    
    return NULL;
}

static void random_restore (t_random *x, t_float f)
{
    t_random *old = (t_random *)instance_pendingFetch (cast_object (x));

    if (old) { x->x_range = old->x_range; x->x_state = old->x_state; }
    else {
        x->x_range = f;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *random_new (t_float f)
{
    t_random *x = (t_random *)pd_new (random_class);
    
    x->x_range  = f;
    x->x_outlet = outlet_newMixed (cast_object (x));
    
    PD_RAND48_INIT (x->x_state);
    
    inlet_newFloat (cast_object (x), &x->x_range);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void random_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_random,
            (t_newmethod)random_new,
            NULL,
            sizeof (t_random),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addBang (c, (t_method)random_bang);
    class_addFloat (c, (t_method)random_float);
    
    class_addMethod (c, (t_method)random_seed,      sym_seed,       A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)random_restore,   sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, random_functionData);
    class_requirePending (c);

    random_class = c;
}

void random_destroy (void)
{
    class_free (random_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
