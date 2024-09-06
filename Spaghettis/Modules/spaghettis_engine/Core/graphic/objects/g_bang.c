
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_class *bng_class;             /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _bng {
    t_gui       x_obj;          /* MUST be the first. */
    t_outlet    *x_outlet;
    t_clock     *x_clock;
    } t_bng;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bng_taskFlash (t_bng *x)
{
    gui_updateFlashed (cast_gui (x), 0, GUI_UPDATE_NOTIFY);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bng_bang (t_bng *x)
{
    gui_updateFlashed (cast_gui (x), 1, GUI_UPDATE_NOTIFY);
    
    clock_delay (x->x_clock, gui_getTime (cast_gui (x)));

    outlet_bang (x->x_outlet);
}

static void bng_float (t_bng *x, t_float f)
{
    bng_bang (x);
}

static void bng_symbol (t_bng *x, t_symbol *s)
{
    bng_bang (x);
}

static void bng_list (t_bng *x, t_symbol *s, int argc, t_atom *argv)
{
    bng_bang (x);
}

static void bng_anything (t_bng *x, t_symbol *s, int argc, t_atom *argv)
{
    bng_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bng_flashtime (t_bng *x, t_float f)
{
    gui_updateTime (cast_gui (x), (int)f, GUI_UPDATE_NOTIFY);
}

static void bng_size (t_bng *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        gui_updateWidth (cast_gui (x), (int)atom_getFloatAtIndex (0, argc, argv), GUI_UPDATE_NOTIFY);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int bng_flags()
{
    return GUI_NONE
            | GUI_FLASHED
            | GUI_TIME
            | GUI_WIDTH;
}

#if defined ( PD_BUILDING_APPLICATION )

static void bng_functionGetParameters (t_object *o, data::Group& group, const Tags& t)
{
    gui_getParameters (o, group, t, bng_flags());
}

static void bng_functionSetParameters (t_object *o, const data::Group& group)
{
    gui_setParameters (o, group, bng_flags());
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void bng_functionSave (t_object *z, t_buffer *b, int flags)
{
    t_bng *x = (t_bng *)z;
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, sym_obj);
    buffer_appendFloat (b,  object_getX (cast_object (x)));
    buffer_appendFloat (b,  object_getY (cast_object (x)));
    buffer_appendSymbol (b, sym_bng);
    buffer_appendFloat (b,  gui_getWidth (cast_gui (x)));
    buffer_appendFloat (b,  gui_getTime (cast_gui (x)));
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void bng_restore (t_bng *x)
{
    gui_restore (cast_gui (x), bng_flags());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *bng_new (t_symbol *s, int argc, t_atom *argv)
{
    t_bng *x = (t_bng *)pd_new (bng_class);
    
    int width = (argc > 1) ? (int)atom_getFloat (argv + 0) : GUI_SIZE_DEFAULT;
    int time  = (argc > 1) ? (int)atom_getFloat (argv + 1) : GUI_TIME_DEFAULT;

    gui_updateTime (cast_gui (x), time, GUI_UPDATE_NONE);
    gui_updateWidth (cast_gui (x), width, GUI_UPDATE_NONE);
    
    x->x_outlet = outlet_newBang (cast_object (x));
    x->x_clock  = clock_newSingle ((void *)x, (t_method)bng_taskFlash);
    
    return x;
}

static void bng_free (t_bng *x)
{
    clock_free (x->x_clock);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void bng_setup (void)
{
    t_class *c = NULL;
    
    c = class_new (sym_bng, 
            (t_newmethod)bng_new, 
            (t_method)bng_free, 
            sizeof (t_bng), 
            CLASS_DEFAULT,
            A_GIMME, 
            A_NULL);
    
    class_addBang (c, (t_method)bng_bang);
    class_addFloat (c, (t_method)bng_float);
    class_addSymbol (c, (t_method)bng_symbol);
    class_addList (c, (t_method)bng_list);
    class_addAnything (c, (t_method)bng_anything);
    
    class_addMethod (c, (t_method)bng_flashtime,    sym_flashtime,  A_FLOAT, A_NULL);
    class_addMethod (c, (t_method)bng_size,         sym_size,       A_GIMME, A_NULL);
    class_addMethod (c, (t_method)bng_size,         sym__resize,    A_GIMME, A_NULL);
    class_addMethod (c, (t_method)bng_restore,      sym__restore,   A_NULL);
    
    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, bng_functionGetParameters, bng_functionSetParameters);
    
    #endif
    
    class_setSaveFunction (c, bng_functionSave);
    class_setDataFunction (c, gui_functionData);
    class_requirePending (c);
    
    bng_class = c;
}

void bng_destroy (void)
{
    class_free (bng_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
