
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *bendout_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _bendout {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_channel;
    } t_bendout;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bendout_float (t_bendout *x, t_float f)
{
    outmidi_pitchBend (x->x_channel, (int)f + 8192);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *bendout_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_bendout *x = (t_bendout *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_channel);
    
    return b;
    //
    }
    
    return NULL;
}

static void bendout_restore (t_bendout *x, t_float f)
{
    t_bendout *old = (t_bendout *)instance_pendingFetch (cast_object (x));

    x->x_channel = old ? old->x_channel : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *bendout_new (t_float channel)
{
    t_bendout *x = (t_bendout *)pd_new (bendout_class);
    
    x->x_channel = channel;
    
    inlet_newFloat (cast_object (x), &x->x_channel);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void bendout_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_bendout,
            (t_newmethod)bendout_new,
            NULL,
            sizeof (t_bendout),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addFloat (c, (t_method)bendout_float);
    
    class_addMethod (c, (t_method)bendout_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, bendout_functionData);
    class_requirePending (c);

    class_setHelpName (c, sym_pgmout);
    
    bendout_class = c;
}

PD_LOCAL void bendout_destroy (void)
{
    class_free (bendout_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
