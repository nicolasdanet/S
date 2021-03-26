
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../d_fft.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *rfft_tilde_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _rfft_tilde {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_rfft_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *rfft_tilde_performFlipZero (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)w[3];
    
    while (n--) { t_sample f = *in; --out; *out = - f; *in = 0.0; in++; }
        
    return (w + 4);
}

/* No aliasing. */

static t_int *rfft_tilde_perform (t_int *w)
{
    t_FFTState *x = (t_FFTState *)(w[1]);
    PD_RESTRICTED in = (t_sample *)(w[2]);
    int n = (int)w[3];
    
    fft_realFFT32 (x, n, in);
    
    return (w + 4);
}

static void rfft_tilde_dsp (t_rfft_tilde *x, t_signal **sp)
{
    int n = sp[0]->s_vectorSize;
    
    PD_ASSERT (PD_IS_POWER_2 (n));
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    if (n < FFT_MINIMUM || n > FFT_MAXIMUM) { error_invalid (sym_rfft__tilde__, sym_size); }
    else {
    //
    PD_RESTRICTED in1  = sp[0]->s_vector;
    PD_RESTRICTED out1 = sp[1]->s_vector;
    PD_RESTRICTED out2 = sp[2]->s_vector;
    
    int half = (n >> 1);
    
    t_FFTState *t = fftstate_new (cast_object (x), n);
        
    dsp_addCopyPerform (in1, out1, n);
    
    dsp_add3 (rfft_tilde_perform, t, out1, n);
    dsp_add3 (rfft_tilde_performFlipZero, out1 + half + 1, out2 + half, half - 1);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *rfft_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_rfft_tilde *x = (t_rfft_tilde *)z;
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

static void *rfft_tilde_new (void)
{
    t_rfft_tilde *x = (t_rfft_tilde *)pd_new (rfft_tilde_class);
    
    x->x_outletLeft  = outlet_newSignal (cast_object (x));
    x->x_outletRight = outlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void rfft_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_rfft__tilde__,
            (t_newmethod)rfft_tilde_new,
            NULL,
            sizeof (t_rfft_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
            
    class_addDsp (c, (t_method)rfft_tilde_dsp);
        
    class_setDataFunction (c, rfft_tilde_functionData);
    
    rfft_tilde_class = c;
}

PD_LOCAL void rfft_tilde_destroy (void)
{
    class_free (rfft_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
