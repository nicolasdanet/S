
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Schedule a task in time. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _hello {
    t_object    x_obj;
    t_outlet    *x_outlet;
    t_clock     *x_clock;
    } t_hello;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* The task to perform. */

static void hello_task (t_hello *x)
{
    static int counter = 0; spaghettis_outletFloat (x->x_outlet, counter++);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void hello_bang (t_hello *x)
{
    /* Schedule the task. */
    
    spaghettis_clockDelay (x->x_clock, 1000.0);
    
    /* Note that it is allowed to schedule the clock inside a DSP perform. */
    /* In fact, it IS the proper way for DSP to interact with control flow. */
    /* It's also the only way to manage communication from a custom thread. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x  = (t_hello *)spaghettis_objectNew (hello_class);
    
    x->x_outlet = spaghettis_objectOutletNewFloat ((t_object *)x);
    
    /* Register the clock with the task to attach. */
    
    x->x_clock = spaghettis_clockNew ((void *)x, (t_method)hello_task);
    
    return x;
}

static void hello_free (t_hello *x)
{
    spaghettis_clockFree (x->x_clock);    /* Unregister the clock. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloClock_setup (t_symbol *s)
{
    t_class *c = NULL;
    
    c = spaghettis_classNew (spaghettis_symbol ("helloClock"),
            (t_newmethod)hello_new,
            (t_method)hello_free,
            sizeof (t_hello),
            CLASS_BOX);
    
    spaghettis_classAddBang (c, (t_method)hello_bang);

    hello_class = c;
}

PD_STUB void helloClock_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
