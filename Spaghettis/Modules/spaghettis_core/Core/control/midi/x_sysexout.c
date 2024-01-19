
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *sysexout_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _sysexout {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_port;
    t_buffer    *x_cache;
    } t_sysexout;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int sysexout_isMalformed (int argc, t_atom *argv)
{
    int i;
    
    for (i = 0; i < argc; i++) {
    //
    if (!IS_FLOAT (argv + i) || GET_FLOAT (argv + i) < 0 || GET_FLOAT (argv + i) > 127) { return 1; }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void sysexout_list (t_sysexout *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    if (sysexout_isMalformed (argc, argv)) { error_invalid (cast_object (x), sym_sysexout, sym_data); }
    else {
    //
    buffer_clear (x->x_cache);
    buffer_appendFloat (x->x_cache, MIDI_STARTSYSEX);
    buffer_append (x->x_cache, argc, argv);
    buffer_appendFloat (x->x_cache, MIDI_ENDSYSEX);
    
    outmidi_sysex (x->x_port, buffer_getSize (x->x_cache), buffer_getAtoms (x->x_cache));
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *sysexout_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_sysexout *x = (t_sysexout *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, x->x_port);
    
    return b;
    //
    }
    
    return NULL;
}

static void sysexout_restore (t_sysexout *x, t_float f)
{
    t_sysexout *old = (t_sysexout *)instance_pendingFetch (cast_object (x));

    x->x_port = old ? old->x_port : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *sysexout_new (t_float port)
{
    t_sysexout *x = (t_sysexout *)pd_new (sysexout_class);
    
    x->x_port  = port;
    x->x_cache = buffer_new();
    
    inlet_newFloat (cast_object (x), &x->x_port);
    
    return x;
}

static void sysexout_free (t_sysexout *x)
{
    buffer_free (x->x_cache);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void sysexout_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_sysexout,
            (t_newmethod)sysexout_new,
            (t_method)sysexout_free,
            sizeof (t_sysexout),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addList (c, (t_method)sysexout_list);
    
    class_addMethod (c, (t_method)sysexout_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, sysexout_functionData);
    class_requirePending (c);
    
    class_setHelpName (c, sym_sysexin);
    
    sysexout_class = c;
}

void sysexout_destroy (void)
{
    class_free (sysexout_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
