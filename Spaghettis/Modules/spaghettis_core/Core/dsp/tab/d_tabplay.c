
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

static t_class *tabplay_tilde_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabplay_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_trylock           x_mutex;
    int                 x_dismissed;
    int                 x_cachedPhase;
    int                 x_cachedEnd;
    int                 x_start;
    int                 x_end;
    int                 x_set;
    int                 x_size;
    t_word              *x_vector;
    t_symbol            *x_name;
    t_clock             *x_clock;
    t_outlet            *x_outletLeft;
    t_outlet            *x_outletRight;
    } t_tabplay_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabplay_tilde_task (t_tabplay_tilde *x)
{
    if (!x->x_dismissed) { outlet_bang (x->x_outletRight); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabplay_tilde_setProceed (t_tabplay_tilde *x, t_symbol *s, int verbose)
{
    trylock_lock (&x->x_mutex);
    
        t_error err = tab_fetchArray ((x->x_name = s), &x->x_size, &x->x_vector);
    
        x->x_start  = PD_INT_MAX;
        x->x_end    = 0;
        x->x_set    |= TAB_ARRAY;
    
    trylock_unlock (&x->x_mutex);
    
    if (verbose && err) { tab_error (cast_object (x), sym_tabplay__tilde__, s); }
}

static void tabplay_tilde_set (t_tabplay_tilde *x, t_symbol *s)
{
    tabplay_tilde_setProceed (x, s, 1);
}

static void tabplay_tilde_restore (t_tabplay_tilde *x, t_symbol *s)
{
    tabplay_tilde_setProceed (x, s, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabplay_tilde_list (t_tabplay_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    trylock_lock (&x->x_mutex);
    
        int start   = (int)atom_getFloatAtIndex (0, argc, argv);
        int length  = (int)atom_getFloatAtIndex (1, argc, argv);
    
        x->x_start  = PD_MAX (0, start);
        x->x_end    = (length > 0) ? x->x_start + length : PD_INT_MAX;
        x->x_set    |= TAB_PHASE;
    
    trylock_unlock (&x->x_mutex);
}

static void tabplay_tilde_stop (t_tabplay_tilde *x)
{
    trylock_lock (&x->x_mutex);
    
        x->x_start  = PD_INT_MAX;
        x->x_end    = 0;
        x->x_set    |= TAB_PHASE;
    
    trylock_unlock (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabplay_tilde_space (t_space *t, t_word *w, int size, int start, int end, int k)
{
    if (k) {
    //
    t->s_int0     = size;
    t->s_pointer0 = (void *)w;
    //
    }
    
    t->s_int1     = start;
    t->s_int2     = PD_MIN (t->s_int0, end);
    t->s_int3     = end;
}

/* No aliasing. */

static t_int *tabplay_tilde_perform (t_int *w)
{
    t_tabplay_tilde *x = (t_tabplay_tilde *)(w[1]);
    PD_RESTRICTED out  = (t_sample *)(w[2]);
    t_space *t         = (t_space *)(w[3]);
    int n = (int)(w[4]);
    
    if (trylock_trylock (&x->x_mutex) == 0) {
    //
    if (x->x_set) {
    
        tabplay_tilde_space (t, x->x_vector, x->x_size, x->x_start, x->x_end, (x->x_set & TAB_ARRAY));
        
        x->x_set = 0;
    }
    
    trylock_unlock (&x->x_mutex);
    //
    }
    
    /* Fetch old value of the phase at startup if the DSP chain is swapped. */
    
    if (!t->s_int4) {
    //
    if (x->x_cachedPhase >= 0) {
    
        t->s_int1 = x->x_cachedPhase;
        t->s_int2 = PD_MIN (t->s_int0, x->x_cachedEnd);
        t->s_int3 = x->x_cachedEnd;
    }
        
    t->s_int4 = 1;
    //
    }
    
    if (t->s_pointer0 && t->s_int1 < t->s_int2) {
    //
    t_word *data = (t_word *)t->s_pointer0 + t->s_int1;
    int size = PD_MIN (n, t->s_int2 - t->s_int1);
    int pad  = n - size;
    
    t->s_int1 += size;
    
    while (size--) { *out++ = (t_sample)w_getFloat (data); data++; }
    while (pad--)  { *out++ = 0.0; }
            
    if (t->s_int1 >= t->s_int2) { clock_delay (x->x_clock, 0.0); t->s_int1 = PD_INT_MAX; }
    //
    } else { while (n--) { *out++ = 0.0; } }
    
    x->x_cachedPhase = t->s_int1;
    x->x_cachedEnd   = t->s_int3;
    
    return (w + 5);
}

static void tabplay_tilde_initialize (void *lhs, void *rhs)
{
    t_tabplay_tilde *x   = (t_tabplay_tilde *)lhs;
    t_tabplay_tilde *old = (t_tabplay_tilde *)rhs;
    
    x->x_cachedPhase = old->x_cachedPhase;
    x->x_cachedEnd   = old->x_cachedEnd;
}

static void tabplay_tilde_dsp (t_tabplay_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_tabplay_tilde *old = (t_tabplay_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    initializer_new (tabplay_tilde_initialize, x, old);
    
    if (x->x_name != old->x_name) { tabplay_tilde_setProceed (x, old->x_name, 1); }
    //
    }
    //
    }
    
    {
    t_space *t  = space_new (cast_object (x));
    int size    = 0;
    t_word *w   = NULL;
    t_error err = tab_fetchArray (x->x_name, &size, &w);

    if (err) { tab_error (cast_object (x), sym_tabplay__tilde__, x->x_name); }
    else {
    //
    tabplay_tilde_space (t, w, size, PD_INT_MAX, 0, 1);
    //
    }

    dsp_add4 (tabplay_tilde_perform, x, sp[0]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabplay_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabplay_tilde *x = (t_tabplay_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, x->x_name);
    
    return b;
    //
    }
    
    return NULL;
}

static void tabplay_tilde_functionDismiss (t_object *z)
{
    t_tabplay_tilde *x = (t_tabplay_tilde *)z; x->x_dismissed = 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *tabplay_tilde_new (t_symbol *s)
{
    t_tabplay_tilde *x = (t_tabplay_tilde *)pd_new (tabplay_tilde_class);
    
    trylock_init (&x->x_mutex);
    
    x->x_cachedPhase = -1;
    x->x_cachedEnd   = 0;
    x->x_name        = s;
    x->x_clock       = clock_new ((void *)x, (t_method)tabplay_tilde_task);
    x->x_outletLeft  = outlet_newSignal (cast_object (x));
    x->x_outletRight = outlet_newBang (cast_object (x));
    
    return x;
}

static void tabplay_tilde_free (t_tabplay_tilde *x)
{
    clock_free (x->x_clock);
    
    trylock_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void tabplay_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabplay__tilde__,
            (t_newmethod)tabplay_tilde_new,
            (t_method)tabplay_tilde_free,
            sizeof (t_tabplay_tilde),
            CLASS_DEFAULT,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addDsp (c, (t_method)tabplay_tilde_dsp);
    
    class_addList (c, (t_method)tabplay_tilde_list);
        
    class_addMethod (c, (t_method)tabplay_tilde_set,        sym_set,        A_DEFSYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabplay_tilde_restore,    sym__restore,   A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabplay_tilde_stop,       sym_stop,       A_NULL);
    
    class_setDataFunction (c, tabplay_tilde_functionData);
    class_setDismissFunction (c, tabplay_tilde_functionDismiss);

    tabplay_tilde_class = c;
}

void tabplay_tilde_destroy (void)
{
    class_free (tabplay_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
