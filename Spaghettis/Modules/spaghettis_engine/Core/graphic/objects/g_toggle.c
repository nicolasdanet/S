
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *toggle_class;              /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _toggle {
    t_gui       x_obj;              /* MUST be the first. */
    t_outlet    *x_outlet;
    } t_toggle;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_output (t_toggle *x)
{
    outlet_float (x->x_outlet, gui_getState (cast_gui (x)) ? gui_getNonZero (cast_gui (x)) : 0.0);
}

static void toggle_bang (t_toggle *x)
{
    gui_updateState (cast_gui (x), (gui_getState (cast_gui (x)) == 0), GUI_UPDATE_NOTIFY);
    
    toggle_output (x);
}

static void toggle_float (t_toggle *x, t_float f)
{
    gui_updateState (cast_gui (x), (int)f, GUI_UPDATE_NOTIFY);
    
    toggle_output (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_set (t_toggle *x, t_float f)
{
    gui_updateState (cast_gui (x), (int)f, GUI_UPDATE_NOTIFY);
}

static void toggle_nonZero (t_toggle *x, t_float f)
{
    gui_updateNonZero (cast_gui (x), f, GUI_UPDATE_NOTIFY);
}

static void toggle_size (t_toggle *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        gui_updateWidth (cast_gui (x), (int)atom_getFloatAtIndex (0, argc, argv), GUI_UPDATE_NOTIFY);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int toggle_flags()
{
    return GUI_NONE
            | GUI_STATE
            | GUI_NONZERO
            | GUI_WIDTH;
}

#if defined ( PD_BUILDING_APPLICATION )

static void toggle_functionGetParameters (t_object *o, data::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, toggle_flags());
}

static void toggle_functionSetParameters (t_object *o, const data::Group& group)
{
    if (gui_setParameters (o, group, toggle_flags())) { toggle_output ((t_toggle *)o); }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void toggle_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_toggle *x = (t_toggle *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (z));
    buffer_appendFloat (b,  object_getY (z));
    buffer_appendSymbol (b, sym_tgl);
    buffer_appendFloat (b,  gui_getWidth (cast_gui (x)));
    buffer_appendFloat (b,  gui_getNonZero (cast_gui (x)));
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, gui_getState (cast_gui (x))); }
    buffer_appendSemicolon (b);
        
    object_saveIdentifiers (z, b, flags);
}

static void toggle_restore (t_toggle *x)
{
    gui_restore (cast_gui (x), toggle_flags());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *toggle_new (t_symbol *s, int argc, t_atom *argv)
{
    t_toggle *x = (t_toggle *)pd_new (toggle_class);
    
    int width       = (argc > 1) ? atom_getFloat (argv + 0) : GUI_SIZE_DEFAULT;
    t_float nonZero = (argc > 1) ? atom_getFloat (argv + 1) : GUI_NONZERO_DEFAULT;
    int state       = (argc > 2) ? atom_getFloat (argv + 2) : GUI_STATE_DEFAULT;

    gui_updateState (cast_gui (x), state, GUI_UPDATE_NONE);
    gui_updateNonZero (cast_gui (x), nonZero, GUI_UPDATE_NONE);
    gui_updateWidth (cast_gui (x), width, GUI_UPDATE_NONE);
    
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

static void toggle_free (t_toggle *x)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void toggle_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_tgl,
            (t_newmethod)toggle_new,
            (t_method)toggle_free,
            sizeof (t_toggle),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
    
    class_addBang (c, (t_method)toggle_bang);
    class_addFloat (c, (t_method)toggle_float);
    
    class_addMethod (c, (t_method)toggle_set,       sym_set,        A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)toggle_nonZero,   sym_nonzero,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)toggle_size,      sym_size,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)toggle_size,      sym__resize,    A_GIMME, A_NULL);
    class_addMethod (c, (t_method)toggle_restore,   sym__restore,   A_NULL);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, toggle_functionGetParameters, toggle_functionSetParameters);
    
    #endif
    
    class_setSaveFunction (c, toggle_functionSave);
    class_setDataFunction (c, gui_functionDataRestore);
    class_requirePending (c);
    
    toggle_class = c;
}

void toggle_destroy (void)
{
    class_free (toggle_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
