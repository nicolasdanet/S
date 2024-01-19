
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *noteout_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _noteout {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_velocity;
    t_float     x_channel;
    } t_noteout;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void noteout_float (t_noteout *x, t_float f)
{
    outmidi_noteOn (x->x_channel, (int)f, (int)x->x_velocity);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *noteout_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_noteout *x = (t_noteout *)z;
    t_buffer *b  = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_velocity);
    buffer_appendFloat (b, x->x_channel);
    
    return b;
    //
    }
    
    return NULL;
}

static void noteout_restore (t_noteout *x, t_symbol *s, int argc, t_atom *argv)
{
    t_noteout *old = (t_noteout *)instance_pendingFetch (cast_object (x));

    x->x_velocity = old ? old->x_velocity : atom_getFloatAtIndex (0, argc, argv);
    x->x_channel  = old ? old->x_channel  : atom_getFloatAtIndex (1, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *noteout_new (t_float channel)
{
    t_noteout *x = (t_noteout *)pd_new (noteout_class);
    
    x->x_velocity = 0;
    x->x_channel  = channel;
    
    inlet_newFloat (cast_object (x), &x->x_velocity);
    inlet_newFloat (cast_object (x), &x->x_channel);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void noteout_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_noteout,
            (t_newmethod)noteout_new,
            NULL,
            sizeof (t_noteout),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
        
    class_addFloat (c, (t_method)noteout_float);
    
    class_addMethod (c, (t_method)noteout_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, noteout_functionData);
    class_requirePending (c);

    noteout_class = c;
}

void noteout_destroy (void)
{
    class_free (noteout_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

