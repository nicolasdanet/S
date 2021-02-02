
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

static t_class *rifft_tilde_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _rifft_tilde {
    t_object    x_obj;                      /* Must be the first. */
    t_outlet    *x_outlet;
    } t_rifft_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *rifft_tilde_performFlip (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)w[3];
    
    while (n--) { t_sample f = *in; --out; *out = - f; in++; }
    
    return (w + 4);
}

/* No aliasing. */

static t_int *rifft_tilde_perform (t_int *w)
{
    t_FFTState *x = (t_FFTState *)(w[1]);
    PD_RESTRICTED in = (t_sample *)(w[2]);
    int n = (int)w[3];
    
    fft_realInverseFFT32 (x, n, in);
    
    return (w + 4);
}

static void rifft_tilde_dsp (t_rifft_tilde *x, t_signal **sp)
{
    int n = sp[0]->s_vectorSize;
    
    PD_ASSERT (PD_IS_POWER_2 (n));
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    if (n < FFT_MINIMUM || n > FFT_MAXIMUM) { error_invalid (sym_rifft__tilde__, sym_size); }
    else {
    //
    int half = (n >> 1);
    
    PD_RESTRICTED in1  = sp[0]->s_vector;
    PD_RESTRICTED in2  = sp[1]->s_vector;
    PD_RESTRICTED out1 = sp[2]->s_vector;

    t_FFTState *t = fftstate_new (cast_object (x), n);
        
    dsp_addCopyPerform (in1, out1, half + 1);
    
    dsp_add3 (rifft_tilde_performFlip, in2 + 1, out1 + n, half - 1);
    dsp_add3 (rifft_tilde_perform, t, out1, n);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *rifft_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_rifft_tilde *x = (t_rifft_tilde *)z;
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

static void *rifft_tilde_new (void)
{
    t_rifft_tilde *x = (t_rifft_tilde *)pd_new (rifft_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void rifft_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_rifft__tilde__,
            (t_newmethod)rifft_tilde_new,
            NULL,
            sizeof (t_rifft_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
            
    class_addDSP (c, (t_method)rifft_tilde_dsp);
        
    class_setDataFunction (c, rifft_tilde_functionData);
    
    rifft_tilde_class = c;
}

PD_LOCAL void rifft_tilde_destroy (void)
{
    class_free (rifft_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
