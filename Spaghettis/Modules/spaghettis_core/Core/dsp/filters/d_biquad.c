
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Biquad filter. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* < https://ccrma.stanford.edu/~jos/filters/Direct_Form_II.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *biquad_tilde_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct biquad_tilde {
    t_object            x_obj;                  /* Must be the first. */
    t_trylock           x_mutex;
    t_sample            x_real1;
    t_sample            x_real2;
    t_float             x_a1;
    t_float             x_a2;
    t_float             x_b0;
    t_float             x_b1;
    t_float             x_b2;
    int                 x_set;
    t_outlet            *x_outlet;
    } t_biquad_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void biquad_tilde_set (t_biquad_tilde *x, t_float a1, t_float a2, t_float b0, t_float b1, t_float b2)
{
    trylock_lock (&x->x_mutex);
    
        x->x_a1  = a1;
        x->x_a2  = a2;
        x->x_b0  = b0;
        x->x_b1  = b1;
        x->x_b2  = b2;
        x->x_set = 1;
    
    trylock_unlock (&x->x_mutex);
}

static void biquad_tilde_list (t_biquad_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float a1 = atom_getFloatAtIndex (0, argc, argv);
    t_float a2 = atom_getFloatAtIndex (1, argc, argv);
    t_float b0 = atom_getFloatAtIndex (2, argc, argv);
    t_float b1 = atom_getFloatAtIndex (3, argc, argv);
    t_float b2 = atom_getFloatAtIndex (4, argc, argv);
    
    biquad_tilde_set (x, a1, a2, b0, b1, b2);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void biquad_tilde_space (t_space *t, t_float a1, t_float a2, t_float b0, t_float b1, t_float b2)
{
    t->s_float0 = a1;
    t->s_float1 = a2;
    t->s_float2 = b0;
    t->s_float3 = b1;
    t->s_float4 = b2;
}

/* No aliasing. */

static t_int *biquad_tilde_perform (t_int *w)
{
    t_biquad_tilde *x = (t_biquad_tilde *)(w[1]);
    PD_RESTRICTED in  = (t_sample *)(w[2]);
    PD_RESTRICTED out = (t_sample *)(w[3]);
    t_space *t        = (t_space *)(w[4]);
    int n = (int)(w[5]);
    
    if (trylock_trylock (&x->x_mutex) == 0) {
    //
    if (x->x_set) { biquad_tilde_space (t, x->x_a1, x->x_a2, x->x_b0, x->x_b1, x->x_b2); x->x_set = 0; }
    
    trylock_unlock (&x->x_mutex);
    //
    }

    t_sample a1 = (t_sample)t->s_float0;
    t_sample a2 = (t_sample)t->s_float1;
    t_sample b0 = (t_sample)t->s_float2;
    t_sample b1 = (t_sample)t->s_float3;
    t_sample b2 = (t_sample)t->s_float4;
    
    t_sample last1 = x->x_real1;
    t_sample last2 = x->x_real2;
    
    while (n--) {
    //
    t_sample f = (*in++) + a1 * last1 + a2 * last2; 
        
    if (denormal_isBigOrSmall (f)) { f = 0.0; }
        
    *out++ = b0 * f + b1 * last1 + b2 * last2;
    last2  = last1;
    last1  = f;
    //
    }
    
    x->x_real1 = last1;
    x->x_real2 = last2;
    
    return (w + 6);
}

static void biquad_tilde_initialize (void *lhs, void *rhs)
{
    t_biquad_tilde *x   = (t_biquad_tilde *)lhs;
    t_biquad_tilde *old = (t_biquad_tilde *)rhs;
    
    x->x_real1 = old->x_real1;
    x->x_real2 = old->x_real2;
}

static void biquad_tilde_dsp (t_biquad_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_biquad_tilde *old = (t_biquad_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (biquad_tilde_initialize, x, old);
    
    biquad_tilde_set (x, old->x_a1, old->x_a2, old->x_b0, old->x_b1, old->x_b2);
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    {
    //
    t_space *t = space_new (cast_object (x));
    
    // trylock_lock (&x->x_mutex);
    
        biquad_tilde_space (t, x->x_a1, x->x_a2, x->x_b0, x->x_b1, x->x_b2);
    
    // trylock_unlock (&x->x_mutex);
    
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    dsp_add5 (biquad_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *biquad_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_biquad_tilde *x = (t_biquad_tilde *)z;
    t_buffer *b = buffer_new();
    
    // trylock_lock (&x->x_mutex);
    
        t_float a1 = x->x_a1;
        t_float a2 = x->x_a2;
        t_float b0 = x->x_b0;
        t_float b1 = x->x_b1;
        t_float b2 = x->x_b2;
        
    // trylock_unlock (&x->x_mutex);
    
    buffer_appendSymbol (b, &s_list);
    buffer_appendFloat (b,  a1);
    buffer_appendFloat (b,  a2);
    buffer_appendFloat (b,  b0);
    buffer_appendFloat (b,  b1);
    buffer_appendFloat (b,  b2);
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

static void *biquad_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    t_biquad_tilde *x = (t_biquad_tilde *)pd_new (biquad_tilde_class);
    
    trylock_init (&x->x_mutex);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    biquad_tilde_list (x, s, argc, argv);

    return x;
}

static void biquad_tilde_free (t_biquad_tilde *x)
{
    trylock_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void biquad_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_biquad__tilde__,
            (t_newmethod)biquad_tilde_new,
            (t_method)biquad_tilde_free,
            sizeof (t_biquad_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_GIMME,
            A_NULL);
            
    class_addDSP (c, (t_method)biquad_tilde_dsp);
    class_addList (c, (t_method)biquad_tilde_list);
    
    class_setDataFunction (c, biquad_tilde_functionData);
    
    biquad_tilde_class = c;
}

PD_LOCAL void biquad_tilde_destroy (void)
{
    class_free (biquad_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
