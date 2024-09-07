
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int gui_updateValue (t_gui *x, t_float f, int flag)
{
    if (x->x_value != f) {
    //
    x->x_value = f;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Value));
        #endif
    }
    
    return 1;
    //
    }
    
    return 0;
}

int gui_updateState (t_gui *x, int n, int flag)
{
    int t = (n != 0);
    
    if (x->x_state != t) {
    //
    x->x_state = t;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::State));
        #endif
    }
    
    return 1;
    //
    }
    
    return 0;
}

void gui_updateRange (t_gui *x, t_float minimum, t_float maximum, int flag)
{
    t_float min = x->x_low;
    t_float max = x->x_high;

    x->x_low  = minimum;
    x->x_high = maximum;
    
    if (flag) {
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

void gui_updateInterval (t_gui *x, t_float interval, int flag)
{
    t_float step = PD_MAX (0.0, interval);
    
    if (x->x_interval != step) {
    //
    x->x_interval = step;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Interval));
        #endif
    }
    //
    }
}

void gui_updateNonZero (t_gui *x, t_float f, int flag)
{
    if (x->x_nonZero != f) {
    //
    x->x_nonZero = f;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::NonZero));
        #endif
    }
    //
    }
}

int gui_updatePeak (t_gui *x, t_float f, int flag)
{
    if (x->x_peak != f) {
    //
    x->x_peak = f;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Peak));
        #endif
    }
    
    return 1;
    //
    }
    
    return 0;
}

void gui_updateLogarithmic (t_gui *x, int isLogarithmic, int flag)
{
    if (x->x_isLogarithmic != isLogarithmic) {
    //
    x->x_isLogarithmic = isLogarithmic;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Logarithmic));
        #endif
    }
    //
    }
}

void gui_updateMultiple (t_gui *x, int isMultiple, int flag)
{
    if (x->x_isMultiple != isMultiple) {
    //
    x->x_isMultiple = isMultiple;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Multiple));
        #endif
    }
    //
    }
}

void gui_updateOrientation (t_gui *x, int isVertical, int flag)
{
    if (x->x_isVertical != isVertical) {
    //
    x->x_isVertical = isVertical;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Vertical));
        #endif
    }
    
    if (flag == GUI_UPDATE_NOTIFY_UNDO) {
        t_glist *glist = object_getOwner (cast_object (x));
        if (glist_undoIsOk (glist)) {
            glist_undoAppend (glist, undoorientation_new (cast_object (x), isVertical));
        }
    }
    //
    }
}

void gui_updateOrientationSwap (t_gui *x, int isVertical, int flag)
{
    if (x->x_isVertical != isVertical) {
    //
    const int h = x->x_height;
    const int w = x->x_width;
    
    x->x_isVertical = isVertical;
    x->x_width      = h;
    x->x_height     = w;
    
    if (flag) {
    //
    #if defined ( PD_BUILDING_APPLICATION )
    outputs_objectUpdated (cast_object (x), Tags::parameters ( { Tag::Vertical, Tag::Width, Tag::Height } ));
    #endif
    //
    }
    
    if (flag == GUI_UPDATE_NOTIFY_UNDO) {
        t_glist *glist = object_getOwner (cast_object (x));
        if (glist_undoIsOk (glist)) {
            glist_undoAppend (glist, undoorientation_new (cast_object (x), isVertical));
        }
    }
    //
    }
}

void gui_updateFlashed (t_gui *x, int n, int flag)
{
    int t = (n != 0);
    
    if (x->x_isFlashed != t) {
    //
    x->x_isFlashed = t;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Flashed));
        #endif
    }
    //
    }
}

void gui_updateEmbedded (t_gui *x, int n, int flag)
{
    int t = (n != 0);
    
    if (x->x_isEmbedded != t) {
    //
    x->x_isEmbedded = t;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Embedded));
        #endif
    }
    //
    }
}

