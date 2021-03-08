
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

static t_class *tabreceive_tilde_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _tabreceive_tilde {
    t_object            x_obj;                  /* Must be the first. */
    t_spin              x_mutex;
    int                 x_set;
    int                 x_size;
    t_word              *x_vector;
    t_symbol            *x_name;
    t_outlet            *x_outlet;
    } t_tabreceive_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void tabreceive_tilde_setProceed (t_tabreceive_tilde *x, t_symbol *s, int verbose)
{
    spin_lock (&x->x_mutex);

        t_error err = tab_fetchArray ((x->x_name = s), &x->x_size, &x->x_vector);
    
        x->x_set = 1;
    
    spin_unlock (&x->x_mutex);

    
    if (verbose && err) { tab_error (sym_tabreceive__tilde__, s); }
}

static void tabreceive_tilde_set (t_tabreceive_tilde *x, t_symbol *s)
{
    tabreceive_tilde_setProceed (x, s, 1);
}

static void tabreceive_tilde_restore (t_tabreceive_tilde *x, t_symbol *s)
{
    tabreceive_tilde_setProceed (x, s, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *tabreceive_tilde_perform (t_int *w)
{
    t_tabreceive_tilde *x = (t_tabreceive_tilde *)(w[1]);
    PD_RESTRICTED out     = (t_sample *)(w[2]);
    t_space *t            = (t_space *)(w[3]);
    int n = (int)w[4];
    
    if (spin_trylock (&x->x_mutex) == 0) {
    //
    if (x->x_set) {
        t->s_int0     = x->x_size;
        t->s_pointer0 = (void *)x->x_vector;
        x->x_set      = 0;
    }
    
    spin_unlock (&x->x_mutex);
    //
    }
    
    if (t->s_pointer0) {
    //
    t_word *data = (t_word *)t->s_pointer0;
    
    int size = PD_MIN (n, t->s_int0);
    int pad  = n - size;
    
    while (size--) { *out++ = (t_sample)w_getFloat (data); data++; }
    while (pad--)  { *out++ = 0.0; }
    //
    } else { while (n--) { *out++ = 0.0; } }
    
    return (w + 5);
}

static void tabreceive_tilde_dsp (t_tabreceive_tilde *x, t_signal **sp)
{
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_tabreceive_tilde *old = (t_tabreceive_tilde *)garbage_fetch (cast_object (x));
    
    if (old) {
    //
    if (x->x_name != old->x_name) { tabreceive_tilde_setProceed (x, old->x_name, 1); }
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

    if (err) { tab_error (sym_tabreceive__tilde__, x->x_name); }
    else {
        t->s_int0 = size; t->s_pointer0 = (void *)w;
    }
    
    dsp_add4 (tabreceive_tilde_perform, x, sp[0]->s_vector, t, sp[0]->s_vectorSize);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *tabreceive_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_tabreceive_tilde *x = (t_tabreceive_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendSymbol (b, x->x_name);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *tabreceive_tilde_new (t_symbol *s)
{
    t_tabreceive_tilde *x = (t_tabreceive_tilde *)pd_new (tabreceive_tilde_class);
    
    spin_init (&x->x_mutex);
    
    x->x_name   = s;
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    return x;
}

static void tabreceive_tilde_free (t_tabreceive_tilde *x)
{
    spin_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void tabreceive_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tabreceive__tilde__,
            (t_newmethod)tabreceive_tilde_new,
            (t_method)tabreceive_tilde_free,
            sizeof (t_tabreceive_tilde),
            CLASS_DEFAULT,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addDSP (c, (t_method)tabreceive_tilde_dsp);
    
    class_addMethod (c, (t_method)tabreceive_tilde_set,     sym_set,        A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)tabreceive_tilde_restore, sym__restore,   A_SYMBOL, A_NULL);
    
    class_setDataFunction (c, tabreceive_tilde_functionData);

    tabreceive_tilde_class = c;
}

PD_LOCAL void tabreceive_tilde_destroy (void)
{
    class_free (tabreceive_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
