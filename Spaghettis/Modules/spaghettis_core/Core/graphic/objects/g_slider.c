
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *slider_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _slider {
    t_gui       x_obj;                  /* MUST be the first. */
    t_outlet    *x_outlet;
    } t_slider;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_bang (t_slider *x)
{
    outlet_float (x->x_outlet, gui_getValue (cast_gui (x)));
}

static void slider_float (t_slider *x, t_float f)
{
    gui_updateValue (cast_gui (x), f, 1); slider_bang (x);
}

static void slider_size (t_slider *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) {
    //
    gui_updateWidth (cast_gui (x),  (int)atom_getFloatAtIndex (0, argc, argv), 1);
    gui_updateHeight (cast_gui (x), (int)atom_getFloatAtIndex (1, argc, argv), 1);
    //
    }
}

static void slider_range (t_slider *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float minimum = atom_getFloatAtIndex (0, argc, argv);
    t_float maximum = atom_getFloatAtIndex (1, argc, argv);
    
    gui_updateRange (cast_gui (x), minimum, maximum, 1);
}

static void slider_set (t_slider *x, t_float f)
{
    gui_updateValue (cast_gui (x), f, 1);
}

static void slider_logarithmic (t_slider *x)
{
    gui_updateLogarithmic (cast_gui (x), 1, 1);
}

static void slider_linear (t_slider *x)
{
    gui_updateLogarithmic (cast_gui (x), 0, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_slider *x = (t_slider *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (z));
    buffer_appendFloat (b,  object_getY (z));
    buffer_appendSymbol (b, gui_isVertical (cast_gui (x)) ? sym_vslider : sym_hslider);
    buffer_appendFloat (b,  gui_getWidth (cast_gui (x)));
    buffer_appendFloat (b,  gui_getHeight (cast_gui (x)));
    buffer_appendFloat (b,  gui_getLow (cast_gui (x)));
    buffer_appendFloat (b,  gui_getHigh (cast_gui (x)));
    buffer_appendFloat (b,  gui_isLogarithmic (cast_gui (x)));
    buffer_appendFloat (b,  gui_getInterval (cast_gui (x)));
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, gui_getValue (cast_gui (x))); }
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void slider_restore (t_slider *x)
{
    t_slider *old = (t_slider *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    /* ??? */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static constexpr int slider_flags()
{
    return GUI_NONE
            | GUI_VALUE
            | GUI_LOW
            | GUI_HIGH
            | GUI_INTERVAL
            | GUI_LOGARITHMIC
            | GUI_WIDTH
            | GUI_HEIGHT
            | GUI_ORIENTATION
            | GUI_SWAP;
}

static void slider_functionGetParameters (t_object *o, data::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, slider_flags());
}

static void slider_functionSetParameters (t_object *o, const data::Group& group)
{
    if (gui_setParameters (o, group, slider_flags())) { slider_bang ((t_slider *)o); }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *slider_new (t_symbol *s, int argc, t_atom *argv)
{
    t_slider *x = (t_slider *)pd_new (slider_class);
    
    int isVertical      = (s == sym_vslider) ? 1 : 0;
    int widthDefault    = isVertical ? GUI_SIZE_DEFAULT : GUI_SIZE_DEFAULT * 8;
    int heightDefault   = isVertical ? GUI_SIZE_DEFAULT * 8 : GUI_SIZE_DEFAULT;
    int width           = (argc > 4) ? atom_getFloat (argv + 0) : widthDefault;
    int height          = (argc > 4) ? atom_getFloat (argv + 1) : heightDefault;
    t_float minimum     = (argc > 4) ? atom_getFloat (argv + 2) : GUI_MINIMUM_DEFAULT;
    t_float maximum     = (argc > 4) ? atom_getFloat (argv + 3) : GUI_MAXIMUM_DEFAULT;
    int isLogarithmic   = (argc > 4) ? atom_getFloat (argv + 4) : GUI_LOGARITHMIC_DEFAULT;
    t_float interval    = (argc > 5) ? atom_getFloat (argv + 5) : GUI_INTERVAL_DEFAULT;
    t_float value       = (argc > 6) ? atom_getFloat (argv + 6) : minimum;

    gui_updateValue (cast_gui (x), value, 0);
    gui_updateRange (cast_gui (x), minimum, maximum, 0);
    gui_updateInterval (cast_gui (x), interval, 0);
    gui_updateLogarithmic (cast_gui (x), (isLogarithmic != 0), 0);
    gui_updateWidth (cast_gui (x), width, 0);
    gui_updateHeight (cast_gui (x), height, 0);
    gui_updateOrientation (cast_gui (x), isVertical, 0);

    x->x_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void slider_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_hslider,
            (t_newmethod)slider_new,
            NULL,
            sizeof (t_slider),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);

    class_addCreator ((t_newmethod)slider_new, sym_vslider, A_GIMME, A_NULL);

    class_addBang (c, (t_method)slider_bang);
    class_addFloat (c, (t_method)slider_float);
    
    class_addMethod (c, (t_method)slider_size,          sym_size,           A_GIMME, A_NULL);
    class_addMethod (c, (t_method)slider_range,         sym_range,          A_GIMME, A_NULL);
    class_addMethod (c, (t_method)slider_set,           sym_set,            A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)slider_logarithmic,   sym_logarithmic,    A_NULL);
    class_addMethod (c, (t_method)slider_linear,        sym_linear,         A_NULL);
    class_addMethod (c, (t_method)slider_restore,       sym__restore,       A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, slider_functionGetParameters, slider_functionSetParameters);
    
    #endif
    
    class_setHelpName (c, sym_slider);
    class_setSaveFunction (c, slider_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);
    
    slider_class = c;
}

void slider_destroy (void)
{
    class_free (slider_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
