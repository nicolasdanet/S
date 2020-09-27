
/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Save data into the patch (also manage undo and encapsulation properly). */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* The data function is called to serialize an object. */
/* It is used for instance for saving and undoing. */
/* During encapsulation objects are deleted then recreated. */
/* It is similar to cut/paste operation. */
/* Original objects (marked to require pending) are momentary stored. */
/* Thus you can grab (and swap) internal states before deletion. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _hello {
    t_object    x_obj;
    int         x_keep;
    t_float     x_f;
    t_outlet    *x_outlet;
    } t_hello;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class  *hello_class;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_symbol *hello__restore;        /* Pointers to symbols can be cached for efficiency. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void hello_bang (t_hello *x)
{
    outlet_float (x->x_outlet, x->x_f);
}

static void hello_float (t_hello *x, t_float f)
{
    x->x_f = f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *hello_functionData (t_object *z, int flags)
{
    t_hello *x = (t_hello *)z;

    if (x->x_keep || object_isUndoOrEncaspulate (z, flags)) {
    //
    t_buffer *b = buffer_new();
    
    /* Must be labelled according to the method used for data recovery. */
    /* It can be a public method such as set or a private one (starting with an underscore). */
    /* In the private case to avoid to collide it must be "_restore". */
    
    buffer_appendSymbol (b, hello__restore); buffer_appendFloat (b, x->x_f);
    
    return b;       /* Must NOT be freed. */
    //
    }
    
    return NULL;    /* To discard. */
}

static void hello_functionDismiss (t_object *z)
{
    /* The dismiss function is called just after object is put pending. */
    /* You can release here things that can't wait. */
    /* Take care to not doing it twice later in the free function. */

    t_hello *x = (t_hello *)z;
    
    (void)x;        /* We don't care. */
}

/* The method that will be called to restore/set internal states. */

static void hello_restore (t_hello *x, t_symbol *s, int argc, t_atom *argv)
{
    /* Fetch old object during encapsulation (return NULL otherwise). */
    /* Notice that oldest object is catched with a pointer to the newest object. */
    /* Thus you don't need to cache anything. */
    /* You must NOT cache the returned pointer to be used later. */
    
    t_hello *old = (t_hello *)instance_objectGetTemporary ((t_object *)x);

    /* Use value stored in data function in other cases. */
    
    x->x_f = old ? old->x_f : atom_getFloatAtIndex (0, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x  = (t_hello *)pd_new (hello_class);
    
    x->x_keep   = 1;                                    /* Always stored in patch in this example. */
    x->x_outlet = outlet_newFloat ((t_object *)x);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloData_setup (t_symbol *s)
{
    t_class *c = NULL;
    
    c = class_new (gensym ("helloData"),
            (t_newmethod)hello_new,
            NULL,
            sizeof (t_hello),
            CLASS_BOX,
            A_NULL);
    
    hello__restore = gensym ("_restore");

    class_addBang (c, (t_method)hello_bang);
    class_addFloat (c, (t_method)hello_float);
    
    class_addMethod (c, (t_method)hello_restore, hello__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, hello_functionData);
    class_setDismissFunction (c, hello_functionDismiss);
    
    /* Indicate that an object need to be cached while encapsulated. */
    /* It is valid only for non DSP objects. */
    /* DSP objects use another mechanism. */
    /* Note that DSP objects are cached by default. */
    
    class_requirePending (c);
    
    hello_class = c;
}

PD_STUB void helloData_destroy (void)
{
    class_free (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
