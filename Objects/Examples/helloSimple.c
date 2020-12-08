
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
    char *s = atom_atomsToString (buffer_getSize (x->x_buffer), buffer_getAtoms (x->x_buffer));
    
    if (!*s) { post ("I'm empty!"); }
    else {
        post ("%s", s);
    }
    
    PD_MEMORY_FREE (s);                             /* Free the memory allocated for the string. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void hello_bang (t_hello *x)
{
    outlet_list (x->x_outlet, buffer_getSize (x->x_buffer), buffer_getAtoms (x->x_buffer));
    
    buffer_clear (x->x_buffer);
}

static void hello_float (t_hello *x, t_float f)
{
    buffer_appendFloat (x->x_buffer, f);
}

static void hello_symbol (t_hello *x, t_symbol *s)
{
    buffer_appendSymbol (x->x_buffer, s);
}

static void hello_list (t_hello *x, t_symbol *s, int argc, t_atom *argv)
{
    buffer_appendSymbol (x->x_buffer, s); buffer_append (x->x_buffer, argc, argv);
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
    t_hello *x = (t_hello *)pd_new (hello_class);
    
    x->x_buffer = buffer_new();                             /* Allocate a buffer at instantiation. */
    x->x_outlet = outlet_newList ((t_object *)x);
    
    return x;
}

static void hello_free (t_hello *x)
{
    buffer_free (x->x_buffer);                              /* Free it at release. */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloSimple_setup (t_symbol *s)
{
    t_class *c = NULL;
    
    c = class_new (gensym ("helloSimple"),
            (t_newmethod)hello_new,
            (t_method)hello_free,
            sizeof (t_hello),
            CLASS_BOX,
            A_NULL);
    
    class_addBang (c, (t_method)hello_bang); 
    class_addFloat (c, (t_method)hello_float);
    class_addSymbol (c, (t_method)hello_symbol);
    class_addList (c, (t_method)hello_list);
    class_addAnything (c, (t_method)hello_anything);
    
    /* Register a method without additional parameter. */
    
    class_addMethod (c, (t_method)hello_post, gensym ("post"), A_NULL);

    hello_class = c;
}

PD_STUB void helloSimple_destroy (void)
{
    class_free (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
