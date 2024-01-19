
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* One-pole low-pass filter. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/One_Pole.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *lop_tilde_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _lop_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_float64Atomic     x_frequency;
    t_sample            x_real;
    t_outlet            *x_outlet;
    } t_lop_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void lop_tilde_frequency (t_lop_tilde *x, t_float f)
{
    f = PD_MAX (0.0, f); PD_ATOMIC_FLOAT64_WRITE (f, &x->x_frequency);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *lop_tilde_perform (t_int *w)
{
    t_lop_tilde *x = (t_lop_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    t_space *t        = (t_space *)(w[4]);
    int n = (int)(w[5]);
    
    t_sample k = (t_sample)(PD_ATOMIC_FLOAT64_READ (&x->x_frequency) * t->s_float0);
    t_sample b = (t_sample)(PD_CLAMP (k, 0.0, 1.0));
    t_sample a = (t_sample)(1.0 - b);
    
    t_sample last = x->x_real;
    
    while (n--) {
        t_sample f = b * (*in++) + a * last;
        *out++ = last = f;
    }
    
    if (denormal_isBigOrSmall (last)) { last = 0.0; }
    
    x->x_real = last;
    
    return (w + 6);
}

static void lop_tilde_initialize (void *lhs, void *rhs)
{
    t_lop_tilde *x   = (t_lop_tilde *)lhs;
    t_lop_tilde *old = (t_lop_tilde *)rhs;
    
    x->x_real = old->x_real;
}

static void lop_tilde_dsp (t_lop_tilde *x, t_signal **sp)
{
    t_space *t = space_new (cast_object (x)); t->s_float0 = (t_float)(PD_TWO_PI / sp[0]->s_sampleRate);

    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_lop_tilde *old = (t_lop_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (lop_tilde_initialize, x, old);
    
    lop_tilde_frequency (x, PD_ATOMIC_FLOAT64_READ (&old->x_frequency));
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    dsp_add5 (lop_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *lop_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_lop_tilde *x = (t_lop_tilde *)z;
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

static void *lop_tilde_new (t_float f)
{
    t_lop_tilde *x = (t_lop_tilde *)pd_new (lop_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);

    lop_tilde_frequency (x, f);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void lop_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_lop__tilde__,
            (t_newmethod)lop_tilde_new,
            NULL,
            sizeof (t_lop_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)lop_tilde_dsp);
    
    class_addMethod (c, (t_method)lop_tilde_frequency, sym__inlet2, A_FLOAT, A_NULL);
    
    class_setDataFunction (c, lop_tilde_functionData);
    
    lop_tilde_class = c;
}

void lop_tilde_destroy (void)
{
    class_free (lop_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
