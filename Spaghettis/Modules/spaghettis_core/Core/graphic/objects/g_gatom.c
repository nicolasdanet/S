
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

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
    gui_updateValue (cast_gui (x), f, GUI_UPDATE_NOTIFY);
    
    gatom_bang (x);
}

static void gatom_set (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    gui_updateValue (cast_gui (x), atom_getFloatAtIndex (0, argc, argv), GUI_UPDATE_NOTIFY);
}

static void gatom_range (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float low  = atom_getFloatAtIndex (0, argc, argv);
    t_float high = atom_getFloatAtIndex (1, argc, argv);
    
    gui_updateRange (cast_gui (x), low, high, GUI_UPDATE_NOTIFY);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int gatom_flags()
{
    return GUI_NONE
            | GUI_VALUE
            | GUI_RANGE
            | GUI_INTERVAL
            | GUI_DIGITS
            | GUI_INCLUDED;
}

#if defined ( PD_BUILDING_APPLICATION )
    
static void gatom_functionGetParameters (t_object *o, data::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, gatom_flags());
}

static void gatom_functionSetParameters (t_object *o, const data::Group& group)
{
    if (gui_setParameters (o, group, gatom_flags())) { gatom_bang ((t_gatom *)o); }
}

#endif

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
    buffer_appendFloat (b,  gui_getLow (cast_gui (x)));
    buffer_appendFloat (b,  gui_getHigh (cast_gui (x)));
    buffer_appendFloat (b,  gui_getInterval (cast_gui (x)));
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, gui_getValue (cast_gui (x))); }
    buffer_appendSemicolon (b);
        
    object_saveIdentifiers (z, b, flags);
}

static void gatom_include (t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    gui_include (cast_gui (x), argc, argv);
}

static void gatom_restore (t_gatom *x)
{
    gui_restore (cast_gui (x), gatom_flags());
}

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
    
    gui_updateDigits (cast_gui (x), digits, GUI_UPDATE_NONE);
    gui_updateRange (cast_gui (x), low, high, GUI_UPDATE_NONE);
    gui_updateInterval (cast_gui (x), interval, GUI_UPDATE_NONE);
    gui_updateValue (cast_gui (x), f, GUI_UPDATE_NONE);
    
    glist_objectAdd (glist, cast_object (x));
}

void gatom_makeObject (t_glist *glist, t_symbol *dummy, int argc, t_atom *argv)
{
    t_gatom *x  = (t_gatom *)pd_new (gatom_class);
    
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    gatom_makeObjectProceed (glist, x, argc, argv);     /* Must be called after outlet creation. */
    
    instance_setBoundA (cast_pd (x));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void gatom_setup (void)
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
    class_addMethod (c, (t_method)gatom_include,    sym__include,   A_GIMME, A_NULL);
    class_addMethod (c, (t_method)gatom_restore,    sym__restore,   A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, gatom_functionGetParameters, gatom_functionSetParameters);
    
    #endif
    
    class_setSaveFunction (c, gatom_functionSave);
    class_setDataFunction (c, gui_functionData);
    class_requirePending (c);
    
    gatom_class = c;
}

void gatom_destroy (void)
{
    class_free (gatom_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
