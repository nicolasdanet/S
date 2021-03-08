
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *threshold_tilde_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _threshold_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_spin              x_mutex;
    t_float             x_high;
    t_float             x_low;
    t_float             x_deadTimeHigh;
    t_float             x_deadTimeLow;
    int                 x_set;
    int                 x_state;
    int                 x_dismissed;
    t_float             x_wait;
    t_clock             *x_clockLeft;
    t_clock             *x_clockRight;
    t_outlet            *x_outletLeft;
    t_outlet            *x_outletRight;
    } t_threshold_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void threshold_tilde_taskLeft (t_threshold_tilde *x)
{
    if (!x->x_dismissed) { outlet_bang (x->x_outletLeft); }
}

static void threshold_tilde_taskRight (t_threshold_tilde *x)
{
    if (!x->x_dismissed) { outlet_bang (x->x_outletRight); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void threshold_tilde_setProceed (t_threshold_tilde *x,
    t_float high,
    t_float highDead,
    t_float low,
    t_float lowDead)
{
    spin_lock (&x->x_mutex);
    
        x->x_high         = high;
        x->x_low          = low;
        x->x_deadTimeHigh = highDead;
        x->x_deadTimeLow  = lowDead;
        x->x_set          = 1;
    
    spin_unlock (&x->x_mutex);
}

static void threshold_tilde_set (t_threshold_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float high      = atom_getFloatAtIndex (0, argc, argv);
    t_float highDead  = atom_getFloatAtIndex (1, argc, argv);
    t_float low       = atom_getFloatAtIndex (2, argc, argv);
    t_float lowDead   = atom_getFloatAtIndex (3, argc, argv);
    
    threshold_tilde_setProceed (x, high, highDead, low, lowDead);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *threshold_tilde_perform (t_int *w)
{
    t_threshold_tilde *x = (t_threshold_tilde *)(w[1]);
    PD_RESTRICTED in     = (t_sample *)(w[2]);
    t_space *t           = (t_space *)(w[3]);
    int n = (int)(w[4]);
    
    if (spin_trylock (&x->x_mutex) == 0) {
    //
    if (x->x_set) {
        t->s_float1 = x->x_high;
        t->s_float2 = x->x_low;
        t->s_float3 = x->x_deadTimeHigh;
        t->s_float4 = x->x_deadTimeLow;
        x->x_set    = 0;
    }
    
    spin_unlock (&x->x_mutex);
    //
    }
    
    if (x->x_wait > 0.0) { x->x_wait -= t->s_float0; }
    else if (x->x_state) {
    
        while (n--) {
        //
        if ((*in++) < t->s_float2) {
            x->x_state = 0;
            x->x_wait  = t->s_float4;
            clock_delay (x->x_clockRight, 0.0);
            break;
        }
        //
        }
        
    } else {
 
        while (n--) {
        //
        if ((*in++) >= t->s_float1) {
            x->x_state = 1;
            x->x_wait  = t->s_float3;
            clock_delay (x->x_clockLeft, 0.0);
            break;
        }
        //
        }
    }
    
    return (w + 5);
}

static void threshold_tilde_initialize (void *lhs, void *rhs)
{
    t_threshold_tilde *x   = (t_threshold_tilde *)lhs;
    t_threshold_tilde *old = (t_threshold_tilde *)rhs;
    
    x->x_state = old->x_state;
    x->x_wait  = old->x_wait;
}

static void threshold_tilde_dsp (t_threshold_tilde *x, t_signal **sp)
{
    t_space *t = space_new (cast_object (x));
    
    t->s_float0 = (t_float)(1000.0 * sp[0]->s_vectorSize / sp[0]->s_sampleRate);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_threshold_tilde *old = (t_threshold_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (threshold_tilde_initialize, x, old);
    
    threshold_tilde_setProceed (x, old->x_high, old->x_deadTimeHigh, old->x_low, old->x_deadTimeLow);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    // spin_lock (&x->x_mutex);
    
        t->s_float1 = x->x_high;
        t->s_float2 = x->x_low;
        t->s_float3 = x->x_deadTimeHigh;
        t->s_float4 = x->x_deadTimeLow;
    
    // spin_unlock (&x->x_mutex);
    
    dsp_add4 (threshold_tilde_perform, x, sp[0]->s_vector, t, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *threshold_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_threshold_tilde *x = (t_threshold_tilde *)z;
    t_buffer *b = buffer_new();
    
    t_float f1, f2, f3, f4;
    
    // spin_lock (&x->x_mutex);
    
        f1 = x->x_high;
        f2 = x->x_deadTimeHigh;
        f3 = x->x_low;
        f4 = x->x_deadTimeLow;
    
    // spin_unlock (&x->x_mutex);
    
    buffer_appendSymbol (b, sym_set);
    buffer_appendFloat (b, f1);
    buffer_appendFloat (b, f2);
    buffer_appendFloat (b, f3);
    buffer_appendFloat (b, f4);
    
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void threshold_tilde_functionDismiss (t_object *z)
{
    t_threshold_tilde *x = (t_threshold_tilde *)z; x->x_dismissed = 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *threshold_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    t_threshold_tilde *x = (t_threshold_tilde *)pd_new (threshold_tilde_class);
    
    spin_init (&x->x_mutex);
    
    x->x_clockLeft   = clock_new ((void *)x, (t_method)threshold_tilde_taskLeft);
    x->x_clockRight  = clock_new ((void *)x, (t_method)threshold_tilde_taskRight);
    x->x_outletLeft  = outlet_newBang (cast_object (x));
    x->x_outletRight = outlet_newBang (cast_object (x));
    
    threshold_tilde_set (x, s, argc, argv);
    
    return x;
}

static void threshold_tilde_free (t_threshold_tilde *x)
{
    clock_free (x->x_clockRight);
    clock_free (x->x_clockLeft);
    
    spin_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void threshold_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_threshold__tilde__,
            (t_newmethod)threshold_tilde_new,
            (t_method)threshold_tilde_free,
            sizeof (t_threshold_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_GIMME,
            A_NULL);
            
    class_addDSP (c, (t_method)threshold_tilde_dsp);
        
    class_addMethod (c, (t_method)threshold_tilde_set, sym_set, A_GIMME, A_NULL);

    class_setDataFunction (c, threshold_tilde_functionData);
    class_setDismissFunction (c, threshold_tilde_functionDismiss);

    threshold_tilde_class = c;
}

PD_LOCAL void threshold_tilde_destroy (void)
{
    class_free (threshold_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
