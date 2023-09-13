
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

static void slider_updateOrientation (t_slider *x, int isVertical, int notify)
{
    if (x->x_isVertical != isVertical) {
    //
    x->x_isVertical = isVertical;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Vertical));
        #endif
    }
    //
    }
}

static void slider_updateLogarithmic (t_slider *x, int isLogarithmic, int notify)
{
    x->x_isLogarithmic = isLogarithmic;
}

static void slider_updateWidth (t_slider *x, int width, int notify)
{
    x->x_width = PD_CLAMP (width, SLIDER_SIZE_MINIMUM, SLIDER_SIZE_MAXIMUM);
}

static void slider_updateHeight (t_slider *x, int height, int notify)
{
    x->x_height = PD_CLAMP (height, SLIDER_SIZE_MINIMUM, SLIDER_SIZE_MAXIMUM);
}

static void slider_updateRange (t_slider *x, t_float minimum, t_float maximum, int notify)
{
    x->x_minimum = minimum;
    x->x_maximum = maximum;
}

static int slider_updateValue (t_slider *x, t_float f, int notify)
{
    if (x->x_value != f) {
    //
    x->x_value = f;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Value));
        #endif
    }
    
    return 1;
    //
    }
    
    return 0;
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
    slider_updateValue (x, f, 1); slider_bang (x);
}

static void slider_size (t_slider *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 1) {
    //
    slider_updateWidth (x,  (int)atom_getFloatAtIndex (0, argc, argv), 1);
    slider_updateHeight (x, (int)atom_getFloatAtIndex (1, argc, argv), 1);
    //
    }
}

static void slider_range (t_slider *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float minimum = atom_getFloatAtIndex (0, argc, argv);
    t_float maximum = atom_getFloatAtIndex (1, argc, argv);
    
    slider_updateRange (x, minimum, maximum, 1);
}

static void slider_set (t_slider *x, t_float f)
{
    slider_updateValue (x, f, 1);
}

static void slider_logarithmic (t_slider *x)
{
    slider_updateLogarithmic (x, 1, 1);
}

static void slider_linear (t_slider *x)
{
    slider_updateLogarithmic (x, 0, 1);
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

#if defined ( PD_BUILDING_APPLICATION )

static void slider_functionGetParameters (t_object *o, core::Group& group, const Tags& t)
{
    t_slider *x = (t_slider *)o;
    
    static DelegateCache delegate;
    
    if (t.contains (Tag::Value)) {
        group.addParameter (Tag::Value,
            NEEDS_TRANS ("Value"),
            NEEDS_TRANS ("Value of slider"),
            x->x_value,
            delegate);
    }
    
    if (t.contains (Tag::Low)) {
        group.addParameter (Tag::Low,
            NEEDS_TRANS ("Low Range"),
            NEEDS_TRANS ("Low value"),
            x->x_minimum,
            delegate);
    }
    
    if (t.contains (Tag::High)) {
        group.addParameter (Tag::High,
            NEEDS_TRANS ("High Range"),
            NEEDS_TRANS ("High value"),
            x->x_maximum,
            delegate);
    }
    
    if (t.contains (Tag::Vertical)) {
        group.addParameter (Tag::Vertical,
            NEEDS_TRANS ("Vertical"),
            NEEDS_TRANS ("Orientation is vertical"),
            static_cast<bool> (x->x_isVertical),
            delegate);
    }
    
    if (t.contains (Tag::Logarithmic)) {
        group.addParameter (Tag::Logarithmic,
            NEEDS_TRANS ("Logarithmic"),
            NEEDS_TRANS ("Scale is logarithmic"),
            static_cast<bool> (x->x_isLogarithmic),
            delegate);
    }
    
    if (t.contains (Tag::Width)) {
        group.addParameter (Tag::Width,
            NEEDS_TRANS ("Width"),
            NEEDS_TRANS ("Width of slider"),
            x->x_width,
            delegate).setRange (juce::Range<int> (SLIDER_SIZE_MINIMUM, SLIDER_SIZE_MAXIMUM));
    }
    
    if (t.contains (Tag::Height)) {
        group.addParameter (Tag::Height,
            NEEDS_TRANS ("Height"),
            NEEDS_TRANS ("Height of slider"),
            x->x_height,
            delegate).setRange (juce::Range<int> (SLIDER_SIZE_MINIMUM, SLIDER_SIZE_MAXIMUM));
    }
}

static void slider_functionSetParameters (t_object *o, const core::Group& group)
{
    t_slider *x = (t_slider *)o;
    
    jassert (group.hasParameter (Tag::Value));
    jassert (group.hasParameter (Tag::Low));
    jassert (group.hasParameter (Tag::High));
    jassert (group.hasParameter (Tag::Vertical));
    jassert (group.hasParameter (Tag::Logarithmic));
    jassert (group.hasParameter (Tag::Width));
    jassert (group.hasParameter (Tag::Height));
    
    const t_float f = group.getParameter (Tag::Value).getValueTyped<t_float>();
    const t_float min = group.getParameter (Tag::Low).getValueTyped<t_float>();
    const t_float max = group.getParameter (Tag::High).getValueTyped<t_float>();
    
    slider_updateRange (x, min, max, 1);
    slider_updateOrientation (x, group.getParameter (Tag::Vertical).getValueTyped<bool>(), 1);
    slider_updateLogarithmic (x, group.getParameter (Tag::Logarithmic).getValueTyped<bool>(), 1);
    slider_updateWidth (x, group.getParameter (Tag::Width).getValueTyped<int>(), 1);
    slider_updateHeight (x, group.getParameter (Tag::Height).getValueTyped<int>(), 1);
    
    if (slider_updateValue (x, f, 1)) { slider_bang (x); }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void *slider_new (t_symbol *s, int argc, t_atom *argv)
{
    t_slider *x = (t_slider *)pd_new (slider_class);
        
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

    slider_updateOrientation (x, (s == sym_vslider) ? 1 : 0, 0);
    slider_updateLogarithmic (x, (isLogarithmic != 0), 0);
    slider_updateWidth (x, width, 0);
    slider_updateHeight (x, height, 0);
    slider_updateRange (x, minimum, maximum, 0);
    slider_updateValue (x, value, 0);
    
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

    #if defined ( PD_BUILDING_APPLICATION )
    
    class_setParametersFunctions (c, slider_functionGetParameters, slider_functionSetParameters);
    
    #endif
    
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
