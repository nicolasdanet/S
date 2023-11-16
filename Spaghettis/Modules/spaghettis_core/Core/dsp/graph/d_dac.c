
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_sample *audio_soundOut;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class  *dac_tilde_class;                               /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _dac_tilde {
    t_object    x_obj;                                          /* Must be the first. */
    int         x_size;
    int         x_vector[DEVICES_MAXIMUM_CHANNELS];
    } t_dac_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://lists.puredata.info/pipermail/pd-list/2017-10/120755.html > */
/* < https://lists.puredata.info/pipermail/pd-list/2011-09/091004.html > */

static void dac_tilde_dsp (t_dac_tilde *x, t_signal **sp)
{
    t_error err  = PD_ERROR_NONE;
    t_signal **s = sp;
    int i;
        
    for (i = 0; i < x->x_size; i++) {
    //
    int channel = x->x_vector[i] - 1;
    int k = audio_getTotalOfChannelsOut();
    t_signal *t = (*s);
    int n = t->s_vectorSize;
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));
    
    if (n != INTERNAL_BLOCKSIZE) { err = PD_ERROR; }
    else {
    //
    if (channel >= 0 && channel < k) {
    //
    t_sample *out = audio_soundOut + (INTERNAL_BLOCKSIZE * channel);
    
    dsp_addPlusPerformAliased (out, t->s_vector, out, INTERNAL_BLOCKSIZE);
    //
    }
    //
    }
        
    s++;
    //
    }
    
    if (err) { error_invalid (cast_object (x), sym_dac__tilde__, sym_signal); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *dac_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_dac_tilde *x = (t_dac_tilde *)z;
    t_buffer *b = buffer_new();

    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *dac_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    t_dac_tilde *x = (t_dac_tilde *)pd_new (dac_tilde_class);
    int i;
    
    x->x_size = argc ? argc : 2;
    x->x_size = PD_MIN (x->x_size, DEVICES_MAXIMUM_CHANNELS);
    
    if (!argc) { x->x_vector[0] = 1; x->x_vector[1] = 2; }
    else {
        for (i = 0; i < x->x_size; i++) { x->x_vector[i] = (int)atom_getFloatAtIndex (i, argc, argv); }
    }
    
    for (i = 1; i < x->x_size; i++) { inlet_newSignal (cast_object (x)); }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void dac_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_dac__tilde__,
            (t_newmethod)dac_tilde_new,
            NULL,
            sizeof (t_dac_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_GIMME,
            A_NULL);
        
    class_addDsp (c, (t_method)dac_tilde_dsp);
    
    class_setDataFunction (c, dac_tilde_functionData);
    class_setHelpName (c, sym_audio);
    
    dac_tilde_class = c;
}

void dac_tilde_destroy (void)
{
    class_free (dac_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
