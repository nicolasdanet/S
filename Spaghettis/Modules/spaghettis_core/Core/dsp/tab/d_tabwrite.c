
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

static t_class *tabwrite_tilde_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabwrite_tilde {
    t_object            x_obj;                  /* Must be the first. */
    t_trylock           x_mutex;
    t_int32Atomic       x_redraw;
    int                 x_dismissed;
    int                 x_time;
    int                 x_set;
    int                 x_phase;
    int                 x_size;
    int                 x_cached;
    t_word              *x_vector;
    t_symbol            *x_name;
    t_clock             *x_clock;
    } t_tabwrite_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabwrite_tilde_dismiss (t_tabwrite_tilde *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error tab_fetchArray (t_symbol *s, int *size, t_word **data)
{
    t_garray *a = garray_fetch (s);
    
    (*size) = 0;
    (*data) = NULL;
    
    if (a) { garray_setAsUsedInDSP (a, 1); garray_getData (a, size, data); return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabwrite_tilde_polling (t_tabwrite_tilde *x)
{
    int n = PD_ATOMIC_INT32_READ (&x->x_redraw);
    
    if (n > 0) {
    //
    t_garray *a = garray_fetch (x->x_name);
    
    if (a) { garray_redraw (a); }

    while (n--) { PD_ATOMIC_INT32_DECREMENT (&x->x_redraw); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabwrite_tilde_setProceed (t_tabwrite_tilde *x, t_symbol *s, int verbose)
{
    tabwrite_tilde_polling (x);
    
    trylock_lock (&x->x_mutex);
    
        t_error err = tab_fetchArray ((x->x_name = s), &x->x_size, &x->x_vector);

        x->x_phase = PD_INT_MAX;
        x->x_set   |= TAB_ARRAY;
    
    trylock_unlock (&x->x_mutex);
    
    if (verbose && err) { tab_error (sym_tabwrite__tilde__, s); }
}

static void tabwrite_tilde_set (t_tabwrite_tilde *x, t_symbol *s)
{
    tabwrite_tilde_setProceed (x, s, 1);
}

static void tabwrite_tilde_restore (t_tabwrite_tilde *x, t_symbol *s)
{
    tabwrite_tilde_setProceed (x, s, 0);
}

static void tabwrite_tilde_bang (t_tabwrite_tilde *x)
{
    trylock_lock (&x->x_mutex);
    
        x->x_phase = 0;
        x->x_set   |= TAB_PHASE;
    
    trylock_unlock (&x->x_mutex);
    
    if (!x->x_dismissed && x->x_time > 0.0) { clock_delay (x->x_clock, x->x_time); }
}

static void tabwrite_tilde_start (t_tabwrite_tilde *x, t_float f)
{
    trylock_lock (&x->x_mutex);
    
        x->x_phase = (f > 0 ? f : 0);
        x->x_set   |= TAB_PHASE;
    
    trylock_unlock (&x->x_mutex);
}

static void tabwrite_tilde_stop (t_tabwrite_tilde *x)
{
    PD_ATOMIC_INT32_INCREMENT (&x->x_redraw);
    
    trylock_lock (&x->x_mutex);
    
        x->x_phase = PD_INT_MAX;
        x->x_set   |= TAB_PHASE;
    
    trylock_unlock (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabwrite_tilde_space (t_space *t, t_word *w, int size, int phase, int k)
{
    t->s_int2       = phase;
    t->s_int3       = 0;
    
    if (k) {
    //
    t->s_int0       = size;
    t->s_int1       = (size > 4096) ? (size >> 4) : 0;      /* Don't wait the end to draw large arrays. */
    t->s_pointer0   = (void *)w;
    //
    }
}

/* No aliasing. */

static t_int *tabwrite_tilde_perform (t_int *w)
{
    t_tabwrite_tilde *x = (t_tabwrite_tilde *)(w[1]);
    PD_RESTRICTED in    = (t_sample *)(w[2]);
    t_space *t          = (t_space *)(w[3]);
    int n = (int)(w[4]);
    
    if (trylock_trylock (&x->x_mutex) == 0) {
    //
    if (x->x_set) {
    
        tabwrite_tilde_space (t, x->x_vector, x->x_size, x->x_phase, (x->x_set & TAB_ARRAY));
        
        x->x_set = 0;
    }
    
    trylock_unlock (&x->x_mutex);
    //
    }
    
    /* Fetch old value of the phase at startup if the DSP chain is swapped. */
    
    if (!t->s_int4) { if (x->x_cached >= 0) { t->s_int2 = x->x_cached; } t->s_int4 = 1; }
    
    if (t->s_pointer0) {
    //
    if (t->s_int0 > t->s_int2) {
    //
    t_word *data = (t_word *)t->s_pointer0 + t->s_int2;
    int remains  = t->s_int0 - t->s_int2;
    int size     = PD_MIN (n, remains);
    
    t->s_int2 += size;
    t->s_int3 += size;
    
    if (t->s_int2 >= t->s_int0) {
        PD_ATOMIC_INT32_INCREMENT (&x->x_redraw);
        t->s_int2 = PD_INT_MAX;
        t->s_int3 = 0;
        
    } else if (t->s_int1 && t->s_int3 > t->s_int1) {
        PD_ATOMIC_INT32_INCREMENT (&x->x_redraw);
        t->s_int3 = 0;
    }
    
    while (size--) {

        t_sample f = *in++;
        if (denormal_isBigOrSmall (f)) { f = 0.0; }
        w_setFloat (data, (t_float)f);
        data++;
    }
    //
    } else { t->s_int2 = PD_INT_MAX; }
    //
    }
    
    x->x_cached = t->s_int2;
    
    return (w + 5);
}

static void tabwrite_tilde_initialize (void *lhs, void *rhs)
{
    t_tabwrite_tilde *x   = (t_tabwrite_tilde *)lhs;
    t_tabwrite_tilde *old = (t_tabwrite_tilde *)rhs;
    
    x->x_cached = old->x_cached;
}

static void tabwrite_tilde_dsp (t_tabwrite_tilde *x, t_signal **sp)
{
    if (!x->x_dismissed && x->x_time > 0.0) { clock_delay (x->x_clock, x->x_time); }
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_tabwrite_tilde *old = (t_tabwrite_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (tabwrite_tilde_initialize, x, old);
    
    if (x->x_name != old->x_name) { tabwrite_tilde_setProceed (x, old->x_name, 1); }
    
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

    if (err) { tab_error (sym_tabwrite__tilde__, x->x_name); }
    else {
        tabwrite_tilde_space (t, w, size, PD_INT_MAX, 1);
    }
    
    dsp_add4 (tabwrite_tilde_perform, x, sp[0]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabwrite_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabwrite_tilde *x = (t_tabwrite_tilde *)z;
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

static void tabwrite_tilde_functionDismiss (t_object *z)
{
    tabwrite_tilde_dismiss ((t_tabwrite_tilde *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *tabwrite_tilde_new (t_symbol *s, t_float f)
{
    t_tabwrite_tilde *x = (t_tabwrite_tilde *)pd_new (tabwrite_tilde_class);
    
    trylock_init (&x->x_mutex);
    
    x->x_time   = PD_MAX (0.0, f);
    x->x_cached = -1;
    x->x_name   = s;
    x->x_clock  = clock_new ((void *)x, (t_method)tabwrite_tilde_bang);
    
    instance_pollingRegister (cast_pd (x));
    
    return x;
}

static void tabwrite_tilde_dismiss (t_tabwrite_tilde *x)
{
    if (!x->x_dismissed) {
    //
    x->x_dismissed = 1;
    
    instance_pollingUnregister (cast_pd (x));
    //
    }
}

static void tabwrite_tilde_free (t_tabwrite_tilde *x)
{
    tabwrite_tilde_dismiss (x);
    
    clock_free (x->x_clock);
    
    trylock_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void tabwrite_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabwrite__tilde__,
            (t_newmethod)tabwrite_tilde_new,
            (t_method)tabwrite_tilde_free,
            sizeof (t_tabwrite_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_DEFFLOAT,
            A_NULL);
            
    class_addDsp (c, (t_method)tabwrite_tilde_dsp);
    class_addBang (c, (t_method)tabwrite_tilde_bang);
    class_addPolling (c, (t_method)tabwrite_tilde_polling);
        
    class_addMethod (c, (t_method)tabwrite_tilde_set,       sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabwrite_tilde_start,     sym_start,      A_DEFFLOAT, A_NULL);
    class_addMethod (c, (t_method)tabwrite_tilde_stop,      sym_stop,       A_NULL);
    class_addMethod (c, (t_method)tabwrite_tilde_restore,   sym__restore,   A_SYMBOL, A_NULL);

    class_setDataFunction (c, tabwrite_tilde_functionData);
    class_setDismissFunction (c, tabwrite_tilde_functionDismiss);

    tabwrite_tilde_class = c;
}

PD_LOCAL void tabwrite_tilde_destroy (void)
{
    class_free (tabwrite_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
