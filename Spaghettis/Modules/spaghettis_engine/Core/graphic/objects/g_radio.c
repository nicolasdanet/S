
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *radio_class;        /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _radio {
    t_gui       x_obj;              /* MUST be the first. */
    t_outlet    *x_outlet;
    } t_radio;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void radio_bang (t_radio *x)
{
    outlet_float (x->x_outlet, gui_getValue (cast_gui (x)));
}

static void radio_float (t_radio *x, t_float f)
{
    gui_updateValue (cast_gui (x), f, GUI_UPDATE_NOTIFY);
    
    radio_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_set (t_radio *x, t_float f)
{
    gui_updateValue (cast_gui (x), f, GUI_UPDATE_NOTIFY);
}

static void radio_mode (t_radio *x, t_symbol *s)
{
    gui_updateMultiple (cast_gui (x), ((s == sym_multiple) ? 1 : 0), GUI_UPDATE_NOTIFY);
}

static void radio_orientation (t_radio *x, t_symbol *s)
{
    gui_updateOrientation (cast_gui (x), ((s == sym_vertical) ? 1 : 0), GUI_UPDATE_NOTIFY);
}

static void radio_buttonsNumber (t_radio *x, t_float f)
{
    gui_updateButtons (cast_gui (x), (int)f, GUI_UPDATE_NOTIFY);
}

static void radio_size (t_radio *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        gui_updateWidth (cast_gui (x), (int)atom_getFloatAtIndex (0, argc, argv), GUI_UPDATE_NOTIFY);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int radio_flags()
{
    return GUI_NONE
            | GUI_VALUE
            | GUI_MULTIPLE
            | GUI_BUTTONS
            | GUI_WIDTH
            | GUI_ORIENTATION
            | GUI_INCLUDED;
}

#if defined ( PD_BUILDING_APPLICATION )
    
static void radio_functionGetParameters (t_object *o, data::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, radio_flags());
}

static void radio_functionSetParameters (t_object *o, const data::Group& group)
{
    gui_setParameters (o, group, radio_flags());
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void radio_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_radio *x = (t_radio *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (z));
    buffer_appendFloat (b,  object_getY (z));
    buffer_appendSymbol (b, gui_isVertical (cast_gui (x)) ? sym_vradio : sym_hradio);
    buffer_appendFloat (b,  gui_getWidth (cast_gui (x)));
    buffer_appendFloat (b,  gui_isMultiple (cast_gui (x)));
    buffer_appendFloat (b,  gui_getButtons (cast_gui (x)));
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, gui_getValue (cast_gui (x))); }
    buffer_appendSemicolon (b);
        
    object_saveIdentifiers (z, b, flags);
}

static void radio_restore (t_radio *x)
{
    gui_restore (cast_gui (x), radio_flags());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *radio_new (t_symbol *s, int argc, t_atom *argv)
{
    t_radio *x = (t_radio *)pd_new (radio_class);
    
    int isVertical  = (s == sym_vradio) ? 1 : 0;
    int width       = (argc > 2) ? atom_getFloat (argv + 0) : GUI_SIZE_DEFAULT;
    int isMultiple  = (argc > 2) ? atom_getFloat (argv + 1) : GUI_MULTIPLE_DEFAULT;
    int buttons     = (argc > 2) ? atom_getFloat (argv + 2) : GUI_BUTTONS_DEFAULT;
    t_float value   = (argc > 3) ? atom_getFloat (argv + 3) : 0.0;
    
    gui_updateValue (cast_gui (x), value, GUI_UPDATE_NONE);
    gui_updateMultiple (cast_gui (x), (isMultiple != 0), GUI_UPDATE_NONE);
    gui_updateButtons (cast_gui (x), buttons, GUI_UPDATE_NONE);
    gui_updateWidth (cast_gui (x), width, GUI_UPDATE_NONE);
    gui_updateOrientation (cast_gui (x), isVertical, GUI_UPDATE_NONE);
    
    x->x_outlet = outlet_newFloat (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void radio_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_hradio, 
            (t_newmethod)radio_new,
            NULL,
            sizeof (t_radio),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addCreator ((t_newmethod)radio_new, sym_vradio, A_GIMME, A_NULL);
        
    class_addBang (c, (t_method)radio_bang);
    class_addFloat (c, (t_method)radio_float);
    
    class_addMethod (c, (t_method)radio_set,            sym_set,            A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)radio_buttonsNumber,  sym_buttons,        A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)radio_mode,           sym_mode,           A_DEFSYMBOL, A_NULL);
    class_addMethod (c, (t_method)radio_orientation,    sym_orientation,    A_DEFSYMBOL, A_NULL);
    class_addMethod (c, (t_method)radio_size,           sym_size,           A_GIMME, A_NULL);
    class_addMethod (c, (t_method)radio_size,           sym__resize,        A_GIMME, A_NULL);
    class_addMethod (c, (t_method)radio_restore,        sym__restore,       A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, radio_functionGetParameters, radio_functionSetParameters);
    
    #endif
    
    class_setHelpName (c, sym_radio);
    class_setSaveFunction (c, radio_functionSave);
    class_setDataFunction (c, gui_functionData);
    class_requirePending (c);
    
    radio_class = c;
}

void radio_destroy (void)
{
    class_free (radio_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
