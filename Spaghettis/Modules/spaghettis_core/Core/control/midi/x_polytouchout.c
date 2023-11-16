
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *polytouchout_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _polytouchout {
    t_object    x_obj;                      /* Must be the first. */
    t_float     x_pitch;
    t_float     x_channel;
    } t_polytouchout;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void polytouchout_float (t_polytouchout *x, t_float n)
{
    outmidi_polyPressure (x->x_channel, x->x_pitch, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *polytouchout_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_polytouchout *x = (t_polytouchout *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_pitch);
    buffer_appendFloat (b, x->x_channel);
    
    return b;
    //
    }
    
    return NULL;
}

static void polytouchout_restore (t_polytouchout *x, t_symbol *s, int argc, t_atom *argv)
{
    t_polytouchout *old = (t_polytouchout *)instance_pendingFetch (cast_object (x));

    x->x_pitch   = old ? old->x_pitch   : atom_getFloatAtIndex (0, argc, argv);
    x->x_channel = old ? old->x_channel : atom_getFloatAtIndex (1, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *polytouchout_new (t_float channel)
{
    t_polytouchout *x = (t_polytouchout *)pd_new (polytouchout_class);
    
    x->x_pitch   = 0;
    x->x_channel = channel;
    
    inlet_newFloat (cast_object (x), &x->x_pitch);
    inlet_newFloat (cast_object (x), &x->x_channel);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void polytouchout_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_polytouchout, 
            (t_newmethod)polytouchout_new,
            NULL,
            sizeof (t_polytouchout),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addFloat (c, (t_method)polytouchout_float);
    
    class_addMethod (c, (t_method)polytouchout_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, polytouchout_functionData);
    class_requirePending (c);

    class_setHelpName (c, sym_pgmout);
    
    polytouchout_class = c;
}

void polytouchout_destroy (void)
{
    class_free (polytouchout_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
