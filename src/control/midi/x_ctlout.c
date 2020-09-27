
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *ctlout_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _ctlout {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_control;
    t_float     x_channel;
    } t_ctlout;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void ctlout_float (t_ctlout *x, t_float f)
{
    outmidi_controlChange (x->x_channel, (int)x->x_control, (int)f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *ctlout_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_ctlout *x = (t_ctlout *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_control);
    buffer_appendFloat (b, x->x_channel);
    
    return b;
    //
    }
    
    return NULL;
}

static void ctlout_restore (t_ctlout *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ctlout *old = (t_ctlout *)instance_pendingFetch (cast_object (x));

    x->x_control = old ? old->x_control : atom_getFloatAtIndex (0, argc, argv);
    x->x_channel = old ? old->x_channel : atom_getFloatAtIndex (1, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *ctlout_new (t_float control, t_float channel)
{
    t_ctlout *x = (t_ctlout *)pd_new (ctlout_class);
    
    x->x_control = control;
    x->x_channel = channel;
    
    inlet_newFloat (cast_object (x), &x->x_control);
    inlet_newFloat (cast_object (x), &x->x_channel);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void ctlout_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_ctlout,
            (t_newmethod)ctlout_new,
            NULL,
            sizeof (t_ctlout),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addFloat (c, (t_method)ctlout_float);
    
    class_addMethod (c, (t_method)ctlout_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, ctlout_functionData);
    class_requirePending (c);

    ctlout_class = c;
}

PD_LOCAL void ctlout_destroy (void)
{
    class_free (ctlout_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
