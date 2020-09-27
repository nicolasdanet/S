
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *touchout_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _touchout {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_channel;
    } t_touchout;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void touchout_float (t_touchout *x, t_float f)
{
    outmidi_afterTouch (x->x_channel, (int)f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *touchout_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_touchout *x = (t_touchout *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_channel);
    
    return b;
    //
    }
    
    return NULL;
}

static void touchout_restore (t_touchout *x, t_float f)
{
    t_touchout *old = (t_touchout *)instance_pendingFetch (cast_object (x));

    x->x_channel = old ? old->x_channel : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *touchout_new (t_float channel)
{
    t_touchout *x = (t_touchout *)pd_new (touchout_class);
    
    x->x_channel = channel;
    
    inlet_newFloat (cast_object (x), &x->x_channel);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void touchout_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_touchout, 
            (t_newmethod)touchout_new,
            NULL,
            sizeof (t_touchout),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addFloat (c, (t_method)touchout_float);
    
    class_addMethod (c, (t_method)touchout_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, touchout_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_pgmout);
    
    touchout_class = c;
}

PD_LOCAL void touchout_destroy (void)
{
    class_free (touchout_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