void gui_updateIncluded (t_gui *x, int n, int flag)
{
    int t = (n != 0);
    
    if (x->x_isIncluded != t) {
    //
    x->x_isIncluded = t;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Included));
        #endif
    }
    //
    }
}

void gui_updateTime (t_gui *x, int n, int flag)
{
    int t = PD_CLAMP (n, GUI_TIME_MINIMUM, GUI_TIME_MAXIMUM);
    
    if (x->x_time != t) {
    //
    x->x_time = t;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::FlashTime));
        #endif
    }
    //
    }
}

void gui_updateDigits (t_gui *x, int digits, int flag)
{
    int n = PD_CLAMP (digits, GUI_DIGITS_MINIMUM, GUI_DIGITS_MAXIMUM);
    
    if (x->x_digits != n) {
    //
    x->x_digits = n;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Digits));
        #endif
    }
    //
    }
}

void gui_updateButtons (t_gui *x, int buttons, int flag)
{
    int n = PD_CLAMP (buttons, GUI_BUTTONS_MINIMUM, GUI_BUTTONS_MAXIMUM);

    if (x->x_buttons != n) {
    //
    x->x_buttons = n;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Buttons));
        #endif
    }
    //
    }
}

void gui_updateWidth (t_gui *x, int width, int flag)
{
    int newWidth = PD_CLAMP (width, GUI_SIZE_MINIMUM, GUI_SIZE_MAXIMUM);
    int oldWidth = x->x_width;
    
    if (oldWidth != newWidth) {
    //
    x->x_width = newWidth;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Width));
        #endif
    }
    
    if (flag == GUI_UPDATE_NOTIFY_UNDO) {
        t_glist *glist = object_getOwner (cast_object (x));
        if (glist_undoIsOk (glist)) {
            glist_undoAppend (glist,
                undoresize_new (cast_object (x), oldWidth, x->x_height, newWidth, x->x_height));
        }
    }
    //
    }
}

void gui_updateHeight (t_gui *x, int height, int flag)
{
    int newHeight = PD_CLAMP (height, GUI_SIZE_MINIMUM, GUI_SIZE_MAXIMUM);
    int oldHeight = x->x_height;
    
    if (oldHeight != newHeight) {
    //
    x->x_height = newHeight;
    
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectUpdated (cast_object (x), Tags::parameters (Tag::Height));
        #endif
    }
    
    if (flag == GUI_UPDATE_NOTIFY_UNDO) {
        t_glist *glist = object_getOwner (cast_object (x));
        if (glist_undoIsOk (glist)) {
            glist_undoAppend (glist,
                undoresize_new (cast_object (x), x->x_width, oldHeight, x->x_width, newHeight));
        }
    }
    //
    }
}

