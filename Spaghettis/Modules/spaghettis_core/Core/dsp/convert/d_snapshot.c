
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *snapshot_tilde_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _snapshot_tilde {
    t_object            x_obj;              /* Must be the first. */
    t_float64Atomic     x_value;
    int                 x_hasPolling;
    t_outlet            *x_outlet;
    } t_snapshot_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void snapshot_tilde_dismiss (t_snapshot_tilde *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void snapshot_tilde_bang (t_snapshot_tilde *x)
{
    outlet_float (x->x_outlet, PD_ATOMIC_FLOAT64_READ (&x->x_value));
}

static void snapshot_tilde_polling (t_snapshot_tilde *x)
{
    if (dsp_getState()) { snapshot_tilde_bang (x); }
}

static void snapshot_tilde_set (t_snapshot_tilde *x, t_float f)
{
    PD_ATOMIC_FLOAT64_WRITE (f, &x->x_value);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *snapshot_tilde_perform (t_int *w)
{
    PD_RESTRICTED in   = (t_sample *)(w[1]);
    t_float64Atomic *v = (t_float64Atomic *)(w[2]);
    t_float f          = *in;
    
    PD_ATOMIC_FLOAT64_WRITE (f, v);
    
    return (w + 3);
}

static void snapshot_tilde_dsp (t_snapshot_tilde *x, t_signal **sp)
{
    object_fetchAndCopySignalValuesIfRequired (cast_object (x));
    
    dsp_add2 (snapshot_tilde_perform, sp[0]->s_vector + (sp[0]->s_vectorSize - 1), &x->x_value);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *snapshot_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_snapshot_tilde *x = (t_snapshot_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym_set);
    buffer_appendFloat (b, PD_ATOMIC_FLOAT64_READ (&x->x_value));
    buffer_appendComma (b);
    object_getSignalValues (cast_object (x), b);
    
    return b;
    //
    }
    
    return NULL;
}

static void snapshot_tilde_functionDismiss (t_object *z)
{
    snapshot_tilde_dismiss ((t_snapshot_tilde *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *snapshot_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
    t_snapshot_tilde *x = (t_snapshot_tilde *)pd_new (snapshot_tilde_class);
    
    if (argc) {
        instance_pollingRegister (cast_pd (x));
        x->x_hasPolling = 1;
        argc--; argv++;
    }
    
    if (argc) { warning_unusedArguments (cast_object (x), s, argc, argv); }
    
    x->x_outlet = outlet_newFloat (cast_object (x));

    return x;
}

static void snapshot_tilde_dismiss (t_snapshot_tilde *x)
{
    if (x->x_hasPolling) { x->x_hasPolling = 0; instance_pollingUnregister (cast_pd (x)); }
}

static void snapshot_tilde_free (t_snapshot_tilde *x)
{
    snapshot_tilde_dismiss (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void snapshot_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_snapshot__tilde__,
            (t_newmethod)snapshot_tilde_new,
            (t_method)snapshot_tilde_free,
            sizeof (t_snapshot_tilde),
            CLASS_DEFAULT | CLASS_SIGNAL,
            A_GIMME,
            A_NULL);
    
    class_addDsp (c, (t_method)snapshot_tilde_dsp);
    class_addBang (c, (t_method)snapshot_tilde_bang);
    class_addPolling (c, (t_method)snapshot_tilde_polling);
    
    class_addMethod (c, (t_method)snapshot_tilde_set, sym_set, A_FLOAT, A_NULL);
    
    class_setDataFunction (c, snapshot_tilde_functionData);
    class_setDismissFunction (c, snapshot_tilde_functionDismiss);

    snapshot_tilde_class = c;
}

PD_LOCAL void snapshot_tilde_destroy (void)
{
    class_free (snapshot_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
