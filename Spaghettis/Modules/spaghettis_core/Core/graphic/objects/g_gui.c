
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int gui_updateValue (t_gui *x, t_float f, int notify)
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

void gui_updateRange (t_gui *x, t_float minimum, t_float maximum, int notify)
{
    t_float min = x->x_minimum;
    t_float max = x->x_maximum;

    x->x_minimum = PD_MIN (minimum, maximum);
    x->x_maximum = PD_MAX (minimum, maximum);
    
    if (notify) {
        if (min != x->x_minimum) {
            #if defined ( PD_BUILDING_APPLICATION )
            outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Low));
            #endif
        }
        if (max != x->x_maximum) {
            #if defined ( PD_BUILDING_APPLICATION )
            outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::High));
            #endif
        }
    }
}

void gui_updateInterval (t_gui *x, t_float interval, int notify)
{
    t_float step = PD_MAX (0.0, interval);
    
    if (x->x_interval != step) {
    //
    x->x_interval = step;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Interval));
        #endif
    }
    //
    }
}

void gui_updateLogarithmic (t_gui *x, int isLogarithmic, int notify)
{
    if (x->x_isLogarithmic != isLogarithmic) {
    //
    x->x_isLogarithmic = isLogarithmic;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Logarithmic));
        #endif
    }
    //
    }
}

void gui_updateDigits (t_gui *x, int digits, int notify)
{
    int n = PD_CLAMP (digits, GUI_DIGITS_MINIMUM, GUI_DIGITS_MAXIMUM);
    
    if (x->x_digits != n) {
    //
    x->x_digits = n;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Digits));
        #endif
    }
    //
    }
}

void gui_updateWidth (t_gui *x, int width, int notify)
{
    int n = PD_CLAMP (width, GUI_SIZE_MINIMUM, GUI_SIZE_MAXIMUM);
    
    if (x->x_width != n) {
    //
    x->x_width = n;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Width));
        #endif
    }
    //
    }
}

void gui_updateHeight (t_gui *x, int height, int notify)
{
    int n = PD_CLAMP (height, GUI_SIZE_MINIMUM, GUI_SIZE_MAXIMUM);
    
    if (x->x_height != n) {
    //
    x->x_height = n;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Height));
        #endif
    }
    //
    }
}

void gui_updateOrientation (t_gui *x, int isVertical, int notify)
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

