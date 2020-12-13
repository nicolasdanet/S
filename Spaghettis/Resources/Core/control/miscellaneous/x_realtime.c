
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *realtime_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _realtime {
    t_object    x_obj;                  /* Must be the first. */
    double      x_time;
    t_outlet    *x_outlet;
    } t_realtime;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void realtime_bang (t_realtime *x)
{
    x->x_time = clock_getRealTimeInSeconds();
}

static void realtime_elapsed (t_realtime *x)
{
    double elapsed = clock_getRealTimeInSeconds() - x->x_time;
    
    outlet_float (x->x_outlet, (t_float)(PD_SECONDS_TO_MILLISECONDS (elapsed)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_buffer *realtime_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_realtime *x = (t_realtime *)z;
    
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);
    buffer_appendFloat (b,  x->x_time);
    
    return b;
    //
    }
    
    return NULL;
}

static void realtime_restore (t_realtime *x, t_float f)
{
    t_realtime *old = (t_realtime *)instance_pendingFetch (cast_object (x));

    x->x_time = old ? old->x_time : f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *realtime_new (void)
{
    t_realtime *x = (t_realtime *)pd_new (realtime_class);
    
    x->x_outlet = outlet_newFloat (cast_object (x));

    inlet_new2 (x, &s_bang);

    realtime_bang (x);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void realtime_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_realtime,
            (t_newmethod)realtime_new,
            NULL,
            sizeof (t_realtime),
            CLASS_DEFAULT,
            A_NULL);
    
    class_addBang (c, (t_method)realtime_bang);
    
    class_addMethod (c, (t_method)realtime_elapsed, sym__inlet2,    A_NULL);
    class_addMethod (c, (t_method)realtime_restore, sym__restore,   A_FLOAT, A_NULL);

    class_setDataFunction (c, realtime_functionData);
    class_requirePending (c);

    realtime_class = c;
}

PD_LOCAL void realtime_destroy (void)
{
    class_free (realtime_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
