
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_filters.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Complex one-zero filter. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/One_Zero.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *czero_tilde_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _complex_raw_tilde t_czero_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */
/* Notice that the fourth signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static t_int *czero_tilde_perform (t_int *w)
{
    t_czero_tilde *x   = (t_czero_tilde *)(w[1]);
    PD_RESTRICTED in1  = (t_sample *)(w[2]);
    PD_RESTRICTED in2  = (t_sample *)(w[3]);
    PD_RESTRICTED in3  = (t_sample *)(w[4]);
    PD_RESTRICTED in4  = (t_sample *)(w[5]);
    PD_RESTRICTED out1 = (t_sample *)(w[6]);
    PD_RESTRICTED out2 = (t_sample *)(w[7]);
    int n = (int)(w[8]);
    
    t_sample lastReal      = x->x_real;
    t_sample lastImaginary = x->x_imaginary;
    
    while (n--) {
    
        t_sample real       = *in1++;
        t_sample imaginary  = *in2++;
        t_sample bReal      = *in3++;
        t_sample bImaginary = *in4++;
        
        *out1++ = real - lastReal * bReal + lastImaginary * bImaginary;
        *out2++ = imaginary - lastReal * bImaginary - lastImaginary * bReal;
        
        lastReal      = real;
        lastImaginary = imaginary;
    }
    
    x->x_real      = lastReal;
    x->x_imaginary = lastImaginary;
    
    return (w + 9);
}

static void czero_tilde_dsp (t_czero_tilde *x, t_signal **sp)
{
    PD_ASSERT (sp[0]->s_vector != sp[4]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[4]->s_vector);
    PD_ASSERT (sp[2]->s_vector != sp[4]->s_vector);
    PD_ASSERT (sp[3]->s_vector != sp[4]->s_vector);
    PD_ASSERT (sp[0]->s_vector != sp[5]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[5]->s_vector);
    PD_ASSERT (sp[2]->s_vector != sp[5]->s_vector);
    PD_ASSERT (sp[3]->s_vector != sp[5]->s_vector);
    PD_ASSERT (sp[4]->s_vector != sp[5]->s_vector);
    
    complex_raw_initializer (cast_object (x));
    
    dsp_add8 (czero_tilde_perform, x,
        sp[0]->s_vector,
        sp[1]->s_vector,
        sp[2]->s_vector,
        sp[3]->s_vector, 
        sp[4]->s_vector,
        sp[5]->s_vector,
        sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *czero_tilde_new (t_float real, t_float imaginary)
{
    t_czero_tilde *x = (t_czero_tilde *)pd_new (czero_tilde_class);
    
    x->x_outletLeft  = outlet_newSignal (cast_object (x));
    x->x_outletRight = outlet_newSignal (cast_object (x));
    
    inlet_newSignal (cast_object (x));
    
    pd_float ((t_pd *)inlet_newSignal (cast_object (x)), real);
    pd_float ((t_pd *)inlet_newSignal (cast_object (x)), imaginary);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void czero_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_czero__tilde__,
            (t_newmethod)czero_tilde_new,
            NULL,
            sizeof (t_czero_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)czero_tilde_dsp);
    
    class_setDataFunction (c, complex_raw_functionData);
    
    czero_tilde_class = c;
}

PD_LOCAL void czero_tilde_destroy (void)
{
    class_free (czero_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