void gui_updateOrientationSwap (t_gui *x, int isVertical, int notify)
{
    if (x->x_isVertical != isVertical) {
    //
    const int h = x->x_height;
    const int w = x->x_width;
    
    x->x_isVertical = isVertical;
    x->x_width      = h;
    x->x_height     = w;
    
    if (notify) {
    //
    #if defined ( PD_BUILDING_APPLICATION )
    outputs_objectUpdated (cast_object (x), Tags::parameters ( { Tag::Vertical, Tag::Width, Tag::Height } ));
    #endif
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void gui_getParameters (t_object *o, core::Group& group, const Tags& t, int flags)
{
    t_gui *x = cast_gui (o);
    
    static DelegateCache delegate;
    
    if ((flags & GUI_VALUE) && t.contains (Tag::Value)) {
        group.addParameter (Tag::Value,
            NEEDS_TRANS ("Value"),
            NEEDS_TRANS ("Value of content"),
            gui_getValue (x),
            delegate);
    }
    
    if ((flags & GUI_LOW) && t.contains (Tag::Low)) {
        group.addParameter (Tag::Low,
            NEEDS_TRANS ("Low Range"),
            NEEDS_TRANS ("Minimum settable value"),
            gui_getMinimum (x),
            delegate);
    }
    
    if ((flags & GUI_HIGH) && t.contains (Tag::High)) {
        group.addParameter (Tag::High,
            NEEDS_TRANS ("High Range"),
            NEEDS_TRANS ("Maximum settable value"),
            gui_getMaximum (x),
            delegate);
    }
    
    if ((flags & GUI_INTERVAL) && t.contains (Tag::Interval)) {
        group.addParameter (Tag::Interval,
            NEEDS_TRANS ("Interval"),
            NEEDS_TRANS ("Step between settable values"),
            gui_getInterval (x),
            delegate).setPositive<t_float>();
    }
    
    if ((flags & GUI_LOGARITHMIC) && t.contains (Tag::Logarithmic)) {
        group.addParameter (Tag::Logarithmic,
            NEEDS_TRANS ("Logarithmic"),
            NEEDS_TRANS ("Scale is logarithmic"),
            static_cast<bool> (gui_isLogarithmic (x)),
            delegate);
    }
    
    if ((flags & GUI_DIGITS) && t.contains (Tag::Digits)) {
        group.addParameter (Tag::Digits,
            NEEDS_TRANS ("Digits"),
            NEEDS_TRANS ("Number of digits"),
            gui_getDigits (x),
            delegate).setRange (juce::Range<int> (GUI_DIGITS_MINIMUM, GUI_DIGITS_MAXIMUM));
    }
    
    if ((flags & GUI_WIDTH) && t.contains (Tag::Width)) {
        group.addParameter (Tag::Width,
            NEEDS_TRANS ("Width"),
            NEEDS_TRANS ("Width of object"),
            gui_getWidth (x),
            delegate).setRange (juce::Range<int> (GUI_SIZE_MINIMUM, GUI_SIZE_MAXIMUM));
    }
    
    if ((flags & GUI_HEIGHT) && t.contains (Tag::Height)) {
        group.addParameter (Tag::Height,
            NEEDS_TRANS ("Height"),
            NEEDS_TRANS ("Height of object"),
            gui_getHeight (x),
            delegate).setRange (juce::Range<int> (GUI_SIZE_MINIMUM, GUI_SIZE_MAXIMUM));
    }
    
    if ((flags & GUI_ORIENTATION) && t.contains (Tag::Vertical)) {
        group.addParameter (Tag::Vertical,
            NEEDS_TRANS ("Vertical"),
            NEEDS_TRANS ("Orientation is vertical"),
            static_cast<bool> (gui_isVertical (x)),
            delegate);
    }
}

void gui_setParameters (t_object *o, const core::Group& group, int flags)
{
    t_gui *x = (t_gui *)o;
    
    if (flags & GUI_LOW)  {
    if (flags & GUI_HIGH) {
        jassert (group.hasParameter (Tag::Low));
        jassert (group.hasParameter (Tag::High));
        const t_float min  = group.getParameter (Tag::Low).getValueTyped<t_float>();
        const t_float max  = group.getParameter (Tag::High).getValueTyped<t_float>();
        gui_updateRange (x, min, max, 1);
    }
    }
    
    if (flags & GUI_INTERVAL) {
        jassert (group.hasParameter (Tag::Interval));
        const t_float interval = group.getParameter (Tag::Interval).getValueTyped<t_float>();
        gui_updateInterval (x, interval, 1);
    }
    
    if (flags & GUI_LOGARITHMIC) {
        jassert (group.hasParameter (Tag::Logarithmic));
        const bool logarithmic = group.getParameter (Tag::Logarithmic).getValueTyped<bool>();
        gui_updateLogarithmic (x, logarithmic, 1);
    }
    
    if (flags & GUI_DIGITS) {
        jassert (group.hasParameter (Tag::Digits));
        const int digits = group.getParameter (Tag::Digits).getValueTyped<int>();
        gui_updateDigits (x, digits, 1);
    }
    
    if (flags & GUI_WIDTH) {
        jassert (group.hasParameter (Tag::Width));
        const int width = group.getParameter (Tag::Width).getValueTyped<int>();
        gui_updateWidth (x, width, 1);
    }
    
    if (flags & GUI_HEIGHT) {
        jassert (group.hasParameter (Tag::Height));
        const int height = group.getParameter (Tag::Height).getValueTyped<int>();
        gui_updateHeight (x, height, 1);
    }
    
    if (flags & GUI_ORIENTATION) {
        jassert (group.hasParameter (Tag::Vertical));
        const bool vertical = group.getParameter (Tag::Vertical).getValueTyped<bool>();
        if (flags & GUI_SWAP) { gui_updateOrientationSwap (x, vertical, 1); }
        else {
            gui_updateOrientation (x, vertical, 1);
        }
    }
    
    /* At last. */
    
    if (flags & GUI_VALUE) {
        jassert (group.hasParameter (Tag::Value));
        const t_float f = group.getParameter (Tag::Value).getValueTyped<t_float>();
        if (gui_updateValue (x, f, 1)) { pd_bang (cast_pd (o)); }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
