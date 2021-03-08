
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Two-pole band-pass filter. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/Two_Pole.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *bp_tilde_class;                 /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _bp_tilde {
    t_object            x_obj;                  /* Must be the first. */
    t_spin              x_mutex;
    t_float             x_frequency;
    t_float             x_q;
    int                 x_set;
    t_sample            x_real1;
    t_sample            x_real2;
    t_outlet            *x_outlet;
    } t_bp_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bp_tilde_frequency (t_bp_tilde *x, t_float f)
{
    spin_lock (&x->x_mutex);
    
        x->x_frequency  = (f < 0.001) ? (t_float)10.0 : f;
        x->x_set        = 1;
    
    spin_unlock (&x->x_mutex);
}

static void bp_tilde_q (t_bp_tilde *x, t_float q)
{
    spin_lock (&x->x_mutex);
    
        x->x_q          = (t_float)PD_MAX (0.0, q);
        x->x_set        = 1;
    
    spin_unlock (&x->x_mutex);
}

static void bp_tilde_set (t_bp_tilde *x, t_float f, t_float q)
{
    bp_tilde_frequency (x, f);
    bp_tilde_q (x, q);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bp_tilde_space (t_space *t, t_float frequency, t_float q)
{
    double omega     = frequency * t->s_float0;
    double omegaPerQ = PD_MIN ((q < 0.001) ? 1.0 : (omega / q), 1.0);
    double r         = 1.0 - omegaPerQ;
    double k         = (omega < -PD_HALF_PI || omega > PD_HALF_PI) ? 0.0 : cos (omega);
    
    t->s_float1      = (2.0 * k * r);
    t->s_float2      = (- r * r);
    t->s_float3      = (2.0 * omegaPerQ * (omegaPerQ + r * omega));
}

/* No aliasing. */

static t_int *bp_tilde_perform (t_int *w)
{
    t_bp_tilde *x = (t_bp_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    t_space *t        = (t_space *)(w[4]);
    int n = (int)(w[5]);

    if (spin_trylock (&x->x_mutex) == 0) {
    //
    if (x->x_set) { bp_tilde_space (t, x->x_frequency, x->x_q); x->x_set = 0; }
    
    spin_unlock (&x->x_mutex);
    //
    }
    
    t_sample a1      = t->s_float1;
    t_sample a2      = t->s_float2;
    t_sample gain    = t->s_float3;
    
    t_sample last1   = x->x_real1;
    t_sample last2   = x->x_real2;
    
    while (n--) {
        t_sample f = (*in++) + a1 * last1 + a2 * last2;
        *out++ = gain * f; 
        last2  = last1;
        last1  = f;
    }
    
    if (denormal_isBigOrSmall (last1)) { last1 = 0.0; }
    if (denormal_isBigOrSmall (last2)) { last2 = 0.0; }
    
    x->x_real1 = last1;
    x->x_real2 = last2;
    
    return (w + 6);
}

static void bp_tilde_initialize (void *lhs, void *rhs)
{
    t_bp_tilde *x   = (t_bp_tilde *)lhs;
    t_bp_tilde *old = (t_bp_tilde *)rhs;
    
    x->x_real1 = old->x_real1;
    x->x_real2 = old->x_real2;
}

static void bp_tilde_dsp (t_bp_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_bp_tilde *old = (t_bp_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (bp_tilde_initialize, x, old);
    
    bp_tilde_set (x, old->x_frequency, old->x_q);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    {
    //
    t_space *t = space_new (cast_object (x)); t->s_float0 = (t_float)(PD_TWO_PI / sp[0]->s_sampleRate);

    // spin_lock (&x->x_mutex);
    
        bp_tilde_space (t, x->x_frequency, x->x_q);
    
    // spin_unlock (&x->x_mutex);
    
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    dsp_add5 (bp_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *bp_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_bp_tilde *x = (t_bp_tilde *)z;
    t_buffer *b = buffer_new();
    
    // spin_lock (&x->x_mutex);
    
        t_float f = x->x_frequency;
        t_float q = x->x_q;
    
    // spin_unlock (&x->x_mutex);
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, f);
    buffer_appendFloat (b, q);
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void bp_tilde_restore (t_bp_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float f = atom_getFloatAtIndex (0, argc, argv);
    t_float q = atom_getFloatAtIndex (1, argc, argv);
    
    bp_tilde_set (x, f, q);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *bp_tilde_new (t_float f, t_float q)
{
    t_bp_tilde *x = (t_bp_tilde *)pd_new (bp_tilde_class);
    
    spin_init (&x->x_mutex);

    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);
    inlet_new3 (x, &s_float);
    
    bp_tilde_set (x, f, q);

    return x;
}

static void bp_tilde_free (t_bp_tilde *x)
{
    spin_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void bp_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_bp__tilde__,
            (t_newmethod)bp_tilde_new,
            (t_method)bp_tilde_free,
            sizeof (t_bp_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDSP (c, (t_method)bp_tilde_dsp);
    
    class_addMethod (c, (t_method)bp_tilde_frequency,   sym__inlet2,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)bp_tilde_q,           sym__inlet3,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)bp_tilde_restore,     sym__restore,   A_GIMME, A_NULL);
    
    class_setDataFunction (c, bp_tilde_functionData);
    
    bp_tilde_class = c;
}

PD_LOCAL void bp_tilde_destroy (void)
{
    class_free (bp_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
