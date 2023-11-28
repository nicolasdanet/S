
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

int gui_updateState (t_gui *x, int n, int notify)
{
    int t = (n != 0);
    
    if (x->x_state != t) {
    //
    x->x_state = t;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::State));
        #endif
    }
    
    return 1;
    //
    }
    
    return 0;
}

void gui_updateRange (t_gui *x, t_float minimum, t_float maximum, int notify)
{
    t_float min = x->x_low;
    t_float max = x->x_high;

    x->x_low  = minimum;
    x->x_high = maximum;
    
    if (notify) {
        if (min != x->x_low) {
            #if defined ( PD_BUILDING_APPLICATION )
            outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Low));
            #endif
        }
        if (max != x->x_high) {
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

void gui_updateNonZero (t_gui *x, t_float f, int notify)
{
    if (x->x_nonZero != f) {
    //
    x->x_nonZero = f;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::NonZero));
        #endif
    }
    //
    }
}

int gui_updatePeak (t_gui *x, t_float f, int notify)
{
    if (x->x_peak != f) {
    //
    x->x_peak = f;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Peak));
        #endif
    }
    
    return 1;
    //
    }
    
    return 0;
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

void gui_updateMultiple (t_gui *x, int isMultiple, int notify)
{
    if (x->x_isMultiple != isMultiple) {
    //
    x->x_isMultiple = isMultiple;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Multiple));
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

#if defined ( PD_BUILDING_APPLICATION )

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
    outputs_objectUpdated (cast_object (x), Tags::parameters ( { Tag::Vertical, Tag::Width, Tag::Height } ));
    //
    }
    //
    }
}

#endif

void gui_updateFlashed (t_gui *x, int n, int notify)
{
    int t = (n != 0);
    
    if (x->x_isFlashed != t) {
    //
    x->x_isFlashed = t;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Flashed));
        #endif
    }
    //
    }
}

void gui_updateEmbedded (t_gui *x, int n, int notify)
{
    int t = (n != 0);
    
    if (x->x_isEmbedded != t) {
    //
    x->x_isEmbedded = t;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Embedded));
        #endif
    }
    //
    }
}

void gui_updateTime (t_gui *x, int n, int notify)
{
    int t = PD_CLAMP (n, GUI_TIME_MINIMUM, GUI_TIME_MAXIMUM);
    
    if (x->x_time != t) {
    //
    x->x_time = t;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::FlashTime));
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

void gui_updateButtons (t_gui *x, int buttons, int notify)
{
    int n = PD_CLAMP (buttons, GUI_BUTTONS_MINIMUM, GUI_BUTTONS_MAXIMUM);

    if (x->x_buttons != n) {
    //
    x->x_buttons = n;
    
    if (notify) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Buttons));
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
    
    if ((flags & GUI_STATE) && t.contains (Tag::State)) {
        group.addParameter (Tag::State,
            NEEDS_TRANS ("State"),
            NEEDS_TRANS ("State on/off"),
            static_cast<bool> (gui_getState (x)),
            delegate);
    }
    
    if ((flags & GUI_LOW) && t.contains (Tag::Low)) {
        group.addParameter (Tag::Low,
            NEEDS_TRANS ("Low Range"),
            NEEDS_TRANS ("Minimum settable value"),
            gui_getLow (x),
            delegate);
    }
    
    if ((flags & GUI_HIGH) && t.contains (Tag::High)) {
        group.addParameter (Tag::High,
            NEEDS_TRANS ("High Range"),
            NEEDS_TRANS ("Maximum settable value"),
            gui_getHigh (x),
            delegate);
    }
    
    if ((flags & GUI_INTERVAL) && t.contains (Tag::Interval)) {
        group.addParameter (Tag::Interval,
            NEEDS_TRANS ("Interval"),
            NEEDS_TRANS ("Step between settable values"),
            gui_getInterval (x),
            delegate).setPositive<t_float>();
    }
    
    if ((flags & GUI_NONZERO) && t.contains (Tag::NonZero)) {
        group.addParameter (Tag::NonZero,
            NEEDS_TRANS ("Non-zero"),
            NEEDS_TRANS ("Value for non-zero state"),
            gui_getNonZero (x),
            delegate);
    }
    
    if ((flags & GUI_PEAK) && t.contains (Tag::Peak)) {
        group.addParameter (Tag::Peak,
            NEEDS_TRANS ("Peak"),
            NEEDS_TRANS ("Peak value"),
            gui_getPeak (x),
            delegate);
    }
    
    if ((flags & GUI_LOGARITHMIC) && t.contains (Tag::Logarithmic)) {
        group.addParameter (Tag::Logarithmic,
            NEEDS_TRANS ("Logarithmic"),
            NEEDS_TRANS ("Scale is logarithmic"),
            static_cast<bool> (gui_isLogarithmic (x)),
            delegate);
    }
    
    if ((flags & GUI_MULTIPLE) && t.contains (Tag::Multiple)) {
        group.addParameter (Tag::Multiple,
            NEEDS_TRANS ("Multiple"),
            NEEDS_TRANS ("Set in multiple mode"),
            static_cast<bool> (gui_isMultiple (x)),
            delegate);
    }
    
    if ((flags & GUI_ORIENTATION) && t.contains (Tag::Vertical)) {
        group.addParameter (Tag::Vertical,
            NEEDS_TRANS ("Vertical"),
            NEEDS_TRANS ("Orientation is vertical"),
            static_cast<bool> (gui_isVertical (x)),
            delegate);
    }
    
    if ((flags & GUI_FLASHED) && t.contains (Tag::Flashed)) {
        group.addParameter (Tag::Flashed,
            NEEDS_TRANS ("Flashed"),
            NEEDS_TRANS ("Light is flashing"),
            static_cast<bool> (x->x_isFlashed),
            delegate).setHidden (true);
    }
    
    if ((flags & GUI_EMBEDDED) && t.contains (Tag::Embedded)) {
        group.addParameter (Tag::Embedded,
            NEEDS_TRANS ("Embedded"),
            NEEDS_TRANS ("Content saved with patch"),
            static_cast<bool> (x->x_isEmbedded),
            delegate);
    }
    
    if ((flags & GUI_TIME) && t.contains (Tag::FlashTime)) {
        group.addParameter (Tag::FlashTime,
            NEEDS_TRANS ("Flash Time"),
            NEEDS_TRANS ("Duration of the flash"),
            x->x_time,
            delegate).setRange (juce::Range<int> (GUI_TIME_MINIMUM, GUI_TIME_MAXIMUM));
    }
    
    if ((flags & GUI_DIGITS) && t.contains (Tag::Digits)) {
        group.addParameter (Tag::Digits,
            NEEDS_TRANS ("Digits"),
            NEEDS_TRANS ("Number of digits"),
            gui_getDigits (x),
            delegate).setRange (juce::Range<int> (GUI_DIGITS_MINIMUM, GUI_DIGITS_MAXIMUM));
    }
    
    if ((flags & GUI_BUTTONS) && t.contains (Tag::Buttons)) {
        group.addParameter (Tag::Buttons,
            NEEDS_TRANS ("Buttons"),
            NEEDS_TRANS ("Number of buttons"),
            gui_getButtons (x),
            delegate).setRange (juce::Range<int> (GUI_BUTTONS_MINIMUM, GUI_BUTTONS_MAXIMUM));
    }
    
    if ((flags & GUI_TEXT) && t.contains (Tag::Text)) {
        group.addParameter (Tag::Text,
            NEEDS_TRANS ("Text"),
            NEEDS_TRANS ("Text of object"),
            object_getBufferAsString (o),
            delegate);
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
}

