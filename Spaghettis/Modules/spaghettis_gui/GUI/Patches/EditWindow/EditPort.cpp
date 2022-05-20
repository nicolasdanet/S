
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

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
        
        return f;
    };
    
    setOrigin (getOrigin().translated (-map (x), -map (y)));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* The order matter below (due to side effects). */

void EditPort::zoom (int n)
{
    if (getZoom() != n) {
    //
    const juce::Point<float> p = getCentralPoint();
    
    setZoom (n);
    
    setCentralPoint (p);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::update()
{
    const float f = getScale();
    
    view_.setBounds (core::Canvas::getArea (f) - PainterPolicy::scaled (origin_, f).toInt());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Point<float> EditPort::getOrigin() const
{
    return origin_;
}

void EditPort::setOrigin (juce::Point<float> pt)
{
    origin_ = pt; update();
}

juce::Point<float> EditPort::getCentralPoint() const
{
    return origin_ + PainterPolicy::unscaled (getBounds().getCentre().toFloat(), getScale());
}

void EditPort::setCentralPoint (juce::Point<float> pt)
{
    setOrigin (pt -  PainterPolicy::unscaled (getBounds().getCentre().toFloat(), getScale()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
