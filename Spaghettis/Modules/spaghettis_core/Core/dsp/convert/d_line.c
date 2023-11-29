
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *line_tilde_class;       /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _line_tilde {
    t_object            x_obj;          /* Must be the first. */
    t_trylock           x_mutex;
    t_float             x_f;
    t_float             x_base;
    t_float             x_target;
    t_float             x_time;
    int                 x_stop;
    int                 x_rebase;
    int                 x_retarget;
    int                 x_dspCount;
    t_float             x_dspCurrent;
    t_float             x_dspStep;
    t_float             x_dspTarget;
    int                 x_tmpInitialize;
    int                 x_tmpCount;
    t_float             x_tmpCurrent;
    t_float             x_tmpStep;
    t_float             x_tmpTarget;
    t_outlet            *x_outlet;
    } t_line_tilde;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_float line_tilde_getValueOfTarget (t_line_tilde *x)
{
    t_float f = 0.0;

    trylock_lock (&x->x_mutex);

        f = x->x_target;

    trylock_unlock (&x->x_mutex);

    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void line_tilde_float (t_line_tilde *x, t_float f)
{
    trylock_lock (&x->x_mutex);

        x->x_target   = f;
        x->x_time     = PD_MAX (0.0, x->x_f);
        x->x_f        = 0.0;
        x->x_stop     = 0;
        x->x_retarget = 1;
    
        if (x->x_time == 0.0) { x->x_base = f; x->x_rebase = 1; }
    
    trylock_unlock (&x->x_mutex);
}

static void line_tilde_stop (t_line_tilde *x)
{
    trylock_lock (&x->x_mutex);
    
        x->x_stop     = 1;
        x->x_retarget = 1;
    
    trylock_unlock (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* No aliasing. */

static t_int *line_tilde_perform (t_int *w)
{
    t_line_tilde *x   = (t_line_tilde *)(w[1]);
    PD_RESTRICTED out = (t_sample *)(w[2]);
    t_space *t        = (t_space *)(w[3]);
    int n = (int)(w[4]);
    
    t_float millisecondsToSamples = t->s_float0;
    
    while (n--) {
    //
    if (trylock_trylock (&x->x_mutex)) {
    //
    if (x->x_retarget) {
        x->x_dspCurrent = x->x_rebase ? x->x_base : x->x_dspCurrent;
        x->x_target     = x->x_stop   ? x->x_dspCurrent : x->x_target;
        x->x_dspCount   = x->x_stop   ? 0 : (int)(x->x_time * millisecondsToSamples);
        x->x_dspStep    = x->x_dspCount ? ((x->x_target - x->x_dspCurrent) / x->x_dspCount) : 0.0;
        x->x_dspTarget  = x->x_target;
        x->x_stop       = 0;
        x->x_rebase     = 0;
        x->x_retarget   = 0;
    }
    
    trylock_unlock (&x->x_mutex);
    //
    }
    
    if (x->x_tmpInitialize) {
    //
    x->x_tmpInitialize  = 0;
    x->x_dspCount       = x->x_tmpCount;
    x->x_dspCurrent     = x->x_tmpCurrent;
    x->x_dspStep        = x->x_tmpStep;
    x->x_dspTarget      = x->x_tmpTarget;
    //
    }
    
    if (x->x_dspCount) {
        *out++ = (t_sample)x->x_dspCurrent; x->x_dspCurrent += x->x_dspStep; x->x_dspCount--;
    } else {
        *out++ = (t_sample)x->x_dspTarget; x->x_dspCurrent = x->x_dspTarget;
    }
    //
    }
    
    return (w + 5);
}

static void line_tilde_initialize (void *lhs, void *rhs)
{
    t_line_tilde *x     = (t_line_tilde *)lhs;
    t_line_tilde *old   = (t_line_tilde *)rhs;
    
    x->x_tmpCount       = old->x_dspCount;
    x->x_tmpCurrent     = old->x_dspCurrent;
    x->x_tmpStep        = old->x_dspStep;
    x->x_tmpTarget      = old->x_dspTarget;
    x->x_tmpInitialize  = 1;
}

static void line_tilde_dsp (t_line_tilde *x, t_signal **sp)
{
    t_space *t = space_new (cast_object (x));

    t->s_float0 = (t_float)(sp[0]->s_sampleRate / 1000.0);
    
    if (object_dspNeedInitializer (cast_object (x))) {
    //
    t_line_tilde *old = (t_line_tilde *)garbage_fetch (cast_object (x));
    
    if (old) { initializer_new (line_tilde_initialize, x, old); }
    //
    }
    
    dsp_add4 (line_tilde_perform, x, sp[0]->s_vector, t, sp[0]->s_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *line_tilde_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_line_tilde *x = (t_line_tilde *)z;
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, &s_float);
    buffer_appendFloat (b, line_tilde_getValueOfTarget (x));
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *line_tilde_new (void)
{
    t_line_tilde *x = (t_line_tilde *)pd_new (line_tilde_class);
    
    trylock_init (&x->x_mutex);
    
    x->x_outlet = outlet_newSignal (cast_object (x));
    
    inlet_newFloat (cast_object (x), &x->x_f);
    
    return x;
}

static void line_tilde_free (t_line_tilde *x)
{
    trylock_destroy (&x->x_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void line_tilde_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_line__tilde__,
            (t_newmethod)line_tilde_new,
            (t_method)line_tilde_free,
            sizeof (t_line_tilde),
            CLASS_DEFAULT,
            A_NULL);
        
    class_addDsp (c, (t_method)line_tilde_dsp);
    class_addFloat (c, (t_method)line_tilde_float);
    
    class_addMethod (c, (t_method)line_tilde_stop, sym_stop, A_NULL);
    
    class_setDataFunction (c, line_tilde_functionData);
    
    line_tilde_class = c;
}

void line_tilde_destroy (void)
{
    class_free (line_tilde_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
