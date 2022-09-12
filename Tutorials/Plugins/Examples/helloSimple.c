
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* A simple object. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _hello {
    t_object    x_obj;                             /* MUST be the first. */
    t_buffer    *x_buffer;
    t_outlet    *x_outlet;
    } t_hello;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void hello_post (t_hello *x)
{
    int argc     = spaghettis_bufferGetSize (x->x_buffer);
    t_atom *argv = spaghettis_bufferGetAtoms (x->x_buffer);
    char *s      = spaghettis_atomsToString (argc, argv);
    
    if (!*s) { spaghettis_post (NULL, "I'm empty!"); }
    else {
        spaghettis_post (NULL, s);
    }
    
    spaghettis_memoryFree (s);                      /* Free the memory allocated for the string. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void hello_bang (t_hello *x)
{
    int argc     = spaghettis_bufferGetSize (x->x_buffer);
    t_atom *argv = spaghettis_bufferGetAtoms (x->x_buffer);
    
    spaghettis_outletList (x->x_outlet, argc, argv);
    
    spaghettis_bufferClear (x->x_buffer);
}

static void hello_float (t_hello *x, t_float f)
{
    spaghettis_bufferAppendFloat (x->x_buffer, f);
}

static void hello_symbol (t_hello *x, t_symbol *s)
{
    spaghettis_bufferAppendSymbol (x->x_buffer, s);
}

static void hello_list (t_hello *x, t_symbol *s, int argc, t_atom *argv)
{
    spaghettis_bufferAppendSymbol (x->x_buffer, s); spaghettis_bufferAppend (x->x_buffer, argc, argv);
}

static void hello_anything (t_hello *x, t_symbol *s, int argc, t_atom *argv)
{
    hello_list (x, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x = (t_hello *)spaghettis_objectNew (hello_class);
    
    x->x_buffer = spaghettis_bufferNew();                           /* Allocate a buffer at instantiation. */
    x->x_outlet = spaghettis_objectOutletNewList ((t_object *)x);
    
    return x;
}

static void hello_free (t_hello *x)
{
    spaghettis_bufferFree (x->x_buffer);                            /* Free it at release. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloSimple_setup (t_symbol *s)
{
    t_class *c = NULL;
    
    c = spaghettis_classNew (spaghettis_symbol ("helloSimple"),
            (t_newmethod)hello_new,
            (t_method)hello_free,
            sizeof (t_hello),
            CLASS_BOX);
    
    spaghettis_classAddBang (c, (t_method)hello_bang);
    spaghettis_classAddFloat (c, (t_method)hello_float);
    spaghettis_classAddSymbol (c, (t_method)hello_symbol);
    spaghettis_classAddList (c, (t_method)hello_list);
    spaghettis_classAddAnything (c, (t_method)hello_anything);
    
    /* Register a method without additional parameter. */
    
    spaghettis_classAddMethod (c, (t_method)hello_post, spaghettis_symbol ("post"));

    hello_class = c;
}

PD_STUB void helloSimple_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
