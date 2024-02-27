
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditPort::EditPort (EditView& view) : view_ (view), offset_ (0, 0), zoom_ (100), v_ (zoom_)
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
    offset_ = pt;
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::updateOffset (core::Point::Real pt)
{
    setOffset (pt);
    
    update(); notify();
}

void EditPort::updateZoom (int n)
{
    setZoom (n);
    
    update(); notify();
}
    
void EditPort::updateZoomAroundPoint (int n, core::Point::Real pt)
{
    const auto [a, b] = getVisibleArea().getProportions (pt);
    
    /* Do NOT cache visible area. */
    
    setZoom (n);
    
    setOffset (getVisibleArea().getOffsetForProportions (pt, a, b));
    
    update(); notify();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::zoomIn()
{
    updateZoom (ZoomSteps::next (getZoom()));
}

void EditPort::zoomOut()
{
    updateZoom (ZoomSteps::previous (getZoom()));
}

void EditPort::zoomReset()
{
    updateZoom (100);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditPort::locate (core::UniqueId u)
{
    ObjectComponent* o = view_.getObjectComponent (u);
    
    if (!o) { return false; }
    
    const core::Point::Real pt (o->getPosition());
    
    /* If required, move visible area to reveal object. */
    
    if (!getVisibleArea().contains (pt)) {
        const core::Vector::Real v (core::Vector::Scaled (getWidth(), getHeight(), getScale()));
        updateOffset (pt - (v / 3));
    }
    
    showLocator (o->getBounds());
    
    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* At least one step increment (zero allowed). */

int withMinimumStep (float f)
{
    if (f) { f = std::signbit (f) ? juce::jmin (-1.0f, f) : juce::jmax (1.0f, f); }
        
    return static_cast<int> (f);
}

void mouseWheelMoveDisplace (EditPort* p, float x, float y)
{
    p->updateOffset (p->getOffset() + core::Vector::Real (-withMinimumStep (x), -withMinimumStep (y)));
}

void mouseWheelMoveZoom (EditPort* p, std::optional<core::Point::Real> pt, float y)
{
    if (pt.has_value() == false) { return; }        /* Happened in weird cases. */
    
    const int n = p->getZoom();
        
    p->updateZoomAroundPoint ((y > 0.0f) ? ZoomSteps::next (n) : ZoomSteps::previous (n), pt.value());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::mouseWheelMove (const juce::MouseEvent &e, const juce::MouseWheelDetails &wheel)
{
    if (view_.isDragging()) { return; }
    
    const float step = 200.0f / getScale();
    
    float x = (wheel.isReversed ? -wheel.deltaX : wheel.deltaX) * step;
    float y = (wheel.isReversed ? -wheel.deltaY : wheel.deltaY) * step;

    #if JUCE_LINUX
    
    if (Mouse::hasShiftKey (e)) { x = y; y = 0.0f; }
    
    #endif
        
    if (Mouse::hasAltKey (e)) { mouseWheelMoveZoom (this, view_.getMousePosition(), y); }
    else {
        mouseWheelMoveDisplace (this, x, y);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::update()
{
    view_.setBounds (core::Geometry::getCanvasAt (core::Point::Scaled (getOffset(), getScale())));
}

void EditPort::notify()
{
    DBG ("???");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
