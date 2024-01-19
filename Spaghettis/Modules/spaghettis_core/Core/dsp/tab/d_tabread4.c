
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

static t_class *tabread4_tilde_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabread4_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_trylock           x_mutex;
    t_float64Atomic     x_onset;
    int                 x_set;
    int                 x_size;
    t_word              *x_vector;
    t_symbol            *x_name;
    t_outlet            *x_outlet;
    } t_tabread4_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabread4_tilde_setProceed (t_tabread4_tilde *x, t_symbol *s, int verbose)
{
    trylock_lock (&x->x_mutex);
    
        t_error err = tab_fetchArray ((x->x_name = s), &x->x_size, &x->x_vector);
    
        x->x_set = 1;
    
    trylock_unlock (&x->x_mutex);
    
    if (verbose && err) { tab_error (cast_object (x), sym_tabread4__tilde__, s); }
}

static void tabread4_tilde_set (t_tabread4_tilde *x, t_symbol *s)
{
    tabread4_tilde_setProceed (x, s, 1);
}

static void tabread4_tilde_restore (t_tabread4_tilde *x, t_symbol *s)
{
    tabread4_tilde_setProceed (x, s, 0);
}

static void tabread4_tilde_onset (t_tabread4_tilde *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_onset);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *tabread4_tilde_perform (t_int *w)
{
    t_tabread4_tilde *x = (t_tabread4_tilde *)(w[1]);
    PD_RESTRICTED in    = (t_sample *)(w[2]);
    PD_RESTRICTED out   = (t_sample *)(w[3]);
    t_space *t          = (t_space *)(w[4]);
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
    
    if (t->s_pointer0 && t->s_int0 > 3) {
    //
    while (n--) {
    //
    double position = (*in++) + (double)PD_ATOMIC_FLOAT64_READ (&x->x_onset);
    int i = (int)position;
    double fractional = position - i;
    
    if (i < 1) { i = 1; fractional = 0.0; }
    else if (i > t->s_int0 - 3) { i = t->s_int0 - 3; fractional = 1.0; }

    *out++ = (t_sample)dsp_4PointsInterpolationWithWords (fractional, (t_word *)t->s_pointer0 + i - 1);
    //
    }
    //
    } else { while (n--) { *out++ = 0.0; } }
    
    return (w + 6);
}

static void tabread4_tilde_dsp (t_tabread4_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_tabread4_tilde *old = (t_tabread4_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    if (x->x_name != old->x_name) { tabread4_tilde_setProceed (x, old->x_name, 1); }
    
    tabread4_tilde_onset (x, PD_ATOMIC_FLOAT64_READ (&old->x_onset));
    
    object_copySignalValues (cast_object (x), cast_object (old));
    //
    }
    //
    }
    
    {
    //
    t_space *t  = space_new (cast_object (x));
    int size    = 0;
    t_word *w   = NULL;
    t_error err = tab_fetchArray (x->x_name, &size, &w);

    if (err) { tab_error (cast_object (x), sym_tabread4__tilde__, x->x_name); }
    else {
        t->s_int0 = size; t->s_pointer0 = (void *)w;
    }

    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    dsp_add5 (tabread4_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabread4_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabread4_tilde *x = (t_tabread4_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, x->x_name);
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__inlet2);
    buffer_appendFloat (b,  PD_ATOMIC_FLOAT64_READ (&x->x_onset));
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

static void *tabread4_tilde_new (t_symbol *s)
{
    t_tabread4_tilde *x = (t_tabread4_tilde *)pd_new (tabread4_tilde_class);
    
    trylock_init (&x->x_mutex);
    
    x->x_name   = s;
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_new2 (x, &s_float);

    return x;
}

static void tabread4_tilde_free (t_tabread4_tilde *x)
{
    trylock_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void tabread4_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabread4__tilde__,
            (t_newmethod)tabread4_tilde_new,
            (t_method)tabread4_tilde_free,
            sizeof (t_tabread4_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addDsp (c, (t_method)tabread4_tilde_dsp);
    
    class_addMethod (c, (t_method)tabread4_tilde_set,       sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabread4_tilde_onset,     sym__inlet2,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)tabread4_tilde_restore,   sym__restore,   A_SYMBOL, A_NULL);
    
    class_setDataFunction (c, tabread4_tilde_functionData);
    
    tabread4_tilde_class = c;
}

void tabread4_tilde_destroy (void)
{
    class_free (tabread4_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
