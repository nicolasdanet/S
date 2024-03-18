
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *uzi_class;              /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _uzi {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_count;
    int         x_inhibit;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletMiddle;
    t_outlet    *x_outletRight;
    } t_uzi;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void uzi_proceed (t_uzi *x)
{
    int i, count = PD_MAX (0, x->x_count);
    
    x->x_inhibit = 0;
    
    for (i = 0; i < count; i++) {
    //
    if (x->x_inhibit) { break; }
    else {
        outlet_float (x->x_outletRight, (t_float)i); outlet_bang (x->x_outletLeft);
    }
    //
    }
    
    outlet_bang (x->x_outletMiddle);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void uzi_bang (t_uzi *x)
{
    uzi_proceed (x);
}

static void uzi_float (t_uzi *x, t_float f)
{
    x->x_count = f; uzi_proceed (x);
}

static void uzi_stop (t_uzi *x)
{
    x->x_inhibit = 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *uzi_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_uzi *x    = (t_uzi *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_count);
    
    return b;
    //
    }
    
    return NULL;
}

static void uzi_restore (t_uzi *x, t_float f)
{
    t_uzi *old = (t_uzi *)instance_pendingFetch (cast_object (x));

    x->x_count = old ? old->x_count : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *uzi_new (t_float f)
{
    t_uzi *x = (t_uzi *)pd_new (uzi_class);
    
    x->x_count        = f;
    x->x_outletLeft   = outlet_newBang (cast_object (x));
    x->x_outletMiddle = outlet_newBang (cast_object (x));
    x->x_outletRight  = outlet_newFloat (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void uzi_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_uzi,
            (t_newmethod)uzi_new,
            NULL,
            sizeof (t_uzi),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addBang (c, (t_method)uzi_bang);
    class_addFloat (c, (t_method)uzi_float);
    
    class_addMethod (c, (t_method)uzi_stop,     sym_stop,       A_NULL);
    class_addMethod (c, (t_method)uzi_restore,  sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, uzi_functionData);
    class_requirePending (c);

    uzi_class = c;
}

void uzi_destroy (void)
{
    class_free (uzi_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
