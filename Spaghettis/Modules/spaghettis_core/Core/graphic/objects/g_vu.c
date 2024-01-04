
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_class *vu_class;               /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _vu {
    t_gui       x_obj;                  /* MUST be the first. */
    t_outlet    *x_outletLeft;
    t_outlet    *x_outletRight;
    } t_vu;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vu_outPeak (t_vu *x)
{
    outlet_float (x->x_outletRight, gui_getPeak (cast_gui (x)));
}

static void vu_outValue (t_vu *x)
{
    outlet_float (x->x_outletLeft,  gui_getValue (cast_gui (x)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vu_bang (t_vu *x)
{
    vu_outPeak (x);
    vu_outValue (x);
}

static void vu_peak (t_vu *x, t_float f)
{
    if (gui_updatePeak (cast_gui (x), f, 1))  { vu_outPeak (x); }
}

static void vu_float (t_vu *x, t_float f)
{
    if (gui_updateValue (cast_gui (x), f, 1)) { vu_outValue (x); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vu_size (t_vu *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) {
    //
    gui_updateWidth (cast_gui (x),  (int)atom_getFloatAtIndex (0, argc, argv), 1);
    gui_updateHeight (cast_gui (x), (int)atom_getFloatAtIndex (1, argc, argv), 1);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void vu_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_vu *x = (t_vu *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendSymbol (b, sym_vu);
    buffer_appendFloat (b,  gui_getWidth (cast_gui (x)));
    buffer_appendFloat (b,  gui_getHeight (cast_gui (x)));
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void vu_restore (t_vu *x)
{
    t_vu *old = (t_vu *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    /* ??? */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

static constexpr int vu_flags()
{
    return GUI_NONE
            | GUI_VALUE
            | GUI_PEAK
            | GUI_WIDTH
            | GUI_HEIGHT;
}

static void vu_functionGetParameters (t_object *o, data::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, vu_flags());
}

static void vu_functionSetParameters (t_object *o, const data::Group& group)
{
    gui_setParameters (o, group, vu_flags());
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *vu_new (t_symbol *s, int argc, t_atom *argv)
{
    t_vu *x = (t_vu *)pd_new (vu_class);

    int widthDefault    = GUI_SIZE_DEFAULT;
    int heightDefault   = GUI_SIZE_DEFAULT * 8;
    int width           = (argc > 1) ? (int)atom_getFloat (argv + 0) : widthDefault;
    int height          = (argc > 1) ? (int)atom_getFloat (argv + 1) : heightDefault;
    float value         = -100.0f;
    float peak          = -100.0f;

    gui_updateWidth (cast_gui (x), width, 0);
    gui_updateHeight (cast_gui (x), height, 0);
    gui_updateValue (cast_gui (x), value, 0);
    gui_updatePeak (cast_gui (x), peak, 0);
    
    inlet_new2 (x, &s_float);
    
    x->x_outletLeft  = outlet_newFloat (cast_object (x));
    x->x_outletRight = outlet_newFloat (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void vu_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_vu,
            (t_newmethod)vu_new,
            NULL,
            sizeof (t_vu),
            CLASS_DEFAULT,
            A_GIMME,
            A_NULL);
        
    class_addBang (c, (t_method)vu_bang);
    class_addFloat (c, (t_method)vu_float);
    
    class_addMethod (c, (t_method)vu_peak,      sym__inlet2,    A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)vu_size,      sym_size,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)vu_restore,   sym__restore,   A_NULL);

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, vu_functionGetParameters, vu_functionSetParameters);
    
    #endif
    
    class_setSaveFunction (c, vu_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);

    vu_class = c;
}

void vu_destroy (void)
{
    class_free (vu_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
