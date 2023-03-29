
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
    hideLocator();
    
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

juce::Rectangle<int> EditPort::getGlobalVisibleArea() const
{
    return getScreenBounds();
}

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
    setZoom (nextStep (getZoom()));
    
    update();
}

void EditPort::zoomOut()
{
    setZoom (previousStep (getZoom()));
    
    update();
}

void EditPort::zoomReset()
{
    setZoom (100); update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::locate (core::UniqueId u)
{
    ObjectComponent* o = view_.getObjectComponent (u);
    
    if (o) {
        show (o);
    }
}

void EditPort::show (ObjectComponent* o)
{
    const juce::Point<int> pt (o->getRealPosition());
    
    if (!getRealVisibleArea().reduced (40).contains (pt)) {
    //
    const float f = getScale();
    const int w   = Distance::unscaled (getWidth(), f);
    const int h   = Distance::unscaled (getHeight(), f);
    
    offset_ = pt - juce::Point<int> (w / 4, h / 3);
    
    update();
    //
    }
    
    showLocator (o->getBounds());
}

void EditPort::showLocator (const juce::Rectangle<int>& bounds)
{
    hideLocator();
    
    locator_ = std::make_unique<Locator> (bounds);
    
    view_.addAndMakeVisible (locator_.get());
}

void EditPort::hideLocator()
{
    if (locator_) { view_.removeChildComponent (locator_.get()); locator_ = nullptr; }
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
}

void EditPort::mouseWheelMoveZoom (float y)
{
    const auto pt = view_.getRealMousePosition();
    const int n   = getZoom();
    
    if (pt.has_value() == false) { return; }        /* Happened in weird cases. */
    
    setZoomAroundPoint ((y > 0.0f) ? nextStep (n) : previousStep (n), pt.value());
}

void EditPort::mouseWheelMove (const juce::MouseEvent &e, const juce::MouseWheelDetails &wheel)
{
    if (dragOrigin_.has_value()) { return; }        /* Don't collide with drag operation. */
    
    const float step = 200.0f / getScale();
    
    float x = (wheel.isReversed ? -wheel.deltaX : wheel.deltaX) * step;
    float y = (wheel.isReversed ? -wheel.deltaY : wheel.deltaY) * step;

    #if JUCE_LINUX
    if (Mouse::hasShiftKey (e)) { x = y; y = 0.0f; }
    #endif
        
    if (Mouse::hasAltKey (e)) { mouseWheelMoveZoom (y); } else { mouseWheelMoveDisplace (x, y); }
    
    update();
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::tuple<float, float> getRatioAround (juce::Rectangle<float> r, juce::Point<float> pt)
{
    jassert (r.contains (pt));
    
    const float dX = pt.getX() - r.getX();
    const float dY = pt.getY() - r.getY();
    const float rX = dX / r.getWidth();
    const float rY = dY / r.getHeight();
    
    return { rX, rY };
}

juce::Point<float> getOffsetAround (juce::Rectangle<float> r, juce::Point<float> pt, float rX, float rY)
{
    const float dX = rX * r.getWidth();
    const float dY = rY * r.getHeight();
    const float x  = pt.getX() - dX;
    const float y  = pt.getY() - dY;
    
    return { x, y };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::setZoomAroundPoint (int n, juce::Point<int> pt)
{
    const auto [a, b] = getRatioAround (getRealVisibleArea().toFloat(), pt.toFloat());
    
    setZoom (n);
    
    offset_ = getOffsetAround (getRealVisibleArea().toFloat(), pt.toFloat(), a, b).toInt();
}

void EditPort::setZoom (int n)
{
    hideLocator();
    
    constexpr int min = steps_.front();
    constexpr int max = steps_.back();
    
    zoom_ = juce::jlimit (min, max, n);
    v_    = juce::var (zoom_);
    
    view_.setScale (getScale());
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
