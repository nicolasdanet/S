
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditPort::EditPort (EditView& view) : view_ (view), zoom_ (100), v_ (zoom_)
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
    return v_;
}

int EditPort::getZoom() const
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
    
    setZoom ((r != steps_.cend()) ? *r : steps_.back());
    
    update();
}

void EditPort::zoomOut()
{
    auto r = std::find_if (steps_.crbegin(), steps_.crend(), [n = getZoom()](int i) { return (i < n); });
    
    setZoom ((r != steps_.crend()) ? *r : steps_.front());
    
    update();
}

void EditPort::zoomReset()
{
    setZoom (100); update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::mouseWheelMoveDisplace (float x, float y)
{
    auto map = [](float f)      /* At least one step increment. */
    {
        if (f) { f = std::signbit (f) ? juce::jmin (-1.0f, f) : juce::jmax (1.0f, f); }
        
        return static_cast<int> (f);
    };
    
    offset_ += juce::Point<int> (-map (x), -map (y));
    
    update();
}

void EditPort::mouseWheelMoveZoom (float y)
{
    const auto pt = view_.getRealMousePosition();
    
    jassert (pt.has_value());
    
    setZoomAroundPoint (getZoom() + ((y > 0.0f) ? 10 : -10), pt.value());
    
    update();
}

void EditPort::mouseWheelMove (const juce::MouseEvent &e, const juce::MouseWheelDetails &wheel)
{
    if (dragOrigin_.has_value()) { return; }        /* Don't collide with drag operation. */
    
    const float step = 200.0f / getScale();
    
    float x = (wheel.isReversed ? -wheel.deltaX : wheel.deltaX) * step;
    float y = (wheel.isReversed ? -wheel.deltaY : wheel.deltaY) * step;

    #if JUCE_LINUX
    if (e.mods.isShiftDown()) { x = y; y = 0.0f; }
    #endif
        
    if (e.mods.isCommandDown()) { mouseWheelMoveZoom (y); } else { mouseWheelMoveDisplace (x, y); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::tuple<float, float> getMouseRatio (juce::Rectangle<int> area, juce::Point<int> pt)
{
    jassert (area.contains (pt));
    
    const float dX = static_cast<float> (pt.getX() - area.getX());
    const float dY = static_cast<float> (pt.getY() - area.getY());
    const float fX = dX / area.getWidth();
    const float fY = dY / area.getHeight();
    
    return { fX, fY };
}

juce::Point<int> getOffsetWithRatio (juce::Rectangle<int> area, std::tuple<float, float>)
{
    return {};
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::setZoomAroundPoint (int n, juce::Point<int> pt)
{
    auto t = getMouseRatio (getRealVisibleArea(), pt);
    
    setZoom (n);
    
    auto p = getOffsetWithRatio (getRealVisibleArea(), t);
}

void EditPort::setZoom (int n)
{
    if (getZoom() != n) {
    //
    constexpr int min = steps_.front();
    constexpr int max = steps_.back();
    
    zoom_ = juce::jlimit (min, max, n); v_ = juce::var (zoom_);
    
    view_.setScale (getScale());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::dragViewStart()
{
    dragOrigin_ = offset_;
}

void EditPort::dragView (juce::Point<int> pt)
{
    if (dragOrigin_.has_value()) { offset_ = dragOrigin_.value() - pt; update(); }
}

void EditPort::dragViewEnd()
{
    dragOrigin_.reset();
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
