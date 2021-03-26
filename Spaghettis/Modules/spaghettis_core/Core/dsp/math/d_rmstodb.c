
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

static t_class *rmstodb_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _unop_tilde t_rmstodb_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *rmstodb_tilde_perform (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    while (n--) { *out++ = math_rootMeanSquareToDecibel (*in++); }
    
    return (w + 4);
}

static void rmstodb_tilde_dsp (t_rmstodb_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    dsp_add3 (rmstodb_tilde_perform, sp[0]->s_vector, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *rmstodb_tilde_new (void)
{
    t_rmstodb_tilde *x = (t_rmstodb_tilde *)pd_new (rmstodb_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void rmstodb_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_rmstodb__tilde__, 
            (t_newmethod)rmstodb_tilde_new,
            NULL,
            sizeof (t_rmstodb_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);

    class_addCreator ((t_newmethod)rmstodb_tilde_new, sym_amptodb__tilde__, A_NULL);

    class_addDsp (c, (t_method)rmstodb_tilde_dsp);
    
    class_setDataFunction (c, unop_tilde_functionData);
    class_setHelpName (c, sym_acoustic__tilde__);
    
    rmstodb_tilde_class = c;
}

PD_LOCAL void rmstodb_tilde_destroy (void)
{
    class_free (rmstodb_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
