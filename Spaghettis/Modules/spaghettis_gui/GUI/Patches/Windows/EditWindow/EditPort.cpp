
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditPort::EditPort (EditView& view, core::Point::Real pt, int zoom) :
    view_ (view),
    offset_ (pt),
    zoom_ (100),
    v_ (zoom_)
{
    view_.setPort (this);
    
    setZoom (zoom); update (false);
        
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

core::Point::Real EditPort::getOffset() const
{
    return offset_;
}

float EditPort::getScale() const
{
    return getZoom() / 100.0f;
}

int EditPort::getZoom() const
{
    return zoom_;
}

juce::Value EditPort::getZoomAsValue() const
{
    return v_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::Area::Real EditPort::getVisibleArea() const
{
    return core::Area::Real (getOffset(), core::Vector::Scaled (getWidth(), getHeight(), getScale()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::setOffset (core::Point::Real pt)
{
    offset_ = pt; update (true);
}

void EditPort::setZoom (int n)
{
    hideLocator();
    
    constexpr int min = ZoomSteps::min();
    constexpr int max = ZoomSteps::max();
    
    zoom_ = juce::jlimit (min, max, n);
    v_    = juce::var (zoom_);
    
    view_.setScale (getScale());
}

void EditPort::setZoomAroundPoint (int n, core::Point::Real pt)
{
    const auto [a, b] = getVisibleArea().getProportions (pt);
    
    /* Do NOT cache visible area. */
    
    setZoom (n);
    
    setOffset (getVisibleArea().getOffsetForProportions (pt, a, b));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::zoomIn()
{
    setZoom (ZoomSteps::next (getZoom()));
    
    update (true);
}

void EditPort::zoomOut()
{
    setZoom (ZoomSteps::previous (getZoom()));
    
    update (true);
}

void EditPort::zoomReset()
{
    setZoom (100); update (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditPort::locate (core::UniqueId u)
{
    ObjectComponent* o = view_.getObjectComponent (u);
    
    if (o) { showObject (o); showLocator (o->getBounds()); return true; }
    
    return false;
}

void EditPort::hideLocator()
{
    if (locator_) { view_.removeChildComponent (locator_.get()); locator_ = nullptr; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::showObject (ObjectComponent* o)
{
    const core::Point::Real pt (o->getPosition());
    
    if (!getVisibleArea().contains (pt)) {
    //
    const core::Vector::Real v (core::Vector::Scaled (getWidth(), getHeight(), getScale()));
    
    setOffset (pt - (v / 3));
    //
    }
}

void EditPort::showLocator (const juce::Rectangle<int>& bounds)
{
    hideLocator();
    
    locator_ = std::make_unique<Locator> (bounds);
    
    view_.addAndMakeVisible (locator_.get());
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
    
    setOffset (getOffset() + core::Vector::Real (-map (x), -map (y)));
}

void EditPort::mouseWheelMoveZoom (float y)
{
    const int n = getZoom();
    
    const std::optional<core::Point::Real> pt = view_.getMousePosition();
    
    if (pt.has_value() == false) { return; }        /* Happened in weird cases. */
    
    setZoomAroundPoint ((y > 0.0f) ? ZoomSteps::next (n) : ZoomSteps::previous (n), pt.value());
}

void EditPort::mouseWheelMove (const juce::MouseEvent &e, const juce::MouseWheelDetails &wheel)
{
    if (origin_.has_value()) { return; }    /* Don't collide with drag operation. */
    
    const float step = 200.0f / getScale();
    
    float x = (wheel.isReversed ? -wheel.deltaX : wheel.deltaX) * step;
    float y = (wheel.isReversed ? -wheel.deltaY : wheel.deltaY) * step;

    #if JUCE_LINUX
    if (Mouse::hasShiftKey (e)) { x = y; y = 0.0f; }
    #endif
        
    if (Mouse::hasAltKey (e)) { mouseWheelMoveZoom (y); } else { mouseWheelMoveDisplace (x, y); }
    
    update (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::dragViewStart()
{
    origin_ = getOffset();
}

void EditPort::dragView (core::Vector::Real pt)
{
    if (origin_.has_value()) { setOffset (origin_.value() - pt); }
}

void EditPort::dragViewEnd()
{
    origin_.reset();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::update (bool notify)
{
    view_.setBounds (core::Geometry::getCanvasAt (core::Point::Scaled (getOffset(), getScale())));
    
    if (notify) { view_.getPatchRoot().getBounds().set (view_.getIdentifier(), getOffset(), getZoom()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
