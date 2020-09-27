
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* One-pole one-zero high-pass filter. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < http://www.embedded.com/print/4007653 > */
/* < https://ccrma.stanford.edu/~jos/fp/DC_Blocker.html > */
/* < http://msp.ucsd.edu/techniques/v0.11/book-html/node141.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *hip_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _hip_tilde {
    t_object            x_obj;          /* Must be the first. */
    t_float64Atomic     x_frequency;
    t_sample            x_real;
    t_outlet            *x_outlet;
    } t_hip_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void hip_tilde_frequency (t_hip_tilde *x, t_float f)
{
    f = PD_MAX (0.0, f); PD_ATOMIC_FLOAT64_WRITE (f, &x->x_frequency);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *hip_tilde_perform (t_int *w)
{
    t_hip_tilde *x = (t_hip_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    t_space *t        = (t_space *)(w[4]);
    int n = (int)(w[5]);
    
    t_sample k = (t_sample)(1.0 - PD_ATOMIC_FLOAT64_READ (&x->x_frequency) * t->s_float0);
    t_sample a = (t_sample)(PD_CLAMP (k, 0.0, 1.0));
    t_sample normalize = (t_sample)(0.5 * (1.0 + a));
    
    if (a < 1.0) {  /* Is that really necessary? */
    //
    t_sample last = x->x_real;
        
    while (n--) {
        t_sample f = (*in++) + (a * last);
        *out++ = normalize * (f - last);
        last = f;
    }
        
    if (denormal_isBigOrSmall (last)) { last = 0.0; }
        
    x->x_real = last;
    //
    } else { while (n--) { *out++ = *in++; } x->x_real = 0; }

    return (w + 6);
}

static void hip_tilde_initialize (void *lhs, void *rhs)
{
    t_hip_tilde *x   = (t_hip_tilde *)lhs;
    t_hip_tilde *old = (t_hip_tilde *)rhs;
    
    x->x_real = old->x_real;
}

static void hip_tilde_dsp (t_hip_tilde *x, t_signal **sp)
{
    t_space *t = space_new (cast_object (x)); t->s_float0 = (t_float)(PD_TWO_PI / sp[0]->s_sampleRate);
    
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_hip_tilde *old = (t_hip_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (hip_tilde_initialize, x, old);
    
    hip_tilde_frequency (x, PD_ATOMIC_FLOAT64_READ (&old->x_frequency));
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    dsp_add5 (hip_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *hip_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_hip_tilde *x = (t_hip_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__inlet2);
    buffer_appendFloat (b,  PD_ATOMIC_FLOAT64_READ (&x->x_frequency));
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

static void *hip_tilde_new (t_float f)
{
    t_hip_tilde *x = (t_hip_tilde *)pd_new (hip_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);
    
    hip_tilde_frequency (x, f);

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void hip_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_hip__tilde__,
            (t_newmethod)hip_tilde_new,
            NULL,
            sizeof (t_hip_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
    
    class_addDSP (c, (t_method)hip_tilde_dsp);
    
    class_addMethod (c, (t_method)hip_tilde_frequency, sym__inlet2, A_FLOAT, A_NULL);
    
    class_setDataFunction (c, hip_tilde_functionData);
    
    hip_tilde_class = c;
}

PD_LOCAL void hip_tilde_destroy (void)
{   
    class_free (hip_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
