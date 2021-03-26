
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
/* Original objects (if marked to require pending) are momentary stored. */
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

static t_symbol *hello_s;       /* Pointers to symbols can be cached for efficiency. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void hello_bang (t_hello *x)
{
    spaghettis_outletFloat (x->x_outlet, x->x_f);
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

    if (x->x_keep || spaghettis_objectFlagIsUndoOrEncaspulate (z, flags)) {
    //
    t_buffer *b = spaghettis_bufferNew();
    
    /* Must be labelled according to the method used for data recovery. */
    /* It can be a public method such as set or a private one. */
    /* In the private case to avoid to collide it must be this one. */
    
    spaghettis_bufferAppendSymbol (b, hello_s);
    spaghettis_bufferAppendFloat (b, x->x_f);
    
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
    
    t_hello *old = (t_hello *)spaghettis_objectGetTemporary ((t_object *)x);

    /* Use value stored in data function in other cases. */
    
    x->x_f = old ? old->x_f : spaghettis_atomGetFloatAtIndex (0, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *hello_new (void)
{
    t_hello *x  = (t_hello *)spaghettis_objectNew (hello_class);
    
    x->x_outlet = spaghettis_objectOutletNewFloat ((t_object *)x);
    
    /* Always stored in patch in this example. */
    
    x->x_keep = 1;
        
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_STUB void helloData_setup (t_symbol *s)
{
    t_class *c = NULL;
    
    c = spaghettis_classNew (spaghettis_symbol ("helloData"),
            (t_newmethod)hello_new,
            NULL,
            sizeof (t_hello),
            CLASS_BOX);
    
    hello_s = spaghettis_getRestoreSymbol();

    spaghettis_classAddBang (c, (t_method)hello_bang);
    spaghettis_classAddFloat (c, (t_method)hello_float);
    
    spaghettis_classAddMethodWithArguments (c, (t_method)hello_restore, hello_s);

    spaghettis_classSetDataFunction (c, hello_functionData);
    spaghettis_classSetDismissFunction (c, hello_functionDismiss);
    
    /* Indicate that an object need to be cached while encapsulated. */
    /* It is valid only for non DSP objects. */
    /* DSP objects use another mechanism. */
    /* Note that DSP objects are cached by default. */
    
    spaghettis_classRequirePending (c);
    
    hello_class = c;
}

PD_STUB void helloData_destroy (void)
{
    spaghettis_classFree (hello_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
