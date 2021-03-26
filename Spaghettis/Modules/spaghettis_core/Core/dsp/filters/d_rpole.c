
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_filters.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Real one-pole filter. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/One_Pole.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *rpole_tilde_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _real_raw_tilde t_rpole_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */
/* Notice that the two signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static t_int *rpole_tilde_perform (t_int *w)
{
    t_rpole_tilde *x  = (t_rpole_tilde *)(w[1]);
    PD_RESTRICTED in1 = (t_sample *)(w[2]);
    PD_RESTRICTED in2 = (t_sample *)(w[3]);
    PD_RESTRICTED out = (t_sample *)(w[4]);
    int n = (int)(w[5]);
    
    t_sample last = x->x_real;
    
    while (n--) {
        t_sample a = (*in2++);
        t_sample f = (*in1++) + a * last;
        *out++ = last = f;
    }
    
    if (denormal_isBigOrSmall (last)) { last = 0.0; }
        
    x->x_real = last;
    
    return (w + 6);
}

static void real_raw_initialize (void *lhs, void *rhs)
{
    struct _real_raw_tilde *x   = (struct _real_raw_tilde *)lhs;
    struct _real_raw_tilde *old = (struct _real_raw_tilde *)rhs;
    
    x->x_real = old->x_real;
}

PD_LOCAL void real_raw_initializer (t_object *x)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    struct _real_raw_tilde *old = (struct _real_raw_tilde *)garbage_fetch (x);
    
    if (old) {
    //
    initializer_new (real_raw_initialize, x, old);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
}

static void rpole_tilde_dsp (t_rpole_tilde *x, t_signal **sp)
{
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    
    real_raw_initializer (cast_object (x));
    
    dsp_add5 (rpole_tilde_perform, x,
        sp[0]->s_vector,
        sp[1]->s_vector,
        sp[2]->s_vector,
        sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_buffer *real_raw_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    struct _real_raw_tilde *x = (struct _real_raw_tilde *)z;
    
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

static void *rpole_tilde_new (t_float f)
{
    t_rpole_tilde *x = (t_rpole_tilde *)pd_new (rpole_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    pd_float ((t_pd *)inlet_newSignal (cast_object (x)), f);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void rpole_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_rpole__tilde__,
            (t_newmethod)rpole_tilde_new,
            NULL,
            sizeof (t_rpole_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)rpole_tilde_dsp);
    
    class_setDataFunction (c, real_raw_functionData);
    
    rpole_tilde_class = c;
}

PD_LOCAL void rpole_tilde_destroy (void)
{
    class_free (rpole_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
