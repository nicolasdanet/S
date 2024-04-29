
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *phasor_tilde_class;     /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _phasor_tilde {
    t_object            x_obj;          /* Must be the first. */
    t_float64Atomic     x_phase;
    t_outlet            *x_outlet;
    } t_phasor_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *phasor_tilde_perform (t_int *w)
{
    t_phasor_tilde *x = (t_phasor_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    t_space *t        = (t_space *)(w[4]);
    int n = (int)(w[5]);
    
    double phase = atomic_float64Read (&x->x_phase);
    
    double f = phase;

    while (n--) { f = dsp_wrapPhasor (f); *out++ = (t_sample)f; f += (*in++) * t->s_float0; }
    
    /* Don't overwrite if the phase have been explicitly changed. */
    
    atomic_float64CompareAndSwap (&x->x_phase, &phase, f);
    
    return (w + 6);
}

static void phasor_tilde_initialize (void *lhs, void *rhs)
{
    t_phasor_tilde *x   = (t_phasor_tilde *)lhs;
    t_phasor_tilde *old = (t_phasor_tilde *)rhs;
    
    t_float f = atomic_float64Read (&old->x_phase); atomic_float64Write (&x->x_phase, f);
}

static void phasor_tilde_dsp (t_phasor_tilde *x, t_signal **sp)
{
    t_space *t = space_new (cast_object (x)); t->s_float0 = (t_float)(1.0 / sp[0]->s_sampleRate);
    
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_phasor_tilde *old = (t_phasor_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (phasor_tilde_initialize, x, old);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    dsp_add5 (phasor_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *phasor_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_phasor_tilde *x = (t_phasor_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, atomic_float64Read (&x->x_phase));
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void phasor_tilde_restore (t_phasor_tilde *x, t_float f)
{
    atomic_float64Write (&x->x_phase, f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *phasor_tilde_new (t_float f)
{
    t_phasor_tilde *x = (t_phasor_tilde *)pd_new (phasor_tilde_class);
    
    atomic_float64Write (object_getFirstInletSignal (cast_object (x)), f);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void phasor_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_phasor__tilde__,
            (t_newmethod)phasor_tilde_new,
            NULL,
            sizeof (t_phasor_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)phasor_tilde_dsp);
    
    class_addMethod (c, (t_method)phasor_tilde_restore, sym__inlet2,  A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)phasor_tilde_restore, sym__restore, A_FLOAT, A_NULL);

    class_setDataFunction (c, phasor_tilde_functionData);
    
    phasor_tilde_class = c;
}

void phasor_tilde_destroy (void)
{
    class_free (phasor_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
