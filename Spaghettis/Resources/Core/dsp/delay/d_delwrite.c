
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_delay.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void delwrite_tilde_dismiss (t_delwrite_tilde *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *delwrite_tilde_class;      /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *delwrite_tilde_perform (t_int *w)
{
    t_delwrite_tilde_control *c = (t_delwrite_tilde_control *)(w[1]);
    PD_RESTRICTED in = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    int phase = c->dw_phase;
    
    PD_RESTRICTED p = c->dw_vector + phase;
    
    phase += n;

    while (n--) {
    //
    t_sample f = *in++;
    
    if (denormal_isBigOrSmall (f)) { f = 0.0; }
    
    *p++ = f;
    
    /* Make a copy of the last four ending points at start. */
    /* Required for 4-points interpolation. */
    
    if (p == c->dw_vector + (c->dw_size + DELAY_EXTRA_SAMPLES)) {
    //
    t_sample f1 = *(p - 4);
    t_sample f2 = *(p - 3);
    t_sample f3 = *(p - 2);
    t_sample f4 = *(p - 1);
    
    p = c->dw_vector;
    
    *p++ = f1;
    *p++ = f2;
    *p++ = f3;
    *p++ = f4;  // DELAY_EXTRA_SAMPLES
    
    phase -= c->dw_size;
    //
    }
    //
    }
    
    c->dw_phase = phase;
    
    return (w + 4);
}

static void delwrite_tilde_initialize (void *lhs, void *rhs)
{
    t_delwrite_tilde *x   = (t_delwrite_tilde *)lhs;
    t_delwrite_tilde *old = (t_delwrite_tilde *)rhs;
    
    if (x->dw_space.dw_size      == old->dw_space.dw_size) {
    if (x->dw_space.dw_allocated == old->dw_space.dw_allocated) {
    //
    x->dw_space.dw_phase = old->dw_space.dw_phase;
    memcpy (x->dw_space.dw_vector, old->dw_space.dw_vector, x->dw_space.dw_allocated * sizeof (t_sample));
    //
    }
    }
}

static void delwrite_tilde_dsp (t_delwrite_tilde *x, t_signal **sp)
{
    x->dw_identifier = chain_getIdentifier (instance_chainGetTemporary());
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_delwrite_tilde *old = (t_delwrite_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (delwrite_tilde_initialize, x, old);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    dsp_add3 (delwrite_tilde_perform, &x->dw_space, sp[0]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *delwrite_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_delwrite_tilde *x = (t_delwrite_tilde *)z;
    t_buffer *b = buffer_new();
    
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void delwrite_tilde_functionDismiss (t_object *z)
{
    delwrite_tilde_dismiss ((t_delwrite_tilde *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void delwrite_tilde_newAllocate (t_delwrite_tilde *x, t_float milliseconds)
{
    int allocated, n = (int)(PD_MILLISECONDS_TO_SECONDS (milliseconds) * AUDIO_DEFAULT_SAMPLERATE);
    
    n = PD_MAX (1, n);
    n += ((- n) & (DELAY_ROUND_SAMPLES - 1));   /* Snap to the next multiple of DELAY_ROUND_SAMPLES. */
    n += INTERNAL_BLOCKSIZE;

    allocated = n + DELAY_EXTRA_SAMPLES;
    
    x->dw_space.dw_vector    = (t_sample *)PD_MEMORY_GET (allocated * sizeof (t_sample));
    x->dw_space.dw_size      = n;
    x->dw_space.dw_phase     = DELAY_EXTRA_SAMPLES;
    x->dw_space.dw_allocated = allocated;
}

static void *delwrite_tilde_new (t_symbol *s, t_float milliseconds)
{
    t_delwrite_tilde *x = (t_delwrite_tilde *)pd_new (delwrite_tilde_class);

    x->dw_name = (s == &s_) ? sym_delwrite__tilde__ : s;
    
    delwrite_tilde_newAllocate (x, milliseconds);
    
    pd_bind (cast_pd (x), x->dw_name);
    
    return x;
}

static void delwrite_tilde_dismiss (t_delwrite_tilde *x)
{
    if (x->dw_name) { pd_unbind (cast_pd (x), x->dw_name); x->dw_name = NULL; }
}

static void delwrite_tilde_free (t_delwrite_tilde *x)
{
    delwrite_tilde_dismiss (x); PD_MEMORY_FREE (x->dw_space.dw_vector);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void delwrite_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_delwrite__tilde__, 
            (t_newmethod)delwrite_tilde_new,
            (t_method)delwrite_tilde_free,
            sizeof (t_delwrite_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDSP (c, (t_method)delwrite_tilde_dsp);

    class_setDataFunction (c, delwrite_tilde_functionData);
    class_setDismissFunction (c, delwrite_tilde_functionDismiss);

    delwrite_tilde_class = c;
}

PD_LOCAL void delwrite_tilde_destroy (void)
{
    class_free (delwrite_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

