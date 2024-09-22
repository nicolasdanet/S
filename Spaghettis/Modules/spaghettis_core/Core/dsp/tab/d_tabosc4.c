
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_tab.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *tabosc4_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabosc4_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_trylock           x_mutex;
    t_float             x_phase;
    int                 x_set;
    int                 x_size;
    t_word              *x_vector;
    t_symbol            *x_name;
    t_outlet            *x_outlet;
    } t_tabosc4_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabosc4_tilde_setProceed (t_tabosc4_tilde *x, t_symbol *s, int verbose)
{
    trylock_lock (&x->x_mutex);
    
        int n, size;
    
        t_error err1 = tab_fetchArray ((x->x_name = s), &n, &x->x_vector);
        t_error err2 = PD_ERROR;

        size = n - 3;   /* Size of the usable part of the table. */
    
        if (size > 0 && PD_IS_POWER_2 (size)) { x->x_size = size; err2 = PD_ERROR_NONE; }
        else {
            x->x_size = 0; x->x_vector = NULL;
        }
    
        x->x_set = 1;
    
    trylock_unlock (&x->x_mutex);
    
    if (verbose) { tab_errorProceed (cast_object (x), sym_tabosc4__tilde__, s, err1, err2); }
}

static void tabosc4_tilde_set (t_tabosc4_tilde *x, t_symbol *s)
{
    tabosc4_tilde_setProceed (x, s, 1);
}

static void tabosc4_tilde_restore (t_tabosc4_tilde *x, t_symbol *s)
{
    tabosc4_tilde_setProceed (x, s, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *tabosc4_tilde_perform (t_int *w)
{
    t_tabosc4_tilde *x = (t_tabosc4_tilde *)(w[1]);
    PD_RESTRICTED in   = (t_sample *)(w[2]);
    PD_RESTRICTED out  = (t_sample *)(w[3]);
    t_space *t         = (t_space *)(w[4]);
    int n = (int)(w[5]);
    
    if (trylock_trylock (&x->x_mutex)) {
    //
    if (x->x_set) {
        t->s_int0     = x->x_size;
        t->s_pointer0 = (void *)x->x_vector;
        x->x_set      = 0;
    }
    
    trylock_unlock (&x->x_mutex);
    //
    }
    
    if (t->s_pointer0) {
    //
    const int size = t->s_int0;
    double phase   = (size * x->x_phase) + DSP_UNITBIT;
    
    t_pun64 z;
    
    while (n--) {
    //
    pun64_setDouble (&z, phase);
    phase += (*in++) * size * t->s_float0;
    t_word *p = (t_word *)t->s_pointer0 + (pun64_getMostSignificantBytes (&z) & (size - 1));
    pun64_setMostSignificantBytes (&z, DSP_UNITBIT_MSB);
    *out++ = (t_sample)dsp_4PointsInterpolationWithWords ((t_float)(pun64_getDouble (&z) - DSP_UNITBIT), p);
    //
    }

    /* Wrap the phase (keep only the fractional part). */
    /* Size must be a power of two. */
    
    pun64_setDouble (&z, DSP_UNITBIT * size);
    uint32_t k = pun64_getMostSignificantBytes (&z);
    pun64_setDouble (&z, phase + (DSP_UNITBIT * size - DSP_UNITBIT));
    pun64_setMostSignificantBytes (&z, k);
    x->x_phase = (pun64_getDouble (&z) - DSP_UNITBIT * size) * (1.0 / size);
    //
    } else { while (n--) { *out++ = 0.0; } }
    
    return (w + 6);
}

static void tabosc4_tilde_initialize (void *lhs, void *rhs)
{
    t_tabosc4_tilde *x   = (t_tabosc4_tilde *)lhs;
    t_tabosc4_tilde *old = (t_tabosc4_tilde *)rhs;
    
    x->x_phase = old->x_phase;
}

static void tabosc4_tilde_dsp (t_tabosc4_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_tabosc4_tilde *old = (t_tabosc4_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (tabosc4_tilde_initialize, x, old);
    
    if (x->x_name != old->x_name) { tabosc4_tilde_setProceed (x, old->x_name, 1); }

    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    {
    //
    t_space *t   = space_new (cast_object (x));
    int n        = 0;
    t_word *w    = NULL;
    t_error err1 = tab_fetchArray (x->x_name, &n, &w);
    t_error err2 = PD_ERROR_NONE;
    
    if (!err1) {
    //
    int size = n - 3;   /* Size of the usable part of the table. */
    
    if (size > 0 && PD_IS_POWER_2 (size)) { t->s_int0 = size; t->s_pointer0 = (void *)w; }
    else {
        err2 = PD_ERROR;
    }
    //
    }

    tab_errorProceed (cast_object (x), sym_tabread4__tilde__, x->x_name, err1, err2);
    
    t->s_float0 = (t_float)(1.0 / sp[0]->s_sampleRate);
    
    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    dsp_add5 (tabosc4_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabosc4_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabosc4_tilde *x = (t_tabosc4_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, x->x_name);
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

static void *tabosc4_tilde_new (t_symbol *s)
{
    t_tabosc4_tilde *x = (t_tabosc4_tilde *)pd_new (tabosc4_tilde_class);
    
    trylock_init (&x->x_mutex);
    
    x->x_name   = s;
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    return x;
}

static void tabosc4_tilde_free (t_tabosc4_tilde *x)
{
    trylock_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void tabosc4_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabosc4__tilde__,
            (t_newmethod)tabosc4_tilde_new,
            (t_method)tabosc4_tilde_free,
            sizeof (t_tabosc4_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addDsp (c, (t_method)tabosc4_tilde_dsp);
    
    class_addMethod (c, (t_method)tabosc4_tilde_set,        sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabosc4_tilde_restore,    sym__restore,   A_SYMBOL, A_NULL);

    class_setDataFunction (c, tabosc4_tilde_functionData);
    
    tabosc4_tilde_class = c;
}

void tabosc4_tilde_destroy (void)
{
    class_free (tabosc4_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
