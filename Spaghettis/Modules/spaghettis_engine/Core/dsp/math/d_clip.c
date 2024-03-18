
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *clip_tilde_class;               /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _clip_tilde {
    t_object            x_obj;                  /* Must be the first. */
    t_float64Atomic     x_low;
    t_float64Atomic     x_high;
    t_outlet            *x_outlet;
    } t_clip_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void clip_tilde_low (t_clip_tilde *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_low);
}

static void clip_tilde_high (t_clip_tilde *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_high);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *clip_tilde_perform (t_int *w)
{
    t_clip_tilde *x   = (t_clip_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    int n = (int)(w[4]);
    
    t_float t0  = PD_ATOMIC_FLOAT64_READ (&x->x_low);
    t_float t1  = PD_ATOMIC_FLOAT64_READ (&x->x_high);
    t_sample f1 = PD_MIN (t0, t1);
    t_sample f2 = PD_MAX (t0, t1);
    
    while (n--) {
    //
    t_sample f = *in++;
    
    *out++ = PD_CLAMP (f, f1, f2);
    //
    }
    
    return (w + 5);
}

static void clip_tilde_dsp (t_clip_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_clip_tilde *old = (t_clip_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    clip_tilde_low (x, PD_ATOMIC_FLOAT64_READ (&old->x_low));
    clip_tilde_high (x, PD_ATOMIC_FLOAT64_READ (&old->x_high));
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    dsp_add4 (clip_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *clip_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_clip_tilde *x = (t_clip_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__inlet2);
    buffer_appendFloat (b,  PD_ATOMIC_FLOAT64_READ (&x->x_low));
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__inlet3);
    buffer_appendFloat (b,  PD_ATOMIC_FLOAT64_READ (&x->x_high));
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

static void *clip_tilde_new (t_float low, t_float high)
{
    t_clip_tilde *x = (t_clip_tilde *)pd_new (clip_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);
    inlet_new3 (x, &s_float);
    
    clip_tilde_low (x, low);
    clip_tilde_high (x, high);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void clip_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_clip__tilde__,
            (t_newmethod)clip_tilde_new,
            NULL,
            sizeof (t_clip_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)clip_tilde_dsp);
    
    class_addMethod (c, (t_method)clip_tilde_low,   sym__inlet2, A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)clip_tilde_high,  sym__inlet3, A_FLOAT, A_NULL);

    class_setDataFunction (c, clip_tilde_functionData);
    
    class_setHelpName (c, sym_math__tilde__);
    
    clip_tilde_class = c;
}

void clip_tilde_destroy (void)
{
    class_free (clip_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
