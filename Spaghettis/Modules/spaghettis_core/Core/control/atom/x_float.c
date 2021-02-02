
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *float_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _floatobject {
    t_object    x_obj;
    t_float     x_f;
    t_outlet    *x_outlet;
    } t_floatobject;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void float_bang (t_floatobject *x)
{
    outlet_float (x->x_outlet, x->x_f);
}

static void float_float (t_floatobject *x, t_float f)
{
    x->x_f = f; float_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *float_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_floatobject *x = (t_floatobject *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_f);
    
    return b;
    //
    }
    
    return NULL;
}

static void float_restore (t_floatobject *x, t_float f)
{
    t_floatobject *old = (t_floatobject *)instance_pendingFetch (cast_object (x));

    x->x_f = old ? old->x_f : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Called by the t_floatmethod of the object maker class. */

static void *float_newByTyped (t_pd *dummy, t_float f)
{
    t_floatobject *x = (t_floatobject *)pd_new (float_class);
    
    x->x_f = f;
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_f);
    
    instance_objectSetNewest (cast_pd (x));
    
    return x;
}

static void *float_newByRegular (t_float f)
{
    return float_newByTyped (NULL, f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void float_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (&s_float,
            (t_newmethod)float_newByTyped,
            NULL,
            sizeof (t_floatobject),
            CLASS_DEFAULT,
            A_FLOAT,
            A_NULL);
            
    class_addCreator ((t_newmethod)float_newByRegular, sym_f, A_DEFFLOAT, A_NULL);
    
    class_addBang (c, (t_method)float_bang);
    class_addFloat (c, (t_method)float_float);
    
    class_addMethod (c, (t_method)float_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, float_functionData);
    class_requirePending (c);

    float_class = c;
}

PD_LOCAL void float_destroy (void)
{
    class_free (float_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
