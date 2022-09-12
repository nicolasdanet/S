
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Additional inlets. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _hello {
    t_object    x_obj;
    t_float     x_f;                    /* Member to contain the value. */
    t_symbol    *x_s;                   /* Ditto. */
    t_outlet    *x_outlet;
    } t_hello;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void hello_float (t_hello *x, t_float f)
{
    t_atom a[3];
    
    spaghettis_atomSetFloat  (a + 0, f);
    spaghettis_atomSetFloat  (a + 1, x->x_f);
    spaghettis_atomSetSymbol (a + 2, x->x_s);
    
    spaghettis_outletList (x->x_outlet, 3, a);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x = (t_hello *)spaghettis_objectNew (hello_class);
    
    /* Create additional inlets from left to right. */
    
    spaghettis_objectInletNewFloat ((t_object *)x, &x->x_f);
    spaghettis_objectInletNewSymbol ((t_object *)x, &x->x_s);
    
    /* Initialize them with default values. */
    
    x->x_f = 0;
    x->x_s = spaghettis_symbol ("foo");
    
    x->x_outlet = spaghettis_objectOutletNewList ((t_object *)x);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloInlet_setup (t_symbol *s)
{
    t_class *c = NULL;
    
    c = spaghettis_classNew (spaghettis_symbol ("helloInlet"),
            (t_newmethod)hello_new,
            NULL,
            sizeof (t_hello),
            CLASS_BOX);
    
    spaghettis_classAddFloat (c, (t_method)hello_float);

    hello_class = c;
}

PD_STUB void helloInlet_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
