
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

static t_class *wrap_tilde_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _unop_tilde t_wrap_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *wrap_tilde_perform (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    while (n--) {
        t_sample f = *in++;
        int k = (int)f;
        if (f >= 0.0) { *out++ = f - k; } else { *out++ = f - (k - 1); }
    }
    
    return (w + 4);
}

static void wrap_tilde_dsp (t_wrap_tilde *x, t_signal **sp)
{
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_add3 (wrap_tilde_perform, sp[0]->s_vector, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *wrap_tilde_new (void)
{
    t_wrap_tilde *x = (t_wrap_tilde *)pd_new (wrap_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void wrap_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_wrap__tilde__,
            (t_newmethod)wrap_tilde_new,
            NULL,
            sizeof (t_wrap_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
        
    class_addDSP (c, (t_method)wrap_tilde_dsp);
    
    class_setDataFunction (c, unop_tilde_functionData);
    class_setHelpName (c, sym_math__tilde__);
    
    wrap_tilde_class = c;
}

PD_LOCAL void wrap_tilde_destroy (void)
{
    class_free (wrap_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
