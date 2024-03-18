
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_filters.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Real reverse one-zero filter. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/One_Zero.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *rzero_rev_tilde_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _real_raw_tilde t_rzero_rev_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */
/* Notice that the two signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static t_int *rzero_rev_tilde_perform (t_int *w)
{
    t_rzero_rev_tilde *x = (t_rzero_rev_tilde *)(w[1]);
    PD_RESTRICTED in1 = (t_sample *)(w[2]);
    PD_RESTRICTED in2 = (t_sample *)(w[3]);
    PD_RESTRICTED out = (t_sample *)(w[4]);
    int n = (int)(w[5]);
    
    t_sample last = x->x_real;
    
    while (n--) {
        t_sample f = *in1++;
        t_sample b = *in2++;
        *out++ = last - b * f;
        last   = f;
    }
    
    x->x_real = last;
    
    return (w + 6);
}

static void rzero_rev_tilde_dsp (t_rzero_rev_tilde *x, t_signal **sp)
{
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    
    real_raw_initializer (cast_object (x));
    
    dsp_add5 (rzero_rev_tilde_perform, x,
        sp[0]->s_vector,
        sp[1]->s_vector,
        sp[2]->s_vector, 
        sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *rzero_rev_tilde_new (t_float f)
{
    t_rzero_rev_tilde *x = (t_rzero_rev_tilde *)pd_new (rzero_rev_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    pd_float ((t_pd *)inlet_newSignal (cast_object (x)), f);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void rzero_rev_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_rzero_rev__tilde__,
            (t_newmethod)rzero_rev_tilde_new,
            NULL,
            sizeof (t_rzero_rev_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)rzero_rev_tilde_dsp);
        
    class_setDataFunction (c, real_raw_functionData);
    
    rzero_rev_tilde_class = c;
}

void rzero_rev_tilde_destroy (void)
{
    class_free (rzero_rev_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
