
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *int_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _intobject {
    t_object    x_obj;              /* Must be the first. */
    t_float     x_f;
    t_outlet    *x_outlet;
    } t_intobject;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void int_bang (t_intobject *x)
{
    outlet_float (x->x_outlet, trunc (x->x_f));
}

static void int_float (t_intobject *x, t_float f)
{
    x->x_f = f; int_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *int_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_intobject *x = (t_intobject *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_f);
    
    return b;
    //
    }
    
    return NULL;
}

static void int_restore (t_intobject *x, t_float f)
{
    t_intobject *old = (t_intobject *)instance_pendingFetch (cast_object (x));

    x->x_f = old ? old->x_f : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *int_new (t_float f)
{
    t_intobject *x = (t_intobject *)pd_new (int_class);
    
    x->x_f = f;
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_f);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void int_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_int,
            (t_newmethod)int_new,
            NULL,
            sizeof (t_intobject),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addCreator ((t_newmethod)int_new, sym_i, A_DEFFLOAT, A_NULL);
    
    class_addBang (c, (t_method)int_bang);
    class_addFloat (c, (t_method)int_float);
    
    class_addMethod (c, (t_method)int_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, int_functionData);
    class_requirePending (c);
    
    int_class = c;
}

void int_destroy (void)
{
    class_free (int_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
