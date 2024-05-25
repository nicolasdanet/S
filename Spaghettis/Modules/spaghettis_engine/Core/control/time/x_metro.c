
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define METRO_DEFAULT_DELAY             1000.0

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *metro_class;            /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _metro {
    t_object    x_obj;                  /* Must be the first. */
    t_systime   x_cache;
    double      x_delay;
    int         x_reentrantStart;
    int         x_reentrantStop;
    t_float     x_unitValue;
    t_symbol    *x_unitName;
    t_outlet    *x_outlet;
    t_clock     *x_clock;
    } t_metro;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error clock_reschedule (t_clock *x, double, double, t_systime);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void metro_float     (t_metro *, t_float);
static void metro_dismiss   (t_metro *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void metro_task (t_metro *x)
{
    x->x_reentrantStop  = 0;
    
    if (!x->x_reentrantStart) {
    //
    x->x_reentrantStart = 1;
    outlet_bang (x->x_outlet);
    //
    }
    
    if (!x->x_reentrantStop) { 
    //
    clock_delay (x->x_clock, x->x_delay);
    //
    }
    
    x->x_reentrantStart = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void metro_bang (t_metro *x)
{
    metro_float (x, (t_float)1.0);
}

static void metro_float (t_metro *x, t_float f)
{
    if (f != 0.0) { metro_task (x); }
    else {
        clock_unset (x->x_clock);
    }
    
    x->x_reentrantStop = 1;
}

static void metro_floatDelay (t_metro *x, t_float f)
{
    if (f < 0.0) { error_invalid (cast_object (x), sym_metro, sym_delay); }
    else {
        x->x_delay = (double)((f == 0.0) ? METRO_DEFAULT_DELAY : f);
    }
}

static void metro_stop (t_metro *x)
{
    metro_float (x, 0.0);
}

/* Note that float arguments are always passed at last. */

static void metro_unit (t_metro *x, t_symbol *unitName, t_float f)
{
    t_error err = clock_setUnitParsed (x->x_clock, f, unitName);
    
    if (err) { error_invalid (cast_object (x), sym_metro, sym_unit); }
    else {
        x->x_unitValue = f;
        x->x_unitName  = unitName;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *metro_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_metro *x  = (t_metro *)z;
    t_buffer *b = buffer_new();
    
    if (x->x_unitName) {
        buffer_appendSymbol (b, sym_unit);
        buffer_appendFloat (b,  x->x_unitValue);
        buffer_appendSymbol (b, x->x_unitName);
        buffer_appendComma (b);
    }
    
    buffer_appendSymbol (b, sym__inlet2);
    buffer_appendFloat (b,  x->x_delay);
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b,  clock_isSet (x->x_clock) ? clock_getLogicalTime (x->x_clock) : 0.0);
    
    return b;
    //
    }
    
    return NULL;
}

static void metro_functionDismiss (t_object *z)
{
    metro_dismiss ((t_metro *)z);
}

static void metro_restoreReschedule (t_metro *x, t_systime f)
{
    if (f > 0.0) {
    //
    t_error err = clock_reschedule (x->x_clock, x->x_delay, PD_SECONDS_TO_MILLISECONDS (60), f);
    
    if (err) { metro_bang (x); }
    //
    }
}

static void metro_restore (t_metro *x, t_float f)
{
    t_metro *old = (t_metro *)instance_pendingFetch (cast_object (x));

    if (old) {
    //
    metro_floatDelay (x, old->x_delay);
    
    if (old->x_unitName) { metro_unit (x, old->x_unitName, old->x_unitValue); }
    //
    }
    
    metro_restoreReschedule (x, old ? old->x_cache : f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *metro_new (t_symbol *s, int argc, t_atom *argv)
{
    t_float f          = atom_getFloatAtIndex (0, argc, argv);
    t_float unit       = atom_getFloatAtIndex (1, argc, argv);
    t_symbol *unitName = atom_getSymbolAtIndex (2, argc, argv);
        
    t_metro *x = (t_metro *)pd_new (metro_class);
    
    x->x_reentrantStart = 0;
    x->x_reentrantStop  = 0;
    
    x->x_clock  = clock_newSingle ((void *)x, (t_method)metro_task);
    x->x_outlet = outlet_newBang (cast_object (x));
    
    inlet_new2 (x, &s_float);
    
    metro_floatDelay (x, f);
    
    if (unit != 0.0 && unitName != &s_) { metro_unit (x, unitName, unit); }
    
    if (argc > 3) { warning_unusedArguments (cast_object (x), s, argc - 3, argv + 3); }
    
    return x;
}

static void metro_dismiss (t_metro *x)
{
    if (clock_isSet (x->x_clock)) {
    //
    x->x_cache = clock_getLogicalTime (x->x_clock); clock_unset (x->x_clock);
    //
    }
}

static void metro_free (t_metro *x)
{
    clock_free (x->x_clock);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void metro_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_metro,
            (t_newmethod)metro_new,
            (t_method)metro_free,
            sizeof (t_metro),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addBang (c, (t_method)metro_bang);
    class_addFloat (c, (t_method)metro_float);
    
    class_addMethod (c, (t_method)metro_floatDelay, sym__inlet2,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)metro_stop,       sym_stop,       A_NULL);
    class_addMethod (c, (t_method)metro_unit,       sym_unit,       A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)metro_unit,       sym_tempo,      A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)metro_restore,    sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, metro_functionData);
    class_setDismissFunction (c, metro_functionDismiss);
    class_requirePending (c);
    
    metro_class = c;
}

void metro_destroy (void)
{
    class_free (metro_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
