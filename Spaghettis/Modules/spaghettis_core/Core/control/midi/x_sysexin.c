
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *sysexin_class;          /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _sysexin {
    t_object    x_obj;                  /* Must be the first. */
    t_float     x_port;
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletMiddle;
    t_outlet    *x_outletRight;
    } t_sysexin;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void sysexin_float (t_sysexin *x, t_float f)
{
    x->x_port = f;
}

static void sysexin_list (t_sysexin *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    int i = 0;
    outlet_float (x->x_outletMiddle, x->x_port);
    for (i = 0; i < argc; i++) { outlet_float (x->x_outletLeft, atom_getFloat (argv + i)); }
    outlet_bang (x->x_outletRight);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *sysexin_new (void)
{
    t_sysexin *x = (t_sysexin *)pd_new (sysexin_class);
    
    x->x_outletLeft   = outlet_newFloat (cast_object (x));
    x->x_outletMiddle = outlet_newFloat (cast_object (x));
    x->x_outletRight  = outlet_newBang (cast_object (x));
    
    pd_bind (cast_pd (x), sym__sysexportin);
    pd_bind (cast_pd (x), sym__sysexin);
    
    return x;
}

static void sysexin_free (t_sysexin *x)
{
    pd_unbind (cast_pd (x), sym__sysexin);
    pd_unbind (cast_pd (x), sym__sysexportin);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void sysexin_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_sysexin,
            (t_newmethod)sysexin_new,
            (t_method)sysexin_free,
            sizeof (t_sysexin),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_NULL);
    
    class_addFloat (c, (t_method)sysexin_float);
    class_addList (c, (t_method)sysexin_list);
    
    sysexin_class = c;
}

void sysexin_destroy (void)
{
    class_free (sysexin_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
