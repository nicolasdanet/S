
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *midiports_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _midiports {
    t_object    x_obj;                      /* Must be the first. */
    int         x_out;
    t_buffer    *x_cache;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletMiddle;
    t_outlet    *x_outletRight;
    } t_midiports;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midiports_output (t_midiports *x, int i, t_symbol *s)
{
    outlet_float (x->x_outletMiddle, (t_float)i);
    
    buffer_clear (x->x_cache);
    buffer_appendSymbol (x->x_cache, s);
    buffer_reparseIfContainsWhitespace (x->x_cache);
    
    outlet_list (x->x_outletLeft, buffer_getSize (x->x_cache), buffer_getAtoms (x->x_cache));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midiports_bang (t_midiports *x)
{
    t_devices midi; devices_initAsMidi (&midi); midi_getDevices (&midi);
    
    int i;
    
    if (x->x_out) {
        for (i = 0; i < devices_getOutSize (&midi); i++) {
            midiports_output (x, i, devices_getOutAtIndexAsSymbol (&midi, i));
        }
    } else {
        for (i = 0; i < devices_getInSize (&midi); i++) {
            midiports_output (x, i, devices_getInAtIndexAsSymbol (&midi, i));
        }
    }
}

static void midiports_updated (t_midiports *x)
{
    outlet_bang (x->x_outletRight);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *midiports_new (t_symbol *s)
{
    t_midiports *x = (t_midiports *)pd_new (midiports_class);
    
    x->x_out          = (s == sym_out);
    x->x_cache        = buffer_new();
    x->x_outletLeft   = outlet_newList (cast_object (x));
    x->x_outletMiddle = outlet_newFloat (cast_object (x));
    x->x_outletRight  = outlet_newBang (cast_object (x));
    
    pd_bind (cast_pd (x), sym__midiports);
    
    return x;
}

static void midiports_free (t_midiports *x)
{
    pd_unbind (cast_pd (x), sym__midiports);
    
    buffer_free (x->x_cache);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midiports_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_midiports,
            (t_newmethod)midiports_new,
            (t_method)midiports_free,
            sizeof (t_midiports),
            CLASS_DEFAULT,
            A_DEFSYMBOL,
            A_NULL);
    
    class_addBang (c, (t_method)midiports_bang);
    
    class_addMethod (c, (t_method)midiports_updated, sym__midiports, A_NULL);

    midiports_class = c;
}

void midiports_destroy (void)
{
    class_free (midiports_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
