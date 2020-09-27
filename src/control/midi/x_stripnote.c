
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *stripnote_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _stripnote {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_velocity;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_stripnote;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void stripnote_float (t_stripnote *x, t_float f)
{
    if (x->x_velocity) { 
        outlet_float (x->x_outletRight, x->x_velocity);
        outlet_float (x->x_outletLeft,  f);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *stripnote_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_stripnote *x = (t_stripnote *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_velocity);
    
    return b;
    //
    }
    
    return NULL;
}

static void stripnote_restore (t_stripnote *x, t_float f)
{
    t_stripnote *old = (t_stripnote *)instance_pendingFetch (cast_object (x));

    x->x_velocity = old ? old->x_velocity : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *stripnote_new (void)
{
    t_stripnote *x = (t_stripnote *)pd_new (stripnote_class);
        
    x->x_outletLeft  = outlet_newFloat (cast_object (x));
    x->x_outletRight = outlet_newFloat (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_velocity);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void stripnote_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_stripnote,
            (t_newmethod)stripnote_new,
            NULL,
            sizeof (t_stripnote),
            CLASS_DEFAULT,
            A_NULL);
            
    class_addFloat (c, (t_method)stripnote_float);
    
    class_addMethod (c, (t_method)stripnote_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, stripnote_functionData);
    class_requirePending (c);

    stripnote_class = c;
}

PD_LOCAL void stripnote_destroy (void)
{
    class_free (stripnote_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
