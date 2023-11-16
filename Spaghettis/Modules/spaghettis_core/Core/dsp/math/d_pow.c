
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_math.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *pow_tilde_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_pow_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */
/* Notice that the two signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static t_int *pow_tilde_perform (t_int *w)
{
    PD_RESTRICTED in1 = (t_sample *)(w[1]);
    PD_RESTRICTED in2 = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f = *in1++;
    t_sample g = *in2++;

    /* Avoid divide by zero with negative power. */
    /* Avoid root of negative numbers with fractional power. */
    
    int err = (f == (t_sample)0.0 && g < (t_sample)0.0) || (f < (t_sample)0.0 && !PD_FLOAT32_IS_INTEGER (g));

    if (err) { *out++ = 0.0; } else { *out++ = (t_sample)pow (f, g); }
    //
    }
    
    return (w + 5);
}

static void pow_tilde_dsp (t_pow_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));
    
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    
    dsp_add4 (pow_tilde_perform, sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *pow_tilde_new (t_float f)
{
    t_pow_tilde *x = (t_pow_tilde *)pd_new (pow_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    pd_float ((t_pd *)inlet_newSignal (cast_object (x)), f);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void pow_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_pow__tilde__,
            (t_newmethod)pow_tilde_new,
            NULL,
            sizeof (t_pow_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)pow_tilde_dsp);
        
    class_setDataFunction (c, binop_tilde_functionData);
    class_setHelpName (c, sym_math__tilde__);
    
    pow_tilde_class = c;
}

void pow_tilde_destroy (void)
{
    class_free (pow_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
