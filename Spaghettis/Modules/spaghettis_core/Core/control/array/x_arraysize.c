
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

static t_class *arraysize_class;                /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _arraysize {
    t_arrayclient   x_arrayclient;              /* Must be the first. */
    t_outlet        *x_outlet;
    } t_arraysize;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void arraysize_bang (t_arraysize *x)
{
    t_garray *a = arrayclient_fetchGraphicArray ((t_arrayclient *)x);
    
    if (a) { outlet_float (x->x_outlet, garray_getSize (a)); }
}

static void arraysize_float (t_arraysize *x, t_float f)
{
    t_garray *a = arrayclient_fetchGraphicArray ((t_arrayclient *)x);
    
    if (a) { garray_resize (a, f); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *arraysize_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_arraysize *x = (t_arraysize *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, arrayclient_getName (&x->x_arrayclient));
    
    return b;
    //
    }
    
    return NULL;
}

static void arraysize_restore (t_arraysize *x, t_symbol *s, int argc, t_atom *argv)
{
    t_arraysize *old = (t_arraysize *)instance_pendingFetch (cast_object (x));

    if (old) { arrayclient_restore (&x->x_arrayclient, &old->x_arrayclient); }
    else {
        arrayclient_setName (&x->x_arrayclient, atom_getSymbolAtIndex (0, argc, argv));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void *arraysize_new (t_symbol *s, int argc, t_atom *argv)
{
    t_arraysize *x = (t_arraysize *)pd_new (arraysize_class);
    
    t_error err = arrayclient_init (&x->x_arrayclient, &argc, &argv);
    
    if (!err) {
    
        if (argc) { warning_unusedArguments (sym_array__space__size, argc, argv); }
        
        inlet_newSymbol (cast_object (x), ARRAYCLIENT_ADDRESS_NAME (&x->x_arrayclient));
     
        x->x_outlet = outlet_newFloat (cast_object (x));
        
    } else {
    
        error_invalidArguments (sym_array__space__size, argc, argv);
        
        pd_free (cast_pd (x)); x = NULL; 
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void arraysize_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_array__space__size,
            (t_newmethod)arraysize_new,
            (t_method)arrayclient_free,
            sizeof (t_arraysize),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addBang (c, (t_method)arraysize_bang);
    class_addFloat (c, (t_method)arraysize_float);
    
    class_addMethod (c, (t_method)arraysize_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, arraysize_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_array);
    
    arraysize_class = c;
}

PD_LOCAL void arraysize_destroy (void)
{
    class_free (arraysize_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
