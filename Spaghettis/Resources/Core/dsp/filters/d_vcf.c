
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Complex one-pole resonant filter (with audio-rate center frequency input). */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/Complex_Resonator.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *vcf_tilde_class;                   /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _vcf_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_sample            x_real;
    t_sample            x_imaginary;
    t_float64Atomic     x_q;
    t_outlet            *x_outletLeft;
    t_outlet            *x_outletRight;
    } t_vcf_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vcf_tilde_qFactor (t_vcf_tilde *x, t_float f)
{
    f = PD_MAX (0.0, f); PD_ATOMIC_FLOAT64_WRITE (f, &x->x_q);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */
/* Notice that the two signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static t_int *vcf_tilde_perform (t_int *w)
{
    t_vcf_tilde *x     = (t_vcf_tilde *)(w[1]);
    PD_RESTRICTED in1  = (t_sample *)(w[2]);
    PD_RESTRICTED in2  = (t_sample *)(w[3]);
    PD_RESTRICTED out1 = (t_sample *)(w[4]);
    PD_RESTRICTED out2 = (t_sample *)(w[5]);
    t_space *t         = (t_space *)(w[6]);
    int n = (int)(w[7]);
    
    t_sample re = x->x_real;
    t_sample im = x->x_imaginary;
    
    double q = PD_ATOMIC_FLOAT64_READ (&x->x_q);
    double qInverse = (q > 0.0 ? 1.0 / q : 0.0);
    double correction = 2.0 - (2.0 / (q + 2.0));

    while (n--) {
    //
    double centerFrequency;
    double r, g;
    double pReal;
    double pImaginary;
    
    centerFrequency = (*in2++) * t->s_float0;
    centerFrequency = PD_MAX (0.0, centerFrequency);
    
    r = (qInverse > 0.0 ? 1.0 - centerFrequency * qInverse : 0.0);
    r = PD_MAX (0.0, r);
    g = correction * (1.0 - r);
    
    pReal      = r * dsp_getCosineAtLUT (centerFrequency * (COSINE_TABLE_SIZE / PD_TWO_PI));
    pImaginary = r * dsp_getSineAtLUT   (centerFrequency * (COSINE_TABLE_SIZE / PD_TWO_PI));
    
    {
        double s = (*in1++);
        double tReal = re;
        double tImaginary = im;
        
        re = (t_sample)((g * s) + (pReal * tReal - pImaginary * tImaginary));
        im = (t_sample)((pImaginary * tReal + pReal * tImaginary));
        
        *out1++ = re;
        *out2++ = im;
    }
    //
    }
    
    if (denormal_isBigOrSmall (re)) { re = 0.0; }
    if (denormal_isBigOrSmall (im)) { im = 0.0; }
    
    x->x_real = re;
    x->x_imaginary = im;
    
    return (w + 8);
}

static void vcf_tilde_initialize (void *lhs, void *rhs)
{
    t_vcf_tilde *x   = (t_vcf_tilde *)lhs;
    t_vcf_tilde *old = (t_vcf_tilde *)rhs;
    
    x->x_real      = old->x_real;
    x->x_imaginary = old->x_imaginary;
}

static void vcf_tilde_dsp (t_vcf_tilde *x, t_signal **sp)
{
    t_space *t = space_new (cast_object (x)); t->s_float0 = (t_float)(PD_TWO_PI / sp[0]->s_sampleRate);
   
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[0]->s_vector != sp[3]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[3]->s_vector);
    PD_ASSERT (sp[2]->s_vector != sp[3]->s_vector);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_vcf_tilde *old = (t_vcf_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (vcf_tilde_initialize, x, old);
    
    vcf_tilde_qFactor (x, PD_ATOMIC_FLOAT64_READ (&old->x_q));
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    dsp_add7 (vcf_tilde_perform, x,
        sp[0]->s_vector,
        sp[1]->s_vector,
        sp[2]->s_vector,
        sp[3]->s_vector,
        t,
        sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *vcf_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_vcf_tilde *x = (t_vcf_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__inlet2);
    buffer_appendFloat (b,  PD_ATOMIC_FLOAT64_READ (&x->x_q));
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *vcf_tilde_new (t_float f)
{
    t_vcf_tilde *x = (t_vcf_tilde *)pd_new (vcf_tilde_class);
    
    vcf_tilde_qFactor (x, f);
    
    x->x_outletLeft  = outlet_newSignal (cast_object (x));
    x->x_outletRight = outlet_newSignal (cast_object (x));

    inlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void vcf_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_vcf__tilde__,
            (t_newmethod)vcf_tilde_new,
            NULL,
            sizeof (t_vcf_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDSP (c, (t_method)vcf_tilde_dsp);
    
    class_addMethod (c, (t_method)vcf_tilde_qFactor, sym__inlet2, A_FLOAT, A_NULL);
    
    class_setDataFunction (c, vcf_tilde_functionData);
    
    vcf_tilde_class = c;
}

PD_LOCAL void vcf_tilde_destroy (void)
{
    class_free (vcf_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
