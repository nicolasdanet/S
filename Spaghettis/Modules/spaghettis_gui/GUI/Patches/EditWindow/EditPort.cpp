
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditPort::EditPort (EditView& view) : view_ (view), zoom_ (100)
{
    view_.setPort (this);
    
    update();
        
    addAndMakeVisible (&view_);
}
    
EditPort::~EditPort()
{
    removeChildComponent (&view_);
        
    view_.setPort (nullptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -


juce::Value EditPort::getZoomAsValue() const
{
    return zoom_;
}
    
float EditPort::getScale() const
{
    return getZoom() / 100.0f;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> EditPort::getRealVisibleArea() const
{
    const float f = getScale();
    const int w   = Distance::unscaled (getWidth(), f);
    const int h   = Distance::unscaled (getHeight(), f);

    return juce::Rectangle<int> (w, h) + offset_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::zoomIn()
{
    auto r = std::find_if (steps_.cbegin(), steps_.cend(),   [n = getZoom()](int i) { return (i > n); });
    
    zoom ((r != steps_.cend()) ? *r : steps_.back());
}

void EditPort::zoomOut()
{
    auto r = std::find_if (steps_.crbegin(), steps_.crend(), [n = getZoom()](int i) { return (i < n); });
    
    zoom ((r != steps_.crend()) ? *r : steps_.front());
}

void EditPort::zoomReset()
{
    zoom (100);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::mouseWheelMove (const juce::MouseEvent &e, const juce::MouseWheelDetails &wheel)
{
    if (origin_.has_value()) { return; }        /* Don't collide with drag operation. */
    
    const float step = 200.0f / getScale();
    
    float x = (wheel.isReversed ? -wheel.deltaX : wheel.deltaX) * step;
    float y = (wheel.isReversed ? -wheel.deltaY : wheel.deltaY) * step;

    if (e.mods.isCommandDown()) { const int n = (y > 0.0f) ? 10 : -10; zoom (getZoom() + n); }
    else {
    //
    #if JUCE_LINUX
    
    if (e.mods.isShiftDown()) { x = y; y = 0.0f; }
    
    #endif
    
    auto map = [](float f)      /* At least one step increment. */
    {
        if (f) { f = std::signbit (f) ? juce::jmin (-1.0f, f) : juce::jmax (1.0f, f); }
        
        return static_cast<int> (f);
    };
    
    offset_ += juce::Point<int> (-map (x), -map (y));
    
    update();
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::setZoom (int n)
{
    constexpr int min = steps_.front();
    constexpr int max = steps_.back();
    
    zoom_ = juce::var (juce::jlimit (min, max, n)); view_.setScale (getScale());
}
    
int EditPort::getZoom() const
{
    return static_cast <int> (zoom_.getValue());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::zoom (int n)
{
    if (getZoom() != n) { setZoom (n); update(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::dragViewStart()
{
    origin_ = offset_;
}

void EditPort::dragView (juce::Point<int> pt)
{
    if (origin_.has_value()) { offset_ = origin_.value() - pt; update(); }
}

void EditPort::dragViewEnd()
{
    origin_.reset();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::update()
{
    const juce::Point<int> pt = Coordinates::scaled (Coordinates::realToLocal (offset_), getScale());
    
    view_.setBounds (juce::Rectangle<int> (core::Canvas::getSize(), core::Canvas::getSize()) - pt);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
