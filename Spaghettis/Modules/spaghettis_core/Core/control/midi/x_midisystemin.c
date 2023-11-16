
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *midisystemin_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _midisystemin {
    t_object    x_obj;                      /* Must be the first. */
    t_float     x_port;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_midisystemin;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midisystemin_float (t_midisystemin *x, t_float f)
{
    x->x_port = f;
}

static void midisystemin_list (t_midisystemin *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    outlet_float (x->x_outletRight, x->x_port);
    outlet_list (x->x_outletLeft, argc, argv);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *midisystemin_new (void)
{
    t_midisystemin *x = (t_midisystemin *)pd_new (midisystemin_class);
    
    x->x_outletLeft  = outlet_newList (cast_object (x));
    x->x_outletRight = outlet_newFloat (cast_object (x));
    
    pd_bind (cast_pd (x), sym__midisystemin);
    pd_bind (cast_pd (x), sym__midisystemportin);
    
    return x;
}

static void midisystemin_free (t_midisystemin *x)
{
    pd_unbind (cast_pd (x), sym__midisystemportin);
    pd_unbind (cast_pd (x), sym__midisystemin);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midisystemin_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_midisystemin,
            (t_newmethod)midisystemin_new,
            (t_method)midisystemin_free,
            sizeof (t_midisystemin),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_NULL);
    
    class_addFloat (c, (t_method)midisystemin_float);
    class_addList (c, (t_method)midisystemin_list);
    
    midisystemin_class = c;
}

void midisystemin_destroy (void)
{
    class_free (midisystemin_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