void gui_updateLabel (t_gui *x, t_symbol *s, int flag)
{
    if (x->x_label != s) {
    //
    x->x_label = s;
        
    if (flag) {
        #if defined ( PD_BUILDING_APPLICATION )
        outputs_objectChanged (cast_object (x), Tags::parameters (Tag::Label));
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

void gui_getParameters (t_object *o, data::Group& group, const Tags& t, int flags)
{
    t_gui *x = cast_gui (o);
    
    static data::DelegateCache delegate;
    
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
    
    if ((flags & GUI_RANGE) && t.contains (Tag::Low)) {
        group.addParameter (Tag::Low,
            NEEDS_TRANS ("Low Range"),
            NEEDS_TRANS ("Minimum settable value"),
            gui_getLow (x),
            delegate);
    }
    
    if ((flags & GUI_RANGE) && t.contains (Tag::High)) {
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
    
    if ((flags & GUI_INCLUDED) && t.contains (Tag::Included)) {
        group.addParameter (Tag::Included,
            NEEDS_TRANS ("Included"),
            NEEDS_TRANS ("Is widget included in run view"),
            static_cast<bool> (gui_isIncluded (x)),
            delegate);
    }
    
    if ((flags & GUI_INCLUDED) && t.contains (Tag::Label)) {
        group.addParameter (Tag::Label,
            NEEDS_TRANS ("Label"),
            NEEDS_TRANS ("Widget label in run view"),
            makeString (symbol_getName (gui_getLabel (x))),
            delegate);
    }
}

bool gui_setParameters (t_object *o, const data::Group& group, int flags)
{
    t_gui *x = (t_gui *)o;
    
    if (flags & GUI_INCLUDED) {
        jassert (group.hasParameter (Tag::Included));
        const int n = group.getParameter (Tag::Included).getValueTyped<bool>();
        gui_updateIncluded (x, n, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_INCLUDED) {
        jassert (group.hasParameter (Tag::Label));
        t_symbol *s = gensym (group.getParameter (Tag::Label).getValueTyped<juce::String>().toRawUTF8());
        gui_updateLabel (x, s, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_RANGE) {
        jassert (group.hasParameter (Tag::Low));
        jassert (group.hasParameter (Tag::High));
        const t_float min  = group.getParameter (Tag::Low).getValueTyped<t_float>();
        const t_float max  = group.getParameter (Tag::High).getValueTyped<t_float>();
        gui_updateRange (x, min, max, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_INTERVAL) {
        jassert (group.hasParameter (Tag::Interval));
        const t_float interval = group.getParameter (Tag::Interval).getValueTyped<t_float>();
        gui_updateInterval (x, interval, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_NONZERO) {
        jassert (group.hasParameter (Tag::NonZero));
        const t_float nonzero = group.getParameter (Tag::NonZero).getValueTyped<t_float>();
        gui_updateNonZero (x, nonzero, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_LOGARITHMIC) {
        jassert (group.hasParameter (Tag::Logarithmic));
        const bool logarithmic = group.getParameter (Tag::Logarithmic).getValueTyped<bool>();
        gui_updateLogarithmic (x, logarithmic, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_MULTIPLE) {
        jassert (group.hasParameter (Tag::Multiple));
        const bool multiple = group.getParameter (Tag::Multiple).getValueTyped<bool>();
        gui_updateMultiple (x, multiple, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_EMBEDDED) {
        jassert (group.hasParameter (Tag::Embedded));
        const bool embedded = group.getParameter (Tag::Embedded).getValueTyped<bool>();
        gui_updateEmbedded (x, embedded, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_TIME) {
        jassert (group.hasParameter (Tag::FlashTime));
        const int time = group.getParameter (Tag::FlashTime).getValueTyped<int>();
        gui_updateTime (x, time, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_DIGITS) {
        jassert (group.hasParameter (Tag::Digits));
        const int digits = group.getParameter (Tag::Digits).getValueTyped<int>();
        gui_updateDigits (x, digits, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_BUTTONS) {
        jassert (group.hasParameter (Tag::Buttons));
        const int buttons = group.getParameter (Tag::Buttons).getValueTyped<int>();
        gui_updateButtons (x, buttons, GUI_UPDATE_NOTIFY);
    }
    
    if (flags & GUI_WIDTH) {
        jassert (group.hasParameter (Tag::Width));
        const int width = group.getParameter (Tag::Width).getValueTyped<int>();
        gui_updateWidth (x, width, GUI_UPDATE_NOTIFY_UNDO);
    }
    
    if (flags & GUI_HEIGHT) {
        jassert (group.hasParameter (Tag::Height));
        const int height = group.getParameter (Tag::Height).getValueTyped<int>();
        gui_updateHeight (x, height, GUI_UPDATE_NOTIFY_UNDO);
    }
    
    /* Must be after width and height. */
    
    if (flags & GUI_ORIENTATION) {
        jassert (group.hasParameter (Tag::Vertical));
        const bool vertical = group.getParameter (Tag::Vertical).getValueTyped<bool>();
        if (flags & GUI_SWAP) { gui_updateOrientationSwap (x, vertical, GUI_UPDATE_NOTIFY_UNDO); }
        else {
            gui_updateOrientation (x, vertical, GUI_UPDATE_NOTIFY_UNDO);
        }
    }
    
    /* At last. */
    
    bool trigger = false;
    
    if (flags & GUI_VALUE) {
        jassert (group.hasParameter (Tag::Value));
        const t_float f = group.getParameter (Tag::Value).getValueTyped<t_float>();
        if (gui_updateValue (x, f, GUI_UPDATE_NOTIFY)) { trigger |= true; }
    }
    
    if (flags & GUI_PEAK) {
        jassert (group.hasParameter (Tag::Peak));
        const t_float f = group.getParameter (Tag::Peak).getValueTyped<t_float>();
        if (gui_updatePeak (x, f, GUI_UPDATE_NOTIFY)) { trigger |= true; }
    }
    
    if (flags & GUI_STATE) {
        jassert (group.hasParameter (Tag::State));
        const bool f = group.getParameter (Tag::State).getValueTyped<bool>();
        if (gui_updateState (x, f, GUI_UPDATE_NOTIFY)) { trigger |= true; }
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
// MARK: -

/*
t_buffer *gui_functionData (t_object *z, int flags)
{
    t_gui *x = cast_gui (z);
    
    if (gui_isIncluded (x) || gui_hasLabel (x)) {
    //
    t_buffer *b = buffer_new();
    
    buffer_appendSymbol (b, sym__include);
    buffer_appendFloat (b, gui_isIncluded (x));
    buffer_appendSymbol (b, gui_getLabel (x));
    
    return b;
    //
    }
    
    return NULL;
}
*/

t_buffer *gui_functionData (t_object *z, int flags)
{
    if (SAVED_DEEP (flags)) {
    //
    t_buffer *b = buffer_new();

    buffer_appendSymbol (b, sym__restore);
    
    return b;
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void gui_restore (t_gui *x, int flags)
{
    t_gui *y = (t_gui *)instance_pendingFetch (cast_object (x));
    
    if (y) {
    //
    int n = GUI_UPDATE_NOTIFY;
    
    if (flags & GUI_RANGE)          { gui_updateRange (x, gui_getLow (y), gui_getHigh (y), n);  }
    if (flags & GUI_INTERVAL)       { gui_updateInterval (x, gui_getInterval (y), n);           }
    if (flags & GUI_NONZERO)        { gui_updateNonZero (x, gui_getNonZero (y), n);             }
    if (flags & GUI_LOGARITHMIC)    { gui_updateLogarithmic (x, gui_isLogarithmic (y), n);      }
    if (flags & GUI_MULTIPLE)       { gui_updateMultiple (x, gui_isMultiple (y), n);            }
    if (flags & GUI_EMBEDDED)       { gui_updateEmbedded (x, gui_isEmbedded (y), n);            }
    if (flags & GUI_TIME)           { gui_updateTime (x, gui_getTime (y), n);                   }
    if (flags & GUI_DIGITS)         { gui_updateDigits (x, gui_getDigits (y), n);               }
    if (flags & GUI_BUTTONS)        { gui_updateButtons (x, gui_getButtons (y), n);             }
    if (flags & GUI_WIDTH)          { gui_updateWidth (x, gui_getWidth (y), n);                 }
    if (flags & GUI_HEIGHT)         { gui_updateHeight (x, gui_getHeight (y), n);               }
    if (flags & GUI_ORIENTATION)    { gui_updateOrientation (x, gui_isVertical (y), n);         }
    if (flags & GUI_VALUE)          { gui_updateValue (x, gui_getValue (y), n);                 }
    if (flags & GUI_PEAK)           { gui_updatePeak (x, gui_getPeak (y), n);                   }
    if (flags & GUI_STATE)          { gui_updateState (x, gui_getState (y), n);                 }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
