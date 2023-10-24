
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *gatom_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct _gatom {
    t_gui       x_obj;          /* MUST be the first. */
    t_outlet    *x_outlet;
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_bang (t_gatom *x)
{
    outlet_float (x->x_outlet, gui_getValue (cast_gui (x)));
}

static void gatom_float (t_gatom *x, t_float f)
{
    gui_updateValue (cast_gui (x), f, 1); gatom_bang (x);
}

static void gatom_set (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    gui_updateValue (cast_gui (x), atom_getFloatAtIndex (0, argc, argv), 1);
}

static void gatom_range (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float low  = atom_getFloatAtIndex (0, argc, argv);
    t_float high = atom_getFloatAtIndex (1, argc, argv);
    
    gui_updateRange (cast_gui (x), low, high, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_gatom *x = (t_gatom *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_floatatom);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendFloat (b,  gui_getDigits (cast_gui (x)));
    buffer_appendFloat (b,  gui_getMinimum (cast_gui (x)));
    buffer_appendFloat (b,  gui_getMaximum (cast_gui (x)));
    buffer_appendFloat (b,  gui_getInterval (cast_gui (x)));
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, gui_getValue (cast_gui (x))); }
    buffer_appendSemicolon (b);
    
    object_serializeLabel (cast_object (x), b);
    
    object_saveIdentifiers (z, b, flags);
}

static t_buffer *gatom_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__restore);

    return b;
    //
    }
    
    return NULL;
}

/* Encapsulation. */

static void gatom_restore (t_gatom *x)
{
    t_gatom *old = (t_gatom *)instance_pendingFetch (cast_object (x));
    
    if (old) {      /* ??? */
    //
    t_float min = gui_getMinimum (cast_gui (old));
    t_float max = gui_getMaximum (cast_gui (old));
    
    gui_updateRange (cast_gui (x), min, max, 1);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )
    
static constexpr int gatom_flags()
{
    return GUI_NONE
            | GUI_VALUE
            | GUI_LOW
            | GUI_HIGH
            | GUI_INTERVAL
            | GUI_DIGITS;
}

static void gatom_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, gatom_flags());
}

static void gatom_functionSetParameters (t_object *o, const core::Group& group)
{
    if (gui_setParameters (o, group, gatom_flags())) { gatom_bang ((t_gatom *)o); }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void gatom_makeObjectProceed (t_glist *glist, t_gatom *x, int argc, t_atom *argv)
{
    int digits          = (int)atom_getFloatAtIndex (2, argc, argv);
    int low             = (int)atom_getFloatAtIndex (3, argc, argv);
    int high            = (int)atom_getFloatAtIndex (4, argc, argv);
    t_float interval    = (int)atom_getFloatAtIndex (5, argc, argv);
    t_float f           = atom_getFloatAtIndex (6, argc, argv);
    
    t_buffer *t = buffer_new();
    
    /* The buffer content is required only for the GUI inspector. */
    
    buffer_appendSymbol (t, sym_gatom); if (argc > 2) { buffer_append (t, argc - 2, argv + 2); }
    
    object_setBuffer (cast_object (x), t);
    object_setType (cast_object (x),   TYPE_ATOM);
    object_setX (cast_object (x),      atom_getFloatAtIndex (0, argc, argv));
    object_setY (cast_object (x),      atom_getFloatAtIndex (1, argc, argv));
    
    gui_updateDigits (cast_gui (x), digits, 0);
    gui_updateRange (cast_gui (x), low, high, 0);
    gui_updateInterval (cast_gui (x), interval, 0);
    gui_updateValue (cast_gui (x), f, 0);
    
    glist_objectAdd (glist, cast_object (x));
}

PD_LOCAL void gatom_makeObject (t_glist *glist, t_symbol *dummy, int argc, t_atom *argv)
{
    t_gatom *x  = (t_gatom *)pd_new (gatom_class);
    
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    gatom_makeObjectProceed (glist, x, argc, argv);     /* Must be called after outlet creation. */
    
    instance_setBoundA (cast_pd (x));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void gatom_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_gatom,
            NULL,
            NULL,
            sizeof (t_gatom),
            CLASS_DEFAULT,
            A_NULL);
            
    class_addBang (c, (t_method)gatom_bang);
    class_addFloat (c, (t_method)gatom_float);
        
    class_addMethod (c, (t_method)gatom_set,        sym_set,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)gatom_range,      sym_range,      A_GIMME, A_NULL);
    class_addMethod (c, (t_method)gatom_restore,    sym__restore,   A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, gatom_functionGetParameters, gatom_functionSetParameters);
    
    #endif
    
    class_setSaveFunction (c, gatom_functionSave);
    class_setDataFunction (c, gatom_functionData);
    class_requirePending (c);
    
    gatom_class = c;
}

PD_LOCAL void gatom_destroy (void)
{
    class_free (gatom_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
