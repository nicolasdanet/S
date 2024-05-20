
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *print_tilde_class;              /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _print_tilde {
    t_object        x_obj;                      /* Must be the first. */
    t_int32Atomic   x_count;
    int             x_dismissed;
    t_fifo32        *x_fifo;
    t_symbol        *x_name;
    } t_print_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void print_tilde_dismiss (t_print_tilde *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void print_tilde_count (t_print_tilde *x, t_float f)
{
    int n = PD_MAX (1, (int)f); atomic_int32Write (&x->x_count, n);
}

static void print_tilde_bang (t_print_tilde *x)
{
    print_tilde_count (x, (t_float)1.0);
}

static void print_tilde_polling (t_print_tilde *x)
{
    if (!atomic_int32Read (&x->x_count)) {
    //
    int available = fifo32_getAvailableRead (x->x_fifo);
    
    while (available-- > 0) {
    //
    t_sample t = 0.0f;
    
    if (fifo32_read (x->x_fifo, &t, 1)) {
        post (cast_object (x), "%s: %g", x->x_name->s_name, t);
    }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_int *print_tilde_perform (t_int *w)
{
    t_print_tilde *x = (t_print_tilde *)(w[1]);
    PD_RESTRICTED in = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    
    if (atomic_int32Read (&x->x_count)) {
    //
    if (fifo32_getAvailableWrite (x->x_fifo) >= n) { fifo32_write (x->x_fifo, in, n); }
    
    atomic_int32Decrement (&x->x_count);
    //
    }
    
    return (w + 4);
}

static void print_tilde_dsp (t_print_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));

    dsp_add3 (print_tilde_perform, x, sp[0]->s_vector, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *print_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_print_tilde *x = (t_print_tilde *)z;
    t_buffer *b = buffer_new();
    
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void print_tilde_functionDismiss (t_object *z)
{
    print_tilde_dismiss ((t_print_tilde *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *print_tilde_new (t_symbol *s)
{
    t_print_tilde *x = (t_print_tilde *)pd_new (print_tilde_class);
    
    x->x_fifo = fifo32_new();
    x->x_name = (s != &s_ ? s : sym_print__tilde__);
    
    instance_pollingRegister (cast_pd (x));
    
    return x;
}

static void print_tilde_dismiss (t_print_tilde *x)
{
    if (!x->x_dismissed) {
    //
    x->x_dismissed = 1;
    
    instance_pollingUnregister (cast_pd (x));
    //
    }
}

static void print_tilde_free (t_print_tilde *x)
{
    print_tilde_dismiss (x);
    
    fifo32_free (x->x_fifo);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void print_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_print__tilde__,
            (t_newmethod)print_tilde_new,
            (t_method)print_tilde_free,
            sizeof (t_print_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addDsp (c, (t_method)print_tilde_dsp);
    class_addBang (c, (t_method)print_tilde_bang);
    class_addPolling (c, (t_method)print_tilde_polling);
    
    class_addMethod (c, (t_method)print_tilde_count, sym_count, A_FLOAT, A_NULL);
    
    class_setDataFunction (c, print_tilde_functionData);
    class_setDismissFunction (c, print_tilde_functionDismiss);

    print_tilde_class = c;
}

void print_tilde_destroy (void)
{
    class_free (print_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
