
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

static t_class *mtof_tilde_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _unop_tilde t_mtof_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *mtof_tilde_perform (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    while (n--) { *out++ = math_midiToFrequency (*in++); }
    
    return (w + 4);
}

static void mtof_tilde_dsp (t_mtof_tilde *x, t_signal **sp)
{
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_add3 (mtof_tilde_perform, sp[0]->s_vector, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *mtof_tilde_new (void)
{
    t_mtof_tilde *x = (t_mtof_tilde *)pd_new (mtof_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void mtof_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_mtof__tilde__,
            (t_newmethod)mtof_tilde_new,
            NULL,
            sizeof (t_mtof_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
        
    class_addDSP (c, (t_method)mtof_tilde_dsp);
    
    class_setDataFunction (c, unop_tilde_functionData);
    class_setHelpName (c, sym_acoustic__tilde__);
    
    mtof_tilde_class = c;
}

PD_LOCAL void mtof_tilde_destroy (void)
{
    class_free (mtof_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
