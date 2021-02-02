
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

static t_class *fft_tilde_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _fft_tilde {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_fft_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *fft_tilde_perform (t_int *w)
{
    t_FFTState *x = (t_FFTState *)(w[1]);
    PD_RESTRICTED in1 = (t_sample *)(w[2]);
    PD_RESTRICTED in2 = (t_sample *)(w[3]);
    int n = (int)w[4];
    
    fft_complexFFT32 (x, n, in1, in2);
    
    return (w + 5);
}

static void fft_tilde_dsp (t_fft_tilde *x, t_signal **sp)
{
    int n = sp[0]->s_vectorSize;
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    if (n < FFT_MINIMUM || n > FFT_MAXIMUM) { error_invalid (sym_fft__tilde__, sym_size); }
    else {
    //
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[0]->s_vector != sp[3]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[3]->s_vector);
    PD_ASSERT (sp[2]->s_vector != sp[3]->s_vector);
    
    t_FFTState *t = fftstate_new (cast_object (x), n);
        
    dsp_addCopyPerform (sp[0]->s_vector, sp[2]->s_vector, n);
    dsp_addCopyPerform (sp[1]->s_vector, sp[3]->s_vector, n);
    
    dsp_add4 (fft_tilde_perform, t, sp[2]->s_vector, sp[3]->s_vector, n);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *fft_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_fft_tilde *x = (t_fft_tilde *)z;
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

static void *fft_tilde_new (void)
{
    t_fft_tilde *x = (t_fft_tilde *)pd_new (fft_tilde_class);
    
    x->x_outletLeft  = outlet_newSignal (cast_object (x));
    x->x_outletRight = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void fft_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_fft__tilde__,
            (t_newmethod)fft_tilde_new,
            NULL,
            sizeof (t_fft_tilde), 
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
            
    class_addDSP (c, (t_method)fft_tilde_dsp);
        
    class_setDataFunction (c, fft_tilde_functionData);
    
    fft_tilde_class = c;
}

PD_LOCAL void fft_tilde_destroy (void)
{
    class_free (fft_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
