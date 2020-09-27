
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Bind to symbol. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _hello {
    t_object    x_obj;
    t_symbol    *x_bound;
    } t_hello;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Pointers to symbol can be cached locally (i.g. for efficiency). */

static t_symbol *sym_s;
static t_symbol *sym_something;
static t_symbol *sym_Turlututu;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Send a message to all objects attached. */

static void hello_bang (t_hello *x)
{
    if (symbol_hasThingQuiet (sym_s)) {
    //
    t_atom a; atom_setSymbol (&a, sym_Turlututu); pd_message (symbol_getThing (sym_s), sym_something, 1, &a);
    //
    }
}

static void hello_something (t_hello *x, t_symbol *s, int argc, t_atom *argv)
{
    char *t = atom_atomsToString (argc, argv);
    
    post ("%s!", t);
    
    PD_MEMORY_FREE (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x = (t_hello *)pd_new (hello_class);
    
    pd_bind ((t_pd *)x, sym_s);             /* Attach the object to the symbol. */
    
    return x;
}

static void hello_free (t_hello *x)
{
    pd_unbind ((t_pd *)x, sym_s);           /* Detach the object from the symbol. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloBind_setup (t_symbol *s)
{
    t_class *c = NULL;
    
    sym_s         = gensym ("_something_unique_and_unlikely_to_collide");
    sym_something = gensym ("something");
    sym_Turlututu = gensym ("Turlututu");
    
    c = class_new (gensym ("helloBind"),
            (t_newmethod)hello_new,
            (t_method)hello_free,
            sizeof (t_hello),
            CLASS_BOX,
            A_NULL);
    
    class_addBang (c, (t_method)hello_bang);
    
    /* Register a method with variable parameters. */
    
    class_addMethod (c, (t_method)hello_something, sym_something, A_GIMME, A_NULL);
    
    hello_class = c;
}

PD_STUB void helloBind_destroy (void)
{
    class_free (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
