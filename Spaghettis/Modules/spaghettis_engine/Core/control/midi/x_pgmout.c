
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *pgmout_class;               /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _pgmout {
    t_object    x_obj;                      /* Must be the first. */
    t_float     x_channel;
    } t_pgmout;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void pgmout_float (t_pgmout *x, t_float f)
{
    outmidi_programChange (x->x_channel, (int)f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *pgmout_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_pgmout *x = (t_pgmout *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_channel);
    
    return b;
    //
    }
    
    return NULL;
}

static void pgmout_restore (t_pgmout *x, t_float f)
{
    t_pgmout *old = (t_pgmout *)instance_pendingFetch (cast_object (x));

    x->x_channel = old ? old->x_channel : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *pgmout_new (t_float channel)
{
    t_pgmout *x = (t_pgmout *)pd_new (pgmout_class);
    
    x->x_channel = channel;
    
    inlet_newFloat (cast_object (x), &x->x_channel);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void pgmout_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_pgmout,
            (t_newmethod)pgmout_new,
            NULL,
            sizeof (t_pgmout),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addFloat (c, (t_method)pgmout_float);
    
    class_addMethod (c, (t_method)pgmout_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, pgmout_functionData);
    class_requirePending (c);

    pgmout_class = c;
}

void pgmout_destroy (void)
{
    class_free (pgmout_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
