
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

/* Complex one-pole filter. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/One_Pole.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *cpole_tilde_class;              /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _complex_raw_tilde t_cpole_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */
/* Notice that the fourth signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static t_int *cpole_tilde_perform (t_int *w)
{
    t_cpole_tilde *x   = (t_cpole_tilde *)(w[1]);
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

        t_sample aReal          = (*in3++);
        t_sample aImaginary     = (*in4++);
        t_sample realPart       = (*in1++) + lastReal * aReal - lastImaginary * aImaginary;
        t_sample imaginaryPart  = (*in2++) + lastReal * aImaginary + lastImaginary * aReal;
        
        *out1++ = lastReal      = realPart;
        *out2++ = lastImaginary = imaginaryPart;
    }
    
    if (denormal_isBigOrSmall (lastReal))      { lastReal      = 0.0; }
    if (denormal_isBigOrSmall (lastImaginary)) { lastImaginary = 0.0; }
    
    x->x_real      = lastReal;
    x->x_imaginary = lastImaginary;
    
    return (w + 9);
}

static void complex_raw_initialize (void *lhs, void *rhs)
{
    struct _complex_raw_tilde *x   = (struct _complex_raw_tilde *)lhs;
    struct _complex_raw_tilde *old = (struct _complex_raw_tilde *)rhs;
    
    x->x_real      = old->x_real;
    x->x_imaginary = old->x_imaginary;
}

void complex_raw_initializer (t_object *x)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    struct _complex_raw_tilde *old = (struct _complex_raw_tilde *)garbage_fetch (x);
    
    if (old) {
    //
    initializer_new (complex_raw_initialize, x, old);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
}

static void cpole_tilde_dsp (t_cpole_tilde *x, t_signal **sp)
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
    
    dsp_add8 (cpole_tilde_perform, x,
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

t_buffer *complex_raw_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    struct _complex_raw_tilde *x = (struct _complex_raw_tilde *)z;
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

static void *cpole_tilde_new (t_float real, t_float imaginary)
{
    t_cpole_tilde *x = (t_cpole_tilde *)pd_new (cpole_tilde_class);
    
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

void cpole_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_cpole__tilde__,
            (t_newmethod)cpole_tilde_new,
            NULL, 
            sizeof (t_cpole_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)cpole_tilde_dsp);
        
    class_setDataFunction (c, complex_raw_functionData);
    
    cpole_tilde_class = c;
}

void cpole_tilde_destroy (void)
{
    class_free (cpole_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
