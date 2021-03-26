
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

static t_class *ftom_tilde_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _unop_tilde t_ftom_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *ftom_tilde_perform (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    while (n--) { *out++ = math_frequencyToMidi (*in++); }
    
    return (w + 4);
}

static void ftom_tilde_dsp (t_ftom_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    dsp_add3 (ftom_tilde_perform, sp[0]->s_vector, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *ftom_tilde_new (void)
{
    t_ftom_tilde *x = (t_ftom_tilde *)pd_new (ftom_tilde_class);

    x->x_outlet = outlet_newSignal (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void ftom_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_ftom__tilde__,
            (t_newmethod)ftom_tilde_new,
            NULL,
            sizeof (t_ftom_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
        
    class_addDsp (c, (t_method)ftom_tilde_dsp);
    
    class_setDataFunction (c, unop_tilde_functionData);
    class_setHelpName (c, sym_acoustic__tilde__);
    
    ftom_tilde_class = c;
}

PD_LOCAL void ftom_tilde_destroy (void)
{
    class_free (ftom_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
