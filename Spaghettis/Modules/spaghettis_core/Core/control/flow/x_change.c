
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *change_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _change {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_f;
    t_outlet    *x_outlet;
    } t_change;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void change_bang (t_change *x)
{
    outlet_float (x->x_outlet, x->x_f);
}

static void change_float (t_change *x, t_float f)
{
    if (f != x->x_f) { x->x_f = f; change_bang (x); }
}

static void change_set (t_change *x, t_float f)
{
    x->x_f = f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *change_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_change *x = (t_change *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_f);
    
    return b;
    //
    }
    
    return NULL;
}

static void change_restore (t_change *x, t_float f)
{
    t_change *old = (t_change *)instance_pendingFetch (cast_object (x));

    x->x_f = old ? old->x_f : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *change_new (t_float f)
{
    t_change *x = (t_change *)pd_new (change_class);
    
    x->x_f = f;
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void change_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_change,
            (t_newmethod)change_new,
            NULL,
            sizeof (t_change),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addBang (c, (t_method)change_bang);
    class_addFloat (c, (t_method)change_float);
    
    class_addMethod (c, (t_method)change_set,       sym_set,        A_DEFFLOAT, A_NULL);
    class_addMethod (c, (t_method)change_restore,   sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, change_functionData);
    class_requirePending (c);

    change_class = c;
}

void change_destroy (void)
{
    class_free (change_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
