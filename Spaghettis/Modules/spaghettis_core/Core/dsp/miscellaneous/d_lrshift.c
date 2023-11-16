
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *lrshift_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _lrshift_tilde {
    t_object    x_obj;                      /* Must be the first. */
    int         x_n;
    t_outlet    *x_outlet;
    } t_lrshift_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *lrshift_tilde_peformShiftRight (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    int shift = (int)(w[4]);
    
    n  -= shift;
    in -= shift;
    
    while (n--)     { --out; --in; *out = *in; }
    while (shift--) { --out; *out = 0.0; }
    
    return (w + 5);
}

/* No aliasing. */

static t_int *lrshift_tilde_peformShiftLeft (t_int *w)
{
    PD_RESTRICTED in  = (t_sample *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    int shift = (int)(w[4]);
    
    in += shift;
    n  -= shift;
    
    while (n--)     { *out++ = *in++; }
    while (shift--) { *out++ = 0.0; }
    
    return (w + 5);
}

static void lrshift_tilde_dsp (t_lrshift_tilde *x, t_signal **sp)
{
    int size  = sp[0]->s_vectorSize;
    int shift = PD_CLAMP (x->x_n, -size, size);

    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    if (shift < 0) {
    
        dsp_add4 (lrshift_tilde_peformShiftRight,
            sp[0]->s_vector + size,
            sp[1]->s_vector + size,
            size,
            PD_ABS (shift));
            
    } else {
    
        dsp_add4 (lrshift_tilde_peformShiftLeft,
            sp[0]->s_vector,
            sp[1]->s_vector,
            size,
            PD_ABS (shift));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *lrshift_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_lrshift_tilde *x = (t_lrshift_tilde *)z;
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

static void *lrshift_tilde_new (t_float f)
{
    t_lrshift_tilde *x = (t_lrshift_tilde *)pd_new (lrshift_tilde_class);
    
    x->x_n = (int)f;
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void lrshift_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_lrshift__tilde__,
            (t_newmethod)lrshift_tilde_new,
            NULL, 
            sizeof (t_lrshift_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)lrshift_tilde_dsp);
    
    class_setDataFunction (c, lrshift_tilde_functionData);
    
    lrshift_tilde_class = c;
}

void lrshift_tilde_destroy (void)
{
    class_free (lrshift_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
