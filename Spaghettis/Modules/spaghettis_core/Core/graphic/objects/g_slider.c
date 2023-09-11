
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define SLIDER_WIDTH_DEFAULT            15          /* Default is for vertical slider. */
#define SLIDER_HEIGHT_DEFAULT           128
#define SLIDER_SIZE_MINIMUM             8
#define SLIDER_SIZE_MAXIMUM             1024

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *slider_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _slider {
    t_object    x_obj;                  /* MUST be the first. */
    int         x_isVertical;
    int         x_isLogarithmic;
    int         x_width;
    int         x_height;
    t_float     x_minimum;
    t_float     x_maximum;
    t_float     x_value;
    t_outlet    *x_outlet;
    } t_slider;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_set (t_slider *, t_float);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_updateOrientation (t_slider *x, int isVertical)
{
    x->x_isVertical = (isVertical != 0);
}

static void slider_updateMode (t_slider *x, int isLogarithmic)
{
    x->x_isLogarithmic = (isLogarithmic != 0);
}

static void slider_updateWidth (t_slider *x, int width)
{
    x->x_width = PD_CLAMP (width, SLIDER_SIZE_MINIMUM, SLIDER_SIZE_MAXIMUM);
}

static void slider_updateHeight (t_slider *x, int height)
{
    x->x_height = PD_CLAMP (height, SLIDER_SIZE_MINIMUM, SLIDER_SIZE_MAXIMUM);
}

static void slider_updateRange (t_slider *x, t_float minimum, t_float maximum)
{
    x->x_minimum = minimum;
    x->x_maximum = maximum;
}

static void slider_updateValue (t_slider *x, t_float f)
{
    x->x_value = f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_bang (t_slider *x)
{
    outlet_float (x->x_outlet, x->x_value);
}

static void slider_float (t_slider *x, t_float f)
{
    slider_updateValue (x, f); slider_bang (x);
}

static void slider_size (t_slider *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) {
    //
    slider_updateWidth (x, (int)atom_getFloatAtIndex (0, argc, argv));
    slider_updateHeight (x, (int)atom_getFloatAtIndex (1, argc, argv));
    //
    }
}

static void slider_range (t_slider *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float minimum = atom_getFloatAtIndex (0, argc, argv);
    t_float maximum = atom_getFloatAtIndex (1, argc, argv);
    
    slider_updateRange (x, minimum, maximum);
}

static void slider_set (t_slider *x, t_float f)
{
    slider_updateValue (x, f);
}

static void slider_logarithmic (t_slider *x)
{
    slider_updateMode (x, 1);
}

static void slider_linear (t_slider *x)
{
    slider_updateMode (x, 0);
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
    buffer_appendSymbol (b, x->x_isVertical ? sym_vslider : sym_hslider);
    buffer_appendFloat (b,  x->x_width);
    buffer_appendFloat (b,  x->x_height);
    buffer_appendFloat (b,  x->x_minimum);
    buffer_appendFloat (b,  x->x_maximum);
    buffer_appendFloat (b,  x->x_isLogarithmic);
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_value); }
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

static void *slider_new (t_symbol *s, int argc, t_atom *argv)
{
    t_slider *x = (t_slider *)pd_new (slider_class);
    
    if (s == sym_vslider) { x->x_isVertical = 1; }
    
    int widthDefault        = x->x_isVertical ? SLIDER_WIDTH_DEFAULT  : SLIDER_HEIGHT_DEFAULT;
    int heightDefault       = x->x_isVertical ? SLIDER_HEIGHT_DEFAULT : SLIDER_WIDTH_DEFAULT;
    t_float minimumDefault  = 0.0;
    t_float maximumDefault  = (t_float)(x->x_isVertical ? (heightDefault - 1) : (widthDefault - 1));
    int width               = (argc > 4) ? (int)atom_getFloat (argv + 0) : widthDefault;
    int height              = (argc > 4) ? (int)atom_getFloat (argv + 1) : heightDefault;
    t_float minimum         = (argc > 4) ? atom_getFloat (argv + 2) : minimumDefault;
    t_float maximum         = (argc > 4) ? atom_getFloat (argv + 3) : maximumDefault;
    int isLogarithmic       = (argc > 4) ? (int)atom_getFloat (argv + 4) : 0;
    t_float value           = (argc > 5) ? atom_getFloat (argv + 5) : 0.0;

    slider_updateMode (x, isLogarithmic);
    slider_updateWidth (x, width);
    slider_updateHeight (x, height);
    slider_updateRange (x, minimum, maximum);
    slider_updateValue (x, value);
    
    x->x_outlet = outlet_newFloat (cast_object (x));
    
    return x;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void slider_setup (void)
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

    class_setHelpName (c, sym_slider);
    class_setSaveFunction (c, slider_functionSave);
    class_setDataFunction (c, object_functionData);
    class_requirePending (c);
    
    slider_class = c;
}

PD_LOCAL void slider_destroy (void)
{
    class_free (slider_class);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
