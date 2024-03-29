
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *dial_class;         /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _dial {
    t_gui       x_obj;              /* MUST be the first. */
    t_outlet    *x_outlet;
    } t_dial;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_bang (t_dial *x)
{
    outlet_float (x->x_outlet, gui_getValue (cast_gui (x)));
}

static void dial_float (t_dial *x, t_float f)
{
    gui_updateValue (cast_gui (x), f, 1); dial_bang (x);
}

static void dial_list (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc && IS_FLOAT (argv)) { dial_float (x, atom_getFloat (argv)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Kept compatible with vanilla nbx object. */

static void dial_size (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    int digits = (int)atom_getFloatAtIndex (0, argc, argv);
    int width  = (int)atom_getFloatAtIndex (1, argc, argv);

    if (argc > 0) { gui_updateDigits (cast_gui (x), digits, 1); }
    if (argc > 1) { gui_updateWidth (cast_gui (x), width, 1);   }
}

static void dial_range (t_dial *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float minimum = atom_getFloatAtIndex (0, argc, argv);
    t_float maximum = atom_getFloatAtIndex (1, argc, argv);
    
    gui_updateRange (cast_gui (x), minimum, maximum, 1);
}

static void dial_set (t_dial *x, t_float f)
{
    gui_updateValue (cast_gui (x), f, 1);
}

static void dial_logarithmic (t_dial *x)
{
    gui_updateLogarithmic (cast_gui (x), 1, 1);
}

static void dial_linear (t_dial *x)
{
    gui_updateLogarithmic (cast_gui (x), 0, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void dial_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_gui *x = cast_gui (z);
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (z));
    buffer_appendFloat (b,  object_getY (z));
    buffer_appendSymbol (b, sym_dial);
    buffer_appendFloat (b,  gui_getWidth (x));
    buffer_appendFloat (b,  gui_getDigits (x));
    buffer_appendFloat (b,  gui_getLow (x));
    buffer_appendFloat (b,  gui_getHigh (x));
    buffer_appendFloat (b,  gui_isLogarithmic (x));
    buffer_appendFloat (b,  gui_getInterval (x));
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, gui_getValue (x)); }
    buffer_appendSemicolon (b);
    
    object_serializeInclusion (z, b);
    
    object_saveIdentifiers (z, b, flags);
}

static void dial_restore (t_dial *x)
{
    t_dial *old = (t_dial *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    /* ??? */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static constexpr int dial_flags()
{
    return GUI_NONE
            | GUI_VALUE
            | GUI_LOW
            | GUI_HIGH
            | GUI_INTERVAL
            | GUI_LOGARITHMIC
            | GUI_DIGITS
            | GUI_WIDTH;
}

static void dial_functionGetParameters (t_object *o, data::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, dial_flags());
}

static void dial_functionSetParameters (t_object *o, const data::Group& group)
{
    if (gui_setParameters (o, group, dial_flags())) { dial_bang ((t_dial *)o); }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *dial_new (t_symbol *s, int argc, t_atom *argv)
{
    t_dial *x = (t_dial *)pd_new (dial_class);
    
    int defaultWidth    = 3 * GUI_SIZE_DEFAULT;
    int width           = (argc > 4) ? atom_getFloat (argv + 0) : defaultWidth;
    int digits          = (argc > 4) ? atom_getFloat (argv + 1) : GUI_DIGITS_DEFAULT;
    t_float minimum     = (argc > 4) ? atom_getFloat (argv + 2) : GUI_MINIMUM_DEFAULT;
    t_float maximum     = (argc > 4) ? atom_getFloat (argv + 3) : GUI_MAXIMUM_DEFAULT;
    int isLogarithmic   = (argc > 4) ? atom_getFloat (argv + 4) : GUI_LOGARITHMIC_DEFAULT;
    int interval        = (argc > 5) ? atom_getFloat (argv + 5) : GUI_INTERVAL_DEFAULT;
    t_float value       = (argc > 6) ? atom_getFloat (argv + 6) : minimum;
    
    gui_updateValue (cast_gui (x), value, 0);
    gui_updateRange (cast_gui (x), minimum, maximum, 0);
    gui_updateInterval (cast_gui (x), interval, 0);
    gui_updateLogarithmic (cast_gui (x), (isLogarithmic != 0), 0);
    gui_updateDigits (cast_gui (x), digits, 0);
    gui_updateWidth (cast_gui (x), width ? width : defaultWidth, 0);
    
    x->x_outlet = outlet_newFloat (cast_object (x));

    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void dial_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_dial,
            (t_newmethod)dial_new,
            NULL,
            sizeof (t_dial),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)dial_bang);
    class_addFloat (c, (t_method)dial_float);
    class_addList (c, (t_method)dial_list);
    
    class_addMethod (c, (t_method)dial_size,        sym_size,           A_GIMME, A_NULL);
    class_addMethod (c, (t_method)dial_range,       sym_range,          A_GIMME, A_NULL);
    class_addMethod (c, (t_method)dial_set,         sym_set,            A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)dial_logarithmic, sym_logarithmic,    A_NULL);
    class_addMethod (c, (t_method)dial_linear,      sym_linear,         A_NULL);
    class_addMethod (c, (t_method)dial_restore,     sym__restore,       A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, dial_functionGetParameters, dial_functionSetParameters);
    
    #endif
    
    class_setSaveFunction (c, dial_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);
    
    dial_class = c;
}

void dial_destroy (void)
{
    class_free (dial_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
