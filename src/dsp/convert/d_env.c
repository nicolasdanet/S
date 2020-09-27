
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *env_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define ENV_MAXIMUM_OVERLAP             32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _env_tilde {
    t_object            x_obj;          /* Must be the first. */
    t_float64Atomic     x_result;
    int                 x_dismissed;
    int                 x_phase;
    int                 x_period;
    int                 x_window;
    t_sample            x_sum[ENV_MAXIMUM_OVERLAP + 1];
    t_sample            *x_vector;
    t_clock             *x_clock;
    t_outlet            *x_outlet;
    } t_env_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void env_tilde_task (t_env_tilde *x)
{
    if (!x->x_dismissed) {
        outlet_float (x->x_outlet, math_powerToDecibel (PD_ATOMIC_FLOAT64_READ (&x->x_result)));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *env_tilde_perform (t_int *w)
{
    t_env_tilde *x    = (t_env_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    PD_RESTRICTED sum = x->x_sum;
    
    int i;
    
    if (x->x_phase < 0) { x->x_phase = x->x_period - n; }       /* Once at startup. */
    
    for (i = x->x_phase; i < x->x_window; i += x->x_period) {
    //
    PD_RESTRICTED hann = x->x_vector + i;
    t_sample t = *sum;
    int j;
    
    in += n;
    
    for (j = 0; j < n; j++) {
        in--;
        t += (*hann) * ((*in) * (*in));
        hann++;
    }
    
    *sum++ = t;
    //
    }
    
    *sum = 0.0;
    
    x->x_phase -= n;
    
    if (x->x_phase < 0) {
    //
    sum = x->x_sum; PD_ATOMIC_FLOAT64_WRITE (*sum, &x->x_result);
    
    for (i = x->x_period; i < x->x_window; i += x->x_period) { *sum = *(sum + 1); sum++; } 
    
    *sum = 0.0;
    
    x->x_phase = x->x_period - n;
    
    clock_delay (x->x_clock, 0.0);
    //
    }
    
    return (w + 4);
}

static void env_tilde_initialize (void *lhs, void *rhs)
{
    t_env_tilde *x   = (t_env_tilde *)lhs;
    t_env_tilde *old = (t_env_tilde *)rhs;
    
    if (x->x_window == old->x_window) {
    if (x->x_period == old->x_period) {
    //
    x->x_phase = old->x_phase;
    memcpy (x->x_sum, old->x_sum, (ENV_MAXIMUM_OVERLAP + 1) * sizeof (t_sample));
    memcpy (x->x_vector, old->x_vector, x->x_window * sizeof (t_sample));
    //
    }
    }
}

static void env_tilde_dsp (t_env_tilde *x, t_signal **sp)
{
    if (x->x_period % sp[0]->s_vectorSize)      { error_invalid (sym_env__tilde__, sym_period); }
    else if (x->x_window < sp[0]->s_vectorSize) { error_invalid (sym_env__tilde__, sym_window); }
    else {
    //
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_env_tilde *old = (t_env_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (env_tilde_initialize, x, old);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    dsp_add3 (env_tilde_perform, x, sp[0]->s_vector, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *env_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_env_tilde *x = (t_env_tilde *)z;
    t_buffer *b = buffer_new();
    
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void env_tilde_functionDismiss (t_object *z)
{
    t_env_tilde *x = (t_env_tilde *)z; x->x_dismissed = 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *env_tilde_new (t_float f1, t_float f2)
{
    t_env_tilde *x = (t_env_tilde *)pd_new (env_tilde_class);
    
    int window = f1;
    int period = f2;
    int i, k = 0;
    
    if (window < 2) { window = 1024; }
    else if (!PD_IS_POWER_2 (window))  { window = (int)PD_NEXT_POWER_2 (window); }
    if (period < 1 || period > window) { period = window / 2; }
    else if (!PD_IS_POWER_2 (period))  { period = (int)PD_NEXT_POWER_2 (period); }
    
    x->x_phase  = -1;
    x->x_period = PD_MAX (period, (window / ENV_MAXIMUM_OVERLAP));
    x->x_window = window;
    x->x_vector = (t_sample *)PD_MEMORY_GET (x->x_window * sizeof (t_sample));
    x->x_clock  = clock_new ((void *)x, (t_method)env_tilde_task);
    x->x_outlet = outlet_newFloat (cast_object (x));

    if ((int)f1 && x->x_window != (int)f1) { warning_invalid (sym_env__tilde__, sym_window); k = 1; }
    if ((int)f2 && x->x_period != (int)f2) { warning_invalid (sym_env__tilde__, sym_period); k = 1; }
    if (k) { 
        post ("%s: window %d period %d", sym_env__tilde__->s_name, x->x_window, x->x_period);   // --
    }
    
    /* Hanning window. */
    
    for (i = 0; i < x->x_window; i++) { 
        x->x_vector[i] = (t_sample)((1.0 - cos ((PD_TWO_PI * i) / x->x_window)) / x->x_window);
    }
    
    return x;
}

static void env_tilde_free (t_env_tilde *x)
{
    clock_free (x->x_clock);
    
    PD_MEMORY_FREE (x->x_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void env_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_env__tilde__,
            (t_newmethod)env_tilde_new,
            (t_method)env_tilde_free,
            sizeof (t_env_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDSP (c, (t_method)env_tilde_dsp);
    
    class_setDataFunction (c, env_tilde_functionData);
    class_setDismissFunction (c, env_tilde_functionDismiss);

    env_tilde_class = c;
}

PD_LOCAL void env_tilde_destroy (void)
{
    class_free (env_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