bool gui_setParameters (t_object *o, const core::Group& group, int flags)
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
    
    if (flags & GUI_NONZERO) {
        jassert (group.hasParameter (Tag::NonZero));
        const t_float nonzero = group.getParameter (Tag::NonZero).getValueTyped<t_float>();
        gui_updateNonZero (x, nonzero, 1);
    }
    
    if (flags & GUI_LOGARITHMIC) {
        jassert (group.hasParameter (Tag::Logarithmic));
        const bool logarithmic = group.getParameter (Tag::Logarithmic).getValueTyped<bool>();
        gui_updateLogarithmic (x, logarithmic, 1);
    }
    
    if (flags & GUI_MULTIPLE) {
        jassert (group.hasParameter (Tag::Multiple));
        const bool multiple = group.getParameter (Tag::Multiple).getValueTyped<bool>();
        gui_updateMultiple (x, multiple, 1);
    }
    
    if (flags & GUI_ORIENTATION) {
        jassert (group.hasParameter (Tag::Vertical));
        const bool vertical = group.getParameter (Tag::Vertical).getValueTyped<bool>();
        if (flags & GUI_SWAP) { gui_updateOrientationSwap (x, vertical, 1); }
        else {
            gui_updateOrientation (x, vertical, 1);
        }
    }
    
    if (flags & GUI_EMBEDDED) {
        jassert (group.hasParameter (Tag::Embedded));
        const bool embedded = group.getParameter (Tag::Embedded).getValueTyped<bool>();
        gui_updateEmbedded (x, embedded, 1);
    }
    
    if (flags & GUI_TIME) {
        jassert (group.hasParameter (Tag::FlashTime));
        const int time = group.getParameter (Tag::FlashTime).getValueTyped<int>();
        gui_updateTime (x, time, 1);
    }
    
    if (flags & GUI_DIGITS) {
        jassert (group.hasParameter (Tag::Digits));
        const int digits = group.getParameter (Tag::Digits).getValueTyped<int>();
        gui_updateDigits (x, digits, 1);
    }
    
    if (flags & GUI_BUTTONS) {
        jassert (group.hasParameter (Tag::Buttons));
        const int buttons = group.getParameter (Tag::Buttons).getValueTyped<int>();
        gui_updateButtons (x, buttons, 1);
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
    
    /* At last. */
    
    bool trigger = false;
    
    if (flags & GUI_VALUE) {
        jassert (group.hasParameter (Tag::Value));
        const t_float f = group.getParameter (Tag::Value).getValueTyped<t_float>();
        if (gui_updateValue (x, f, 1)) { trigger |= true; }
    }
    
    if (flags & GUI_PEAK) {
        jassert (group.hasParameter (Tag::Peak));
        const t_float f = group.getParameter (Tag::Peak).getValueTyped<t_float>();
        if (gui_updatePeak (x, f, 1)) { trigger |= true; }
    }
    
    if (flags & GUI_STATE) {
        jassert (group.hasParameter (Tag::State));
        const bool f = group.getParameter (Tag::State).getValueTyped<bool>();
        if (gui_updateState (x, f, 1)) { trigger |= true; }
    }
    
    if (flags & GUI_TEXT) {
        jassert (group.hasParameter (Tag::Text));
        if (object_setBufferWithString (o, group.getParameter (Tag::Text).getValueTyped<juce::String>())) {
            outputs_objectUpdated (o, Tags::attributes (Tag::Content));
            outputs_objectUpdated (o, Tags::parameters (Tag::Text));
            trigger |= true;
        }
    }
    
    return trigger;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
