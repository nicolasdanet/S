
/* Copyright (c) 1997 Miller Puckette and others. */

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

static t_class *delread_tilde_class;                /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _delread_tilde {
    t_object            x_obj;                      /* Must be the first. */
    t_float64Atomic     x_delayInMilliseconds;
    t_symbol            *x_name;
    t_outlet            *x_outlet;
    } t_delread_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void delread_tilde_float (t_delread_tilde *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_delayInMilliseconds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *delread_tilde_perform (t_int *w)
{
    t_delread_tilde *x          = (t_delread_tilde *)(w[1]);
    t_delwrite_tilde_control *c = (t_delwrite_tilde_control *)(w[2]);
    PD_RESTRICTED out           = (t_sample *)(w[3]);
    t_space *t                  = (t_space *)(w[4]);
    int n = (int)(w[5]);
    
    /* Note that an offset of a vector size is added in non-recirculating cases. */
    
    t_float delayInMilliseconds = PD_ATOMIC_FLOAT64_READ (&x->x_delayInMilliseconds);
    int delayInSample           = (int)((delayInMilliseconds * t->s_float0) + 0.5) + (int)(t->s_float1);
    int phase                   = c->dw_phase - PD_CLAMP (delayInSample, n, c->dw_size);
    
    if (phase < 0) { phase += c->dw_size; }
    
    {
        PD_RESTRICTED p = c->dw_vector + phase;

        while (n--) {
            *out++ = *p++;
            if (p == c->dw_vector + (c->dw_size + DELAY_EXTRA_SAMPLES)) {
                p -= c->dw_size;
            }
        }
    }
    
    return (w + 6);
}

static void delread_tilde_dsp (t_delread_tilde *x, t_signal **sp)
{
    t_delwrite_tilde *m = (t_delwrite_tilde *)symbol_getThingByClass (x->x_name, delwrite_tilde_class);
    
    if (!m) { if (x->x_name != &s_) { error_canNotFind (cast_object (x), sym_delread__tilde__, x->x_name); } }
    else {
    //
    t_space *t  = space_new (cast_object (x));
    t_id build  = chain_getIdentifier (instance_chainGetTemporary());
    
    t->s_float0 = (t_float)(sp[0]->s_sampleRate * 0.001);
    t->s_float1 = (m->dw_identifier == build ? sp[0]->s_vectorSize : 0.0);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_delread_tilde *old = (t_delread_tilde *)garbage_fetch (cast_object (x));
    
    if (old) { delread_tilde_float (x, PD_ATOMIC_FLOAT64_READ (&old->x_delayInMilliseconds)); }
    //
    }
    
    dsp_add5 (delread_tilde_perform, x, &m->dw_space, sp[0]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *delread_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_delread_tilde *x = (t_delread_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, &s_float);
    buffer_appendFloat (b, PD_ATOMIC_FLOAT64_READ (&x->x_delayInMilliseconds));
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *delread_tilde_new (t_symbol *s, t_float f)
{
    t_delread_tilde *x = (t_delread_tilde *)pd_new (delread_tilde_class);
    
    x->x_name   = s;
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_delayInMilliseconds);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void delread_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_delread__tilde__,
            (t_newmethod)delread_tilde_new,
            NULL,
            sizeof (t_delread_tilde),
            CLASS_DEFAULT,
            A_DEFSYMBOL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)delread_tilde_dsp);
    
    class_addFloat (c, (t_method)delread_tilde_float);
    
    class_setDataFunction (c, delread_tilde_functionData);

    delread_tilde_class = c;
}

void delread_tilde_destroy (void)
{
    class_free (delread_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

