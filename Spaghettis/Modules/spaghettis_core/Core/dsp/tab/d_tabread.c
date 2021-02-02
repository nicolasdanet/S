
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "d_tab.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *tabread_tilde_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabread_tilde {
    t_object            x_obj;                  /* Must be the first. */
    pthread_mutex_t     x_mutex;
    int                 x_set;
    int                 x_size;
    t_word              *x_vector;
    t_symbol            *x_name;
    t_outlet            *x_outlet;
    } t_tabread_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabread_tilde_setProceed (t_tabread_tilde *x, t_symbol *s, int verbose)
{
    pthread_mutex_lock (&x->x_mutex);
    
        t_error err = tab_fetchArray ((x->x_name = s), &x->x_size, &x->x_vector);
    
        x->x_set = 1;
    
    pthread_mutex_unlock (&x->x_mutex);
    
    if (verbose && err) { tab_error (sym_tabread__tilde__, s); }
}

static void tabread_tilde_set (t_tabread_tilde *x, t_symbol *s)
{
    tabread_tilde_setProceed (x, s, 1);
}

static void tabread_tilde_restore (t_tabread_tilde *x, t_symbol *s)
{
    tabread_tilde_setProceed (x, s, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *tabread_tilde_perform (t_int *w)
{
    t_tabread_tilde *x = (t_tabread_tilde *)(w[1]);
    PD_RESTRICTED in   = (t_sample *)(w[2]);
    PD_RESTRICTED out  = (t_sample *)(w[3]);
    t_space *t         = (t_space *)(w[4]);
    int n = (int)(w[5]);
    
    if (pthread_mutex_trylock (&x->x_mutex) == 0) {
    //
    if (x->x_set) {
        t->s_int0     = x->x_size;
        t->s_pointer0 = (void *)x->x_vector;
        x->x_set      = 0;
    }
    
    pthread_mutex_unlock (&x->x_mutex);
    //
    }
    
    if (t->s_pointer0 && t->s_int0 > 0) {
    //
    while (n--) {
    //
    t_sample f = *in++;
    int i = PD_CLAMP ((int)f, 0, t->s_int0 - 1);
    t_sample g = (t_sample)w_getFloat ((t_word *)t->s_pointer0 + i);
    *out++ = g;
    //
    }
    //
    } else { while (n--) { *out++ = 0; } }

    return (w + 6);
}

static void tabread_tilde_dsp (t_tabread_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_tabread_tilde *old = (t_tabread_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    if (x->x_name != old->x_name) { tabread_tilde_setProceed (x, old->x_name, 1); }
    
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

    if (err) { tab_error (sym_tabread__tilde__, x->x_name); }
    else {
        t->s_int0 = size; t->s_pointer0 = (void *)w;
    }

    PD_ASSERT (sp[0]->s_vector != sp[1]->s_vector);
    
    dsp_add5 (tabread_tilde_perform, x, sp[0]->s_vector, sp[1]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabread_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabread_tilde *x = (t_tabread_tilde *)z;
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

static void *tabread_tilde_new (t_symbol *s)
{
    t_tabread_tilde *x = (t_tabread_tilde *)pd_new (tabread_tilde_class);
    
    pthread_mutex_init (&x->x_mutex, NULL);
    
    x->x_name   = s;
    x->x_outlet = outlet_newSignal (cast_object (x));

    return x;
}

static void tabread_tilde_free (t_tabread_tilde *x)
{
    pthread_mutex_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void tabread_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabread__tilde__,
            (t_newmethod)tabread_tilde_new,
            (t_method)tabread_tilde_free,
            sizeof (t_tabread_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_NULL);
        
    class_addDSP (c, (t_method)tabread_tilde_dsp);
    
    class_addMethod (c, (t_method)tabread_tilde_set,     sym_set,       A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabread_tilde_restore, sym__restore,  A_SYMBOL, A_NULL);
    
    class_setDataFunction (c, tabread_tilde_functionData);
    
    tabread_tilde_class = c;
}

PD_LOCAL void tabread_tilde_destroy (void)
{
    class_free (tabread_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
