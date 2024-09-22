
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *timer_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _timer {
    t_object    x_obj;              /* Must be the first. */
    t_systime   x_start;
    t_float     x_unit;
    int         x_isSamples;        /* Samples or milliseconds. */
    int         x_unitValue;
    t_symbol    *x_unitName;
    t_outlet    *x_outlet;
    } t_timer;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error clock_parseUnit (t_float, t_symbol *, t_float *, int *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void timer_bang (t_timer *x)
{
    x->x_start = scheduler_getLogicalTime();
}

static void timer_bangElapsed (t_timer *x)
{
    outlet_float (x->x_outlet, (t_float)scheduler_getUnitsSince (x->x_start, x->x_unit, x->x_isSamples));
}

/* Note that float arguments are always passed at last. */

static void timer_unit (t_timer *x, t_symbol *unitName, t_float f)
{
    t_error err = clock_parseUnit (f, unitName, &x->x_unit, &x->x_isSamples);
    
    if (err) { error_invalid (cast_object (x), sym_timer, sym_unit); }
    else {
        x->x_unitValue = f;
        x->x_unitName  = unitName;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *timer_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_timer *x = (t_timer *)z;
    
    t_buffer *b = buffer_new();
    
    if (x->x_unitName) {
    //
    buffer_appendSymbol (b, sym_unit);
    buffer_appendFloat (b,  x->x_unitValue);
    buffer_appendSymbol (b, x->x_unitName);
    buffer_appendComma (b);
    //
    }
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b,  x->x_start);
    
    return b;
    //
    }
    
    return NULL;
}

static void timer_restore (t_timer *x, t_float f)
{
    t_timer *old = (t_timer *)instance_pendingFetch (cast_object (x));

    x->x_start = old ? old->x_start : f;
    
    if (old && old->x_unitName) { timer_unit (x, old->x_unitName, old->x_unitValue); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that float arguments are always passed at last. */

static void *timer_new (t_symbol *unitName, t_float unit)
{
    t_timer *x = (t_timer *)pd_new (timer_class);
    
    x->x_unit      = 1;
    x->x_isSamples = 0;
    
    timer_bang (x);
        
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    inlet_new2 (x, &s_bang);
    
    if (unit != 0.0 && unitName != &s_) { timer_unit (x, unitName, unit); }
        
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void timer_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_timer,
            (t_newmethod)timer_new,
            NULL,
            sizeof (t_timer),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addBang (c, (t_method)timer_bang);
    
    class_addMethod (c, (t_method)timer_bangElapsed,    sym__inlet2,    A_NULL);
    class_addMethod (c, (t_method)timer_unit,           sym_unit,       A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)timer_unit,           sym_tempo,      A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)timer_restore,        sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, timer_functionData);
    class_requirePending (c);

    timer_class = c;
}

void timer_destroy (void)
{
    class_free (timer_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
