
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define LINE_DEFAULT_GRAIN          20.0

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *line_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _line {
    t_object    x_obj;              /* Must be the first. */
    t_systime   x_systimeTarget;
    t_systime   x_systimeStart;
    t_systime   x_systimeNext;
    double      x_timeRamp;
    t_float     x_valueTarget;
    t_float     x_valueStart;
    t_float     x_grain;
    int         x_hasRamp;
    t_outlet    *x_outlet;
    t_clock     *x_clock;
    } t_line;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error clock_reschedule (t_clock *x, double, double, t_systime);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void line_set (t_line *, t_float);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_float line_valueAtTime (t_line *x, t_systime t)
{
    double a = t - x->x_systimeStart;
    double b = x->x_systimeTarget - x->x_systimeStart;
    double y = x->x_valueTarget - x->x_valueStart;
        
    if (b == 0.0) { return x->x_valueTarget; }      /* Might occurs at load. */
    else {
        return (t_float)(x->x_valueStart + (a / b * y));
    }
}

static void line_task (t_line *x)
{
    double remains = - scheduler_getMillisecondsSince (x->x_systimeTarget);
    
    if (remains < PD_EPSILON) {
    
        outlet_float (x->x_outlet, x->x_valueTarget);
        
        x->x_systimeNext = 0;
        
    } else {

        outlet_float (x->x_outlet, line_valueAtTime (x, scheduler_getLogicalTime()));
    
        clock_delay (x->x_clock, PD_MIN (x->x_grain, remains));
    
        x->x_systimeNext = clock_getLogicalTime (x->x_clock);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void line_float (t_line *x, t_float f)
{
    t_systime now   = scheduler_getLogicalTime();
    t_systime after = scheduler_getLogicalTimeAfter (x->x_timeRamp);
    
    if (x->x_hasRamp && (after > now)) {
    
        x->x_hasRamp = 0;
        
        if (now > x->x_systimeTarget) { x->x_valueStart = x->x_valueTarget; }   /* Usual case. */
        else { 
            x->x_valueStart = line_valueAtTime (x, now);                        /* Retriggered case. */
        }
        
        x->x_systimeStart   = now;
        x->x_systimeTarget  = after;
        x->x_valueTarget    = f;
        
        line_task (x);
        
        clock_delay (x->x_clock, PD_MIN (x->x_grain, x->x_timeRamp));
        
        x->x_systimeNext = clock_getLogicalTime (x->x_clock);
        
    } else {
    
        line_set (x, f); outlet_float (x->x_outlet, f);
    }
}

static void line_floatRamp (t_line *x, t_float f)
{
    x->x_timeRamp = f; x->x_hasRamp = 1;
}

static void line_floatGrain (t_line *x, t_float f)
{
    x->x_grain = (f <= 0.0 ? (t_float)LINE_DEFAULT_GRAIN : f);
}

static void line_cancel (t_line *x)
{
    line_set (x, x->x_valueStart);
}

static void line_set (t_line *x, t_float f)
{
    clock_unset (x->x_clock); x->x_valueTarget = x->x_valueStart = f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *line_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_line *x   = (t_line *)z;
    t_buffer *b = buffer_new();

    if (x->x_hasRamp) {
        buffer_appendSymbol (b, sym__inlet2);
        buffer_appendFloat (b,  x->x_timeRamp);
        buffer_appendComma (b);
    }
    
    buffer_appendSymbol (b, sym__inlet3);
    buffer_appendFloat (b,  x->x_grain);
    buffer_appendComma (b);
    buffer_appendSymbol (b, sym__restore);
    
    return b;
    //
    }
    
    return NULL;
}

static void line_restore (t_line *x)
{
    t_line *old = (t_line *)instance_pendingFetch (cast_object (x));
    
    if (old) {
    //
    x->x_hasRamp        = old->x_hasRamp;
    x->x_timeRamp       = old->x_timeRamp;
    x->x_grain          = old->x_grain;
    x->x_systimeTarget  = old->x_systimeTarget;
    x->x_systimeStart   = old->x_systimeStart;
    x->x_systimeNext    = old->x_systimeNext;
    x->x_valueTarget    = old->x_valueTarget;
    x->x_valueStart     = old->x_valueStart;
    
    if (x->x_systimeNext) {
        if (clock_reschedule (x->x_clock, x->x_grain, PD_SECONDS_TO_MILLISECONDS (60), x->x_systimeNext)) {
            clock_delay (x->x_clock, 0.0); x->x_systimeNext = clock_getLogicalTime (x->x_clock);
        }
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *line_new (t_float f, t_float grain)
{
    t_line *x = (t_line *)pd_new (line_class);
    
    x->x_systimeTarget  = scheduler_getLogicalTime();
    x->x_systimeStart   = x->x_systimeTarget;
    x->x_valueTarget    = f;
    x->x_valueStart     = f;
    x->x_hasRamp        = 0;
    x->x_outlet         = outlet_newFloat (cast_object (x));
    x->x_clock          = clock_new ((void *)x, (t_method)line_task);
    
    line_floatGrain (x, grain);
    
    inlet_new2 (x, &s_float);
    inlet_new3 (x, &s_float);
    
    return x;
}

static void line_free (t_line *x)
{
    clock_free (x->x_clock);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void line_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_line,
            (t_newmethod)line_new,
            (t_method)line_free,
            sizeof (t_line),
            CLASS_DEFAULT,
            A_DEFFLOAT,
            A_DEFFLOAT,
            A_NULL);
        
    class_addFloat (c, (t_method)line_float);
        
    class_addMethod (c, (t_method)line_floatRamp,   sym__inlet2,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)line_floatGrain,  sym__inlet3,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)line_cancel,      sym_cancel,     A_NULL);
    class_addMethod (c, (t_method)line_set,         sym_set,        A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)line_restore,     sym__restore,   A_NULL);

    class_setDataFunction (c, line_functionData);
    class_requirePending (c);
    
    line_class = c;
}

void line_destroy (void)
{
    class_free (line_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
