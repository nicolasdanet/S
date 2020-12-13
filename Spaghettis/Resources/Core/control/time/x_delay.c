
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *delay_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _delay {
    t_object    x_obj;              /* Must be the first. */
    t_systime   x_cache;
    double      x_delay;
    t_float     x_unitValue;
    t_symbol    *x_unitName;
    t_outlet    *x_outlet;
    t_clock     *x_clock;
    } t_delay;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void clock_set (t_clock *, t_systime);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void delay_floatDelay    (t_delay *, t_float);
static void delay_dismiss       (t_delay *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void delay_task (t_delay *x)
{
    outlet_bang (x->x_outlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void delay_bang (t_delay *x)
{
    clock_delay (x->x_clock, x->x_delay);
}

static void delay_float (t_delay *x, t_float f)
{
    delay_floatDelay (x, f); delay_bang (x);
}

static void delay_floatDelay (t_delay *x, t_float f)
{
    if (f < 0.0) { error_invalid (sym_delay, sym_delay); } else { x->x_delay = f; }
}

static void delay_stop (t_delay *x)
{
    clock_unset (x->x_clock);
}

/* Note that float arguments are always passed at last. */

static void delay_unit (t_delay *x, t_symbol *unitName, t_float f)
{
    t_error err = clock_setUnitParsed (x->x_clock, f, unitName);
    
    if (err) { error_invalid (sym_delay, sym_unit); }
    else {
        x->x_unitValue = f;
        x->x_unitName  = unitName;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *delay_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_delay *x  = (t_delay *)z;
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

static void delay_functionDismiss (t_object *z)
{
    delay_dismiss ((t_delay *)z);
}

static void delay_restoreDelay (t_delay *x, t_systime f)
{
    if (f > 0.0 && f > scheduler_getLogicalTime()) { clock_set (x->x_clock, f); }
}

static void delay_restore (t_delay *x, t_float f)
{
    t_delay *old = (t_delay *)instance_pendingFetch (cast_object (x));

    if (old) {
    //
    delay_floatDelay (x, old->x_delay);
    
    if (old->x_unitName) { delay_unit (x, old->x_unitName, old->x_unitValue); }
    //
    }
    
    delay_restoreDelay (x, old ? old->x_cache : f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *delay_new (t_symbol *s, int argc, t_atom *argv)
{
    t_float f          = atom_getFloatAtIndex (0, argc, argv);
    t_float unit       = atom_getFloatAtIndex (1, argc, argv);
    t_symbol *unitName = atom_getSymbolAtIndex (2, argc, argv);
    
    t_delay *x = (t_delay *)pd_new (delay_class);
    
    x->x_outlet = outlet_newBang (cast_object (x));
    x->x_clock  = clock_new ((void *)x, (t_method)delay_task);
    
    inlet_new2 (x, &s_float);
    
    delay_floatDelay (x, f);
    
    if (unit != 0.0 && unitName != &s_) { delay_unit (x, unitName, unit); }
    
    if (argc > 3) { warning_unusedArguments (s, argc - 3, argv + 3); }
    
    return x;
}

static void delay_dismiss (t_delay *x)
{
    if (clock_isSet (x->x_clock)) {
    //
    x->x_cache = clock_getLogicalTime (x->x_clock); clock_unset (x->x_clock);
    //
    }
}

static void delay_free (t_delay *x)
{
    clock_free (x->x_clock);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void delay_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_delay,
            (t_newmethod)delay_new,
            (t_method)delay_free,
            sizeof (t_delay),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
            
    class_addBang (c, (t_method)delay_bang);
    class_addFloat (c, (t_method)delay_float);
        
    class_addMethod (c, (t_method)delay_floatDelay, sym__inlet2,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)delay_stop,       sym_stop,       A_NULL);
    class_addMethod (c, (t_method)delay_unit,       sym_unit,       A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)delay_unit,       sym_tempo,      A_FLOAT, A_SYMBOL, A_NULL);
    class_addMethod (c, (t_method)delay_restore,    sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, delay_functionData);
    class_setDismissFunction (c, delay_functionDismiss);
    class_requirePending (c);
    
    delay_class = c;
}

PD_LOCAL void delay_destroy (void)
{
    class_free (delay_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
