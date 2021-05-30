
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
    if (spaghettis_symbolHasThingQuiet (sym_s)) {
    //
    t_atom a;
    spaghettis_atomSetSymbol (&a, sym_Turlututu);
    spaghettis_handleMessage (spaghettis_symbolGetThing (sym_s), sym_something, 1, &a);
    //
    }
}

static void hello_something (t_hello *x, t_symbol *s, int argc, t_atom *argv)
{
    char *t = spaghettis_atomsToString (argc, argv);
    
    spaghettis_post (NULL, t);
    
    spaghettis_memoryFree (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x = (t_hello *)spaghettis_objectNew (hello_class);
    
    spaghettis_bind ((t_pd *)x, sym_s);         /* Attach the object to the symbol. */
    
    return x;
}

static void hello_free (t_hello *x)
{
    spaghettis_unbind ((t_pd *)x, sym_s);       /* Detach the object from the symbol. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloBind_setup (t_symbol *s)
{
    t_class *c = NULL;
    
    sym_s         = spaghettis_symbol ("_something_unique_and_unlikely_to_collide");
    sym_something = spaghettis_symbol ("something");
    sym_Turlututu = spaghettis_symbol ("Turlututu");
    
    c = spaghettis_classNew (spaghettis_symbol ("helloBind"),
            (t_newmethod)hello_new,
            (t_method)hello_free,
            sizeof (t_hello),
            CLASS_BOX);
    
    spaghettis_classAddBang (c, (t_method)hello_bang);
    
    /* Register a method with variable parameters. */
    
    spaghettis_classAddMethodWithArguments (c, (t_method)hello_something, sym_something);
    
    hello_class = c;
}

PD_STUB void helloBind_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
