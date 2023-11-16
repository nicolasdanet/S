
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *sig_tilde_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _sig_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_float64Atomic     x_f;
    t_outlet            *x_outlet;
    } t_sig_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void sig_tilde_float (t_sig_tilde *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void sig_tilde_dsp (t_sig_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_sig_tilde *old = (t_sig_tilde *)garbage_fetch (cast_object (x));
    
    if (old) { sig_tilde_float (x, PD_ATOMIC_FLOAT64_READ (&old->x_f)); }
    //
    }
    
    dsp_addScalarPerform (&x->x_f, sp[0]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *sig_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_sig_tilde *x = (t_sig_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, &s_float);
    buffer_appendFloat (b, PD_ATOMIC_FLOAT64_READ (&x->x_f));
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *sig_tilde_new (t_float f)
{
    t_sig_tilde *x = (t_sig_tilde *)pd_new (sig_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_f);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void sig_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_sig__tilde__,
            (t_newmethod)sig_tilde_new,
            NULL,
            sizeof (t_sig_tilde),
            CLASS_DEFAULT, 
            A_DEFFLOAT,
            A_NULL);
    
    class_addDsp (c, (t_method)sig_tilde_dsp);
    class_addFloat (c, (t_method)sig_tilde_float);
    
    class_setDataFunction (c, sig_tilde_functionData);

    sig_tilde_class = c;
}

void sig_tilde_destroy (void)
{
    class_free (sig_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
