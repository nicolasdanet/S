
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *until_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _until {
    t_object    x_obj;              /* Must be the first. */
    int         x_run;
    int         x_count;
    t_outlet    *x_outlet;
    } t_until;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void until_proceed (t_until *x)
{
    while (x->x_run && x->x_count) { x->x_count--; outlet_bang (x->x_outlet); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void until_bang (t_until *x)
{
    x->x_run = 1; x->x_count = -1; until_proceed (x);
}

static void until_float (t_until *x, t_float f)
{
    x->x_run = 1; x->x_count = (int)PD_MAX (0.0, f); until_proceed (x);
}

static void until_stop (t_until *x)
{
    x->x_run = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *until_new (void)
{
    t_until *x = (t_until *)pd_new (until_class);
    
    x->x_run    = 0;
    x->x_outlet = outlet_newBang (cast_object (x));
    
    inlet_new2 (x, &s_bang);

    warning_deprecatedObject (cast_object (x), sym_until);
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void until_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_until,
            (t_newmethod)until_new,
            NULL,
            sizeof (t_until),
            CLASS_DEFAULT,
            A_NULL);
            
    class_addBang (c, (t_method)until_bang);
    class_addFloat (c, (t_method)until_float);
    
    class_addMethod (c, (t_method)until_stop, sym__inlet2, A_NULL);
    
    until_class = c;
}

void until_destroy (void)
{
    class_free (until_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------