
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_delay.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *vd_tilde_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _vd_tilde {
    t_object    x_obj;                      /* Must be the first. */
    t_symbol    *x_name;
    t_outlet    *x_outlet;
    } t_vd_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *vd_tilde_perform (t_int *w)
{
    t_delwrite_tilde_control *c = (t_delwrite_tilde_control *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    t_space *t        = (t_space *)(w[4]);
    int n = (int)(w[5]);

    t_float limit = c->dw_size - n;
    
    while (n--) {
    //
    /* Note that the offset size is reported as zero in non-recirculating cases. */

    t_float f0             = (t_float)(*in++);
    t_float f1             = (f0 * t->s_float0) - t->s_float1;
    t_float delayInSamples = (t_float)(PD_CLAMP (f1, 1.0, limit) + n);
    int integer            = (int)delayInSamples;
    t_float fractional     = delayInSamples - (t_float)integer;
    PD_RESTRICTED p        = c->dw_vector + (c->dw_phase - integer);
        
    if (p < c->dw_vector + DELAY_EXTRA_SAMPLES) { p += c->dw_size; }
        
    *out++ = (t_sample)dsp_4PointsInterpolation (fractional, p[0], p[-1], p[-2], p[-3]);
    //
    }
    
    return (w + 6);
}

static void vd_tilde_dsp (t_vd_tilde *x, t_signal **sp)
{
    t_delwrite_tilde *m = (t_delwrite_tilde *)symbol_getThingByClass (x->x_name, delwrite_tilde_class);
    
    if (!m) { if (x->x_name != &s_) { error_canNotFind (cast_object (x), sym_vd__tilde__, x->x_name); } }
    else {
    //
    t_space *t  = space_new (cast_object (x));
    t_id build  = chain_getIdentifier (instance_chainGetTemporary());
    
    t->s_float0 = (t_float)(sp[0]->s_sampleRate * 0.001);
    t->s_float1 = (m->dw_identifier == build ? 0.0 : sp[0]->s_vectorSize);
    
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_add5 (vd_tilde_perform, &m->dw_space, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *vd_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_vd_tilde *x = (t_vd_tilde *)z;
    t_buffer *b = buffer_new();
    
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *vd_tilde_new (t_symbol *s)
{
    t_vd_tilde *x = (t_vd_tilde *)pd_new (vd_tilde_class);
    
    x->x_name   = s;
    x->x_outlet = outlet_newSignal (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void vd_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_vd__tilde__,
            (t_newmethod)vd_tilde_new,
            NULL,
            sizeof (t_vd_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_NULL);
    
    class_addDsp (c, (t_method)vd_tilde_dsp);
    
    class_setDataFunction (c, vd_tilde_functionData);
    
    vd_tilde_class = c;
}

void vd_tilde_destroy (void)
{
    class_free (vd_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

