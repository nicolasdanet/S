
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Results for high frequency (i.e. 700 kHz at 44100 Hz) are wrong. */
/* It is due to limited range of Hoeldrich's trick. */
/* But does it really matter, since it is always far above the Nyquist frequency? */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *osc_tilde_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _osc_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_float64Atomic     x_phase;
    t_outlet            *x_outlet;
    } t_osc_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int atomic_float64CompareAndSwap (double *, double, t_float64Atomic *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *osc_tilde_perform (t_int *w)
{
    t_osc_tilde *x    = (t_osc_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    t_space *t        = (t_space *)(w[4]);
    int n             = (int)(w[5]);
    
    double phase = PD_ATOMIC_FLOAT64_READ (&x->x_phase);
    
    double f = dsp_wrapCosine (phase);

    /* Accumulation can not overflow the Hoeldrich's trick range with a reasonable frequency. */
    
    while (n--) { *out++ = (t_sample)dsp_getCosineAtLUT (f); f += (*in++) * t->s_float0; }

    /* Don't overwrite if the phase have been explicitly changed. */
    
    atomic_float64CompareAndSwap (&phase, f, &x->x_phase);
    
    return (w + 6);
}

static void osc_tilde_initialize (void *lhs, void *rhs)
{
    t_osc_tilde *x   = (t_osc_tilde *)lhs;
    t_osc_tilde *old = (t_osc_tilde *)rhs;
    
    t_float f = PD_ATOMIC_FLOAT64_READ (&old->x_phase); PD_ATOMIC_FLOAT64_WRITE (f, &x->x_phase);
}

static void osc_tilde_dsp (t_osc_tilde *x, t_signal **sp)
{
    t_space *t = space_new (cast_object (x)); t->s_float0 = COSINE_TABLE_SIZE / sp[0]->s_sampleRate;
    
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_osc_tilde *old = (t_osc_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (osc_tilde_initialize, x, old);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    dsp_add5 (osc_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *osc_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_osc_tilde *x = (t_osc_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, PD_ATOMIC_FLOAT64_READ (&x->x_phase));
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void osc_tilde_restore (t_osc_tilde *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_phase);
}

static void osc_tilde_phase (t_osc_tilde *x, t_float f)
{
    osc_tilde_restore (x, (double)COSINE_TABLE_SIZE * f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *osc_tilde_new (t_float f)
{
    t_osc_tilde *x = (t_osc_tilde *)pd_new (osc_tilde_class);
    
    PD_ATOMIC_FLOAT64_WRITE (f, object_getFirstInletSignal (cast_object (x)));
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void osc_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_osc__tilde__,
            (t_newmethod)osc_tilde_new,
            NULL,
            sizeof (t_osc_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)osc_tilde_dsp);
    
    class_addMethod (c, (t_method)osc_tilde_phase,   sym__inlet2,  A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)osc_tilde_restore, sym__restore, A_FLOAT, A_NULL);
    
    class_setDataFunction (c, osc_tilde_functionData);

    osc_tilde_class = c;
}

void osc_tilde_destroy (void)
{
    class_free (osc_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
