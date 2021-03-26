
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_class *vinlet_class;  /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_inlet    *glist_inletAdd     (t_glist *, t_pd *, int);

PD_LOCAL void       glist_inletRemove   (t_glist *, t_inlet *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void vinlet_dismiss (t_vinlet *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL int vinlet_isSignal (t_vinlet *x)
{
    return (x->vi_buffer != NULL);
}

PD_LOCAL int vinlet_getIndex (t_vinlet *x)
{
    return inlet_getIndex (vinlet_getInlet (cast_pd (x)));
}

PD_LOCAL t_inlet *vinlet_getInlet (t_pd *x)
{
    PD_ASSERT (pd_class (x) == vinlet_class); return (((t_vinlet *)x)->vi_inlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vinlet_bang (t_vinlet *x)
{
    outlet_bang (x->vi_outlet);
}

static void vinlet_float (t_vinlet *x, t_float f)
{
    outlet_float (x->vi_outlet, f);
}

static void vinlet_symbol (t_vinlet *x, t_symbol *s)
{
    outlet_symbol (x->vi_outlet, s);
}

static void vinlet_list (t_vinlet *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list (x->vi_outlet, argc, argv);
}

static void vinlet_anything (t_vinlet *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_anything (x->vi_outlet, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vinlet_functionDismiss (t_object *z)
{
    vinlet_dismiss ((t_vinlet *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *vinlet_newSignal (t_symbol *s)
{
    t_vinlet *x = (t_vinlet *)pd_new (vinlet_class);
    
    resample_init (&x->vi_resample, cast_object (x), s);

    x->vi_bufferSize   = 0;
    x->vi_buffer       = (t_sample *)PD_MEMORY_GET (0);
    x->vi_owner        = instance_contextGetCurrent();
    x->vi_outlet       = outlet_newSignal (cast_object (x));
    x->vi_inlet        = glist_inletAdd (x->vi_owner, cast_pd (x), 1);
    x->vi_directSignal = NULL;
    
    return x;
}

static void *vinlet_new (t_symbol *s)
{
    t_vinlet *x = (t_vinlet *)pd_new (vinlet_class);
    
    x->vi_owner  = instance_contextGetCurrent();
    x->vi_outlet = outlet_newMixed (cast_object (x));
    x->vi_inlet  = glist_inletAdd (x->vi_owner, cast_pd (x), 0);
    
    return x;
}

static void vinlet_dismiss (t_vinlet *x)
{
    if (!x->vi_dismissed) {
    //
    x->vi_dismissed = 1;
    
    glist_inletRemove (x->vi_owner, x->vi_inlet);
    
    x->vi_owner = NULL;
    //
    }
}

static void vinlet_free (t_vinlet *x)
{
    vinlet_dismiss (x);
    
    if (x->vi_buffer) { PD_MEMORY_FREE (x->vi_buffer); }
    
    resample_free (&x->vi_resample);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void vinlet_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_inlet,
            (t_newmethod)vinlet_new,
            (t_method)vinlet_free,
            sizeof (t_vinlet),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addCreator ((t_newmethod)vinlet_newSignal, sym_inlet__tilde__, A_DEFSYMBOL, A_NULL);
    
    class_addDsp (c, (t_method)vinlet_dsp);
    class_addBang (c, (t_method)vinlet_bang);
    class_addFloat (c, (t_method)vinlet_float);
    class_addSymbol (c, (t_method)vinlet_symbol);
    class_addList (c, (t_method)vinlet_list);
    class_addAnything (c, (t_method)vinlet_anything);
    
    class_setHelpName (c, sym_pd);
    class_setDismissFunction (c, vinlet_functionDismiss);
    
    vinlet_class = c;
}

PD_LOCAL void vinlet_destroy (void)
{
    class_free (vinlet_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
