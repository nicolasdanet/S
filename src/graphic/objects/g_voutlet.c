
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *voutlet_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_outlet   *glist_outletAddSignal  (t_glist *);
PD_LOCAL t_outlet   *glist_outletAdd        (t_glist *);

PD_LOCAL void       glist_outletRemove      (t_glist *, t_outlet *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void voutlet_dismiss (t_voutlet *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int voutlet_isSignal (t_voutlet *x)
{
    return (x->vo_buffer != NULL);
}

PD_LOCAL int voutlet_getIndex (t_voutlet *x)
{
    return outlet_getIndex (voutlet_getOutlet (cast_pd (x)));
}

PD_LOCAL t_outlet *voutlet_getOutlet (t_pd *x)
{
    PD_ASSERT (pd_class (x) == voutlet_class); return (((t_voutlet *)x)->vo_outlet);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void voutlet_bang (t_voutlet *x)
{
    outlet_bang (x->vo_outlet);
}

static void voutlet_float (t_voutlet *x, t_float f)
{
    outlet_float (x->vo_outlet, f);
}

static void voutlet_symbol (t_voutlet *x, t_symbol *s)
{
    outlet_symbol (x->vo_outlet, s);
}

static void voutlet_list (t_voutlet *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list (x->vo_outlet, argc, argv);
}

static void voutlet_anything (t_voutlet *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_anything (x->vo_outlet, s, argc, argv);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void voutlet_functionDismiss (t_object *z)
{
    voutlet_dismiss ((t_voutlet *)z);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *voutlet_newSignal (t_symbol *s)
{
    t_voutlet *x = (t_voutlet *)pd_new (voutlet_class);
    
    resample_init (&x->vo_resample, cast_object (x), s);
    
    x->vo_bufferSize = 0;
    x->vo_buffer     = (t_sample *)PD_MEMORY_GET (0);
    x->vo_owner      = instance_contextGetCurrent();
    x->vo_outlet     = glist_outletAddSignal (x->vo_owner);
    
    inlet_newSignal (cast_object (x));

    return x;
}

static void *voutlet_new (t_symbol *s)
{
    t_voutlet *x = (t_voutlet *)pd_new (voutlet_class);
    
    x->vo_owner  = instance_contextGetCurrent();
    x->vo_outlet = glist_outletAdd (x->vo_owner);
    
    inlet_new (cast_object (x), cast_pd (x), NULL, NULL);

    return x;
}

static void voutlet_dismiss (t_voutlet *x)
{
    if (!x->vo_dismissed) {
    //
    x->vo_dismissed = 1;
    
    glist_outletRemove (x->vo_owner, x->vo_outlet);
    
    x->vo_owner = NULL;
    //
    }
}

static void voutlet_free (t_voutlet *x)
{
    voutlet_dismiss (x);
    
    if (x->vo_buffer) { PD_MEMORY_FREE (x->vo_buffer); }
    
    resample_free (&x->vo_resample);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void voutlet_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_outlet,
            (t_newmethod)voutlet_new,
            (t_method)voutlet_free,
            sizeof (t_voutlet),
            CLASS_DEFAULT | CLASS_NOINLET,
            A_DEFSYMBOL,
            A_NULL);
            
    class_addCreator ((t_newmethod)voutlet_newSignal, sym_outlet__tilde__, A_DEFSYMBOL, A_NULL);
    
    class_addDSP (c, (t_method)voutlet_dsp);
    class_addBang (c, (t_method)voutlet_bang);
    class_addFloat (c, (t_method)voutlet_float);
    class_addSymbol (c, (t_method)voutlet_symbol);
    class_addList (c, (t_method)voutlet_list);
    class_addAnything (c, (t_method)voutlet_anything);
    
    class_setHelpName (c, sym_pd);
    class_setDismissFunction (c, voutlet_functionDismiss);
    
    voutlet_class = c;
}

PD_LOCAL void voutlet_destroy (void)
{
    class_free (voutlet_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
