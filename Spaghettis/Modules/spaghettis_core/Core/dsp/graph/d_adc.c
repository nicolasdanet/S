
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_sample *audio_soundIn;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class  *adc_tilde_class;                           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _adc_tilde {
    t_object    x_obj;                                      /* Must be the first. */
    int         x_size;
    int         x_vector[DEVICES_MAXIMUM_CHANNELS];
    } t_adc_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void adc_tilde_dsp (t_adc_tilde *x, t_signal **sp)
{
    t_error err  = PD_ERROR_NONE;
    t_signal **s = sp;
    int i;
        
    for (i = 0; i < x->x_size; i++) {
    //
    int channel = x->x_vector[i] - 1;
    int k = audio_getTotalOfChannelsIn();
    t_signal *t = (*s);
    int n = t->s_vectorSize;
    
    if (n != INTERNAL_BLOCKSIZE) { err = PD_ERROR; dsp_addZeroPerform (t->s_vector, n); }
    else {
    //
    if (channel < 0 || channel >= k) { dsp_addZeroPerform (t->s_vector, INTERNAL_BLOCKSIZE); }
    else {
    //
    t_sample *in = audio_soundIn + (INTERNAL_BLOCKSIZE * channel);
    
    dsp_addCopyPerform (in, t->s_vector, INTERNAL_BLOCKSIZE);
    //
    }
    //
    }
        
    s++;
    //
    }
    
    if (err) { error_invalid (sym_adc__tilde__, sym_signal); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *adc_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    t_adc_tilde *x = (t_adc_tilde *)pd_new (adc_tilde_class);
    int i;
    
    x->x_size = argc ? argc : 2;
    x->x_size = PD_MIN (x->x_size, DEVICES_MAXIMUM_CHANNELS);
    
    if (!argc) { x->x_vector[0] = 1; x->x_vector[1] = 2; }
    else {
        for (i = 0; i < x->x_size; i++) { x->x_vector[i] = (int)atom_getFloatAtIndex (i, argc, argv); }
    }
    
    for (i = 0; i < x->x_size; i++) {
        outlet_newSignal (cast_object (x));
    }
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void adc_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_adc__tilde__,
            (t_newmethod)adc_tilde_new,
            NULL,
            sizeof (t_adc_tilde),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_GIMME,
            A_NULL);
            
    class_addDsp (c, (t_method)adc_tilde_dsp);

    class_setHelpName (c, sym_audio);
    
    adc_tilde_class = c;
}

PD_LOCAL void adc_tilde_destroy (void)
{
    class_free (adc_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------


