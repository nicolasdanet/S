
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

static t_class *tabsend_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabsend_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_trylock           x_mutex;
    int                 x_set;
    int                 x_size;
    t_word              *x_vector;
    t_symbol            *x_name;
    } t_tabsend_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabsend_tilde_setProceed (t_tabsend_tilde *x, t_symbol *s, int verbose)
{
    trylock_lock (&x->x_mutex);
    
        t_error err = tab_fetchArray ((x->x_name = s), &x->x_size, &x->x_vector);
    
        x->x_set = 1;
    
    trylock_unlock (&x->x_mutex);
    
    if (verbose && err) { tab_error (cast_object (x), sym_tabsend__tilde__, s); }
}

static void tabsend_tilde_set (t_tabsend_tilde *x, t_symbol *s)
{
    tabsend_tilde_setProceed (x, s, 1);
}

static void tabsend_tilde_restore (t_tabsend_tilde *x, t_symbol *s)
{
    tabsend_tilde_setProceed (x, s, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *tabsend_tilde_perform (t_int *w)
{
    t_tabsend_tilde *x = (t_tabsend_tilde *)(w[1]);
    PD_RESTRICTED in   = (t_sample *)(w[2]);
    t_space *t         = (t_space *)(w[3]);
    int n = (int)w[4];
    
    if (trylock_trylock (&x->x_mutex) == 0) {
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
    t_word *data = (t_word *)t->s_pointer0;
    
    n = PD_MIN (t->s_int0, n);

    while (n--) {
    //  
    t_sample f = *in++;
    if (denormal_isBigOrSmall (f)) { f = 0.0; }
    w_setFloat (data, (t_float)f);
    data++;
    //
    }
    //
    }

    return (w + 5);
}

static void tabsend_tilde_dsp (t_tabsend_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_tabsend_tilde *old = (t_tabsend_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    if (x->x_name != old->x_name) { tabsend_tilde_setProceed (x, old->x_name, 1); }
    
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

    if (err) { tab_error (cast_object (x), sym_tabsend__tilde__, x->x_name); }
    else {
        t->s_int0 = size; t->s_pointer0 = (void *)w;
    }
    
    dsp_add4 (tabsend_tilde_perform, x, sp[0]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabsend_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabsend_tilde *x = (t_tabsend_tilde *)z;
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

static void *tabsend_tilde_new (t_symbol *s)
{
    t_tabsend_tilde *x = (t_tabsend_tilde *)pd_new (tabsend_tilde_class);
    
    trylock_init (&x->x_mutex);
    
    x->x_name = s;
    
    return x;
}

static void tabsend_tilde_free (t_tabsend_tilde *x)
{
    trylock_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void tabsend_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabsend__tilde__,
            (t_newmethod)tabsend_tilde_new,
            (t_method)tabsend_tilde_free,
            sizeof (t_tabsend_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addDsp (c, (t_method)tabsend_tilde_dsp);
    
    class_addMethod (c, (t_method)tabsend_tilde_set,        sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabsend_tilde_restore,    sym__restore,   A_SYMBOL, A_NULL);

    class_setDataFunction (c, tabsend_tilde_functionData);

    tabsend_tilde_class = c;
}

void tabsend_tilde_destroy (void)
{
    class_free (tabsend_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
