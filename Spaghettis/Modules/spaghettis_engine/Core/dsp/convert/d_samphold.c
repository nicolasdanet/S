
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *samphold_tilde_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _samphold_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_float64Atomic     x_lastControl;
    t_float64Atomic     x_lastOut;
    t_outlet            *x_outlet;
    } t_samphold_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */
/* Notice that the two signals incoming could be theoretically just one. */
/* But as only loads are done, it is assumed safe to use restricted pointers. */

static t_int *samphold_tilde_perform (t_int *w)
{
    t_samphold_tilde *x = (t_samphold_tilde *)(w[1]);
    PD_RESTRICTED in1 = (t_sample *)(w[2]);
    PD_RESTRICTED in2 = (t_sample *)(w[3]);
    PD_RESTRICTED out = (t_sample *)(w[4]);
    int i, n = (int)(w[5]);
    
    t_sample lastControl = (t_sample)atomic_float64Read (&x->x_lastControl);
    t_sample lastOut     = (t_sample)atomic_float64Read (&x->x_lastOut);
        
    for (i = 0; i < n; i++) {
    //
    t_sample f = *in2;
    
    if (f < lastControl) { lastOut = *in1; }
    *out++ = lastOut;
    lastControl = f;
    
    in1++;
    in2++;
    //
    }
    
    atomic_float64Write (&x->x_lastControl, (double)lastControl);
    atomic_float64Write (&x->x_lastOut, (double)lastOut);
    
    return (w + 6);
}

static void samphold_tilde_initialize (void *lhs, void *rhs)
{
    t_samphold_tilde *x   = (t_samphold_tilde *)lhs;
    t_samphold_tilde *old = (t_samphold_tilde *)rhs;
    
    atomic_float64Write (&x->x_lastControl, atomic_float64Read (&old->x_lastControl));
    atomic_float64Write (&x->x_lastOut, atomic_float64Read (&old->x_lastOut));
}

static void samphold_tilde_dsp (t_samphold_tilde *x, t_signal **sp)
{
    PD_ASSERT (sp[0]->s_vector != sp[2]->s_vector);
    PD_ASSERT (sp[1]->s_vector != sp[2]->s_vector);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_samphold_tilde *old = (t_samphold_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (samphold_tilde_initialize, x, old);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    dsp_add5 (samphold_tilde_perform, x,
        sp[0]->s_vector,
        sp[1]->s_vector,
        sp[2]->s_vector, 
        sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *samphold_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_samphold_tilde *x = (t_samphold_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b, atomic_float64Read (&x->x_lastControl));
    buffer_appendFloat (b, atomic_float64Read (&x->x_lastOut));
    
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void samphold_tilde_restore (t_samphold_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float f0 = atom_getFloatAtIndex (0, argc, argv);
    t_float f1 = atom_getFloatAtIndex (1, argc, argv);
    
    atomic_float64Write (&x->x_lastControl, f0);
    atomic_float64Write (&x->x_lastOut, f1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *samphold_tilde_new (void)
{
    t_samphold_tilde *x = (t_samphold_tilde *)pd_new (samphold_tilde_class);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
        
    inlet_newSignal (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void samphold_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_samphold__tilde__,
            (t_newmethod)samphold_tilde_new,
            NULL,
            sizeof (t_samphold_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_NULL);
                
    class_addDsp (c, (t_method)samphold_tilde_dsp);
    
    class_addMethod (c, (t_method)samphold_tilde_restore, sym__restore, A_GIMME, A_NULL);

    class_setDataFunction (c, samphold_tilde_functionData);

    samphold_tilde_class = c;
}

void samphold_tilde_destroy (void)
{
    class_free (samphold_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
