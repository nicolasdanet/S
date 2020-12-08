
/* Copyright (c) 1997-2020 Miller Puckette and others. */

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
#define SLIDER_STEPS_PER_PIXEL          100

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_class *slider_class;           /* Shared. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

typedef struct _slider {
    t_object    x_obj;                  /* MUST be the first. */
    int         x_isVertical;
    int         x_width;
    int         x_height;
    int         x_position;
    int         x_isLogarithmic;
    t_float     x_minimum;
    t_float     x_maximum;
    t_float     x_floatValue;
    t_outlet    *x_outlet;
    } t_slider;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_set (t_slider *, t_float);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int slider_getNumberOfSteps (t_slider *x)
{
    if (x->x_isVertical) { 
        return ((x->x_height - 1) * SLIDER_STEPS_PER_PIXEL);
    } else {
        return ((x->x_width - 1) * SLIDER_STEPS_PER_PIXEL);
    }
}

static t_float slider_getStepValue (t_slider *x)
{
    if (x->x_isLogarithmic) {
        return (log (x->x_maximum / x->x_minimum) / (t_float)slider_getNumberOfSteps (x));
    } else {
        return ((x->x_maximum - x->x_minimum) / (t_float)slider_getNumberOfSteps (x));
    }
}

static t_float slider_getValue (t_slider *x)
{
    t_float f, t = slider_getStepValue (x) * (t_float)x->x_position;
    
    if (x->x_isLogarithmic) { 
        f = x->x_minimum * exp (t); 
    } else {
        f = x->x_minimum + t;
    }
    
    if ((f < 1.0e-10) && (f > -1.0e-10)) { f = 0.0; }
    
    return (t_float)f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_setRange (t_slider *x, t_float minimum, t_float maximum)
{
    t_error err = PD_ERROR_NONE;
    
    if (x->x_isLogarithmic) {
        err |= (minimum == 0.0);
        err |= (maximum * minimum < 0.0);
    }
    
    if (err) { 
        x->x_isLogarithmic = 0;
        error_invalid (sym_slider, sym_range);
    } else {
        x->x_minimum = minimum;
        x->x_maximum = maximum;
    }
}

static void slider_setWidth (t_slider *x, int width)
{
    x->x_width = PD_MAX (width, SLIDER_SIZE_MINIMUM);
    
    if ((x->x_isVertical == 0) && x->x_position > slider_getNumberOfSteps (x)) {
        x->x_position = slider_getNumberOfSteps (x);
    }
}

static void slider_setHeight (t_slider *x, int height)
{
    x->x_height = PD_MAX (height, SLIDER_SIZE_MINIMUM);
    
    if ((x->x_isVertical == 1) && x->x_position > slider_getNumberOfSteps (x)) {
        x->x_position = slider_getNumberOfSteps (x);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_bang (t_slider *x)
{
    outlet_float (x->x_outlet, x->x_floatValue);
}

static void slider_float (t_slider *x, t_float f)
{
    slider_set (x, f); slider_bang (x);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void slider_size (t_slider *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
    //
    slider_setWidth (x, (int)atom_getFloatAtIndex (0, argc, argv));
    if (argc > 1) { slider_setHeight (x, (int)atom_getFloatAtIndex (1, argc, argv)); }
    //
    }
}

static void slider_range (t_slider *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float minimum = atom_getFloatAtIndex (0, argc, argv);
    t_float maximum = atom_getFloatAtIndex (1, argc, argv);
    
    slider_setRange (x, minimum, maximum);
    
    x->x_floatValue = slider_getValue (x);
}

static void slider_set (t_slider *x, t_float f)
{
    x->x_floatValue = f;
    
    if (x->x_minimum > x->x_maximum) { f = (t_float)PD_CLAMP (f, x->x_maximum, x->x_minimum); }
    else {
        f = (t_float)PD_CLAMP (f, x->x_minimum, x->x_maximum);
    }
    
    if (x->x_isLogarithmic) { 
        x->x_position = (int)(log (f / x->x_minimum) / slider_getStepValue (x));
    } else {
        x->x_position = (int)((f - x->x_minimum) / slider_getStepValue (x));
    }
}

static void slider_logarithmic (t_slider *x)
{
    x->x_isLogarithmic = 1; 
    
    slider_setRange (x, x->x_minimum, x->x_maximum);
    
    x->x_floatValue = slider_getValue (x);
}

static void slider_linear (t_slider *x)
{
    x->x_isLogarithmic = 0;
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
    if (SAVED_DEEP (flags)) { buffer_appendFloat (b, x->x_position); }
    buffer_appendSemicolon (b);
    
    object_saveIdentifiers (z, b, flags);
}

static void slider_restore (t_slider *x)
{
    t_slider *old = (t_slider *)instance_pendingFetch (cast_object (x));
    
    if (old) { }    // -- ???
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
    int position            = (argc > 5) ? (int)atom_getFloat (argv + 5) : 0;

    x->x_position           = position;
    x->x_isLogarithmic      = (isLogarithmic != 0);
    x->x_outlet             = outlet_newFloat (cast_object (x));

    slider_setHeight (x, height);
    slider_setWidth (x, width);
    slider_setRange (x, minimum, maximum);
    
    x->x_floatValue = slider_getValue (x);
    
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
