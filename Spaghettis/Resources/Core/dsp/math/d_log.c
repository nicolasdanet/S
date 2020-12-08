
/* Copyright (c) 1997-2020 Miller Puckette and others. */

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

static t_class *log_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _binop_tilde t_log_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */
/* Notice that the two signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static t_int *log_tilde_perform (t_int *w)
{
    PD_RESTRICTED in1 = (t_sample *)(w[1]);
    PD_RESTRICTED in2 = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    int n = (int)(w[4]);
    
    while (n--) {
    //
    t_sample f = *in1++;
    t_sample g = *in2++;
    
    if (f <= 0.0)      { *out++ = (t_sample)-1000.0; }
    else if (g <= 0.0) { *out++ = (t_sample)log (f); }
    else {
        *out++ = (t_sample)(log (f) / log (g));
    }
    //
    }
    
    return (w + 5);
}

static void log_tilde_dsp (t_log_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));
    
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    
    dsp_add4 (log_tilde_perform, sp[0]->s_vector, sp[1]->s_vector, sp[2]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *log_tilde_new (t_float f)
{
    t_log_tilde *x = (t_log_tilde *)pd_new (log_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    pd_float ((t_pd *)inlet_newSignal (cast_object (x)), f);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void log_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_log__tilde__,
            (t_newmethod)log_tilde_new,
            NULL,
            sizeof (t_log_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
                
    class_addDSP (c, (t_method)log_tilde_dsp);
        
    class_setDataFunction (c, binop_tilde_functionData);
    class_setHelpName (c, sym_math__tilde__);
    
    log_tilde_class = c;
}

PD_LOCAL void log_tilde_destroy (void)
{
    class_free (log_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
