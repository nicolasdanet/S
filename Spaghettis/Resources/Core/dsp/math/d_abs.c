
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

static t_class *abs_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _unop_tilde t_abs_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *abs_tilde_perform (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    while (n--) { t_sample f = *in++; *out++ = PD_ABS (f); }
    
    return (w + 4);
}

static void abs_tilde_dsp (t_abs_tilde *x, t_signal **sp)
{
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_add3 (abs_tilde_perform, sp[0]->s_vector, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer *unop_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    struct _unop_tilde *x = (struct _unop_tilde *)z;
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

static void *abs_tilde_new (void)
{
    t_abs_tilde *x = (t_abs_tilde *)pd_new (abs_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void abs_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_abs__tilde__,
            (t_newmethod)abs_tilde_new,
            NULL,
            sizeof (t_abs_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
            
    class_addDSP (c, (t_method)abs_tilde_dsp);

    class_setDataFunction (c, unop_tilde_functionData);
    class_setHelpName (c, sym_math__tilde__);
    
    abs_tilde_class = c;
}

PD_LOCAL void abs_tilde_destroy (void)
{
    class_free (abs_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
