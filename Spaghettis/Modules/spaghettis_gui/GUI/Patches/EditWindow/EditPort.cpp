
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
    
    setZoom ((r != steps_.cend()) ? *r : steps_.back());
}

void EditPort::zoomOut()
{
    auto r = std::find_if (steps_.crbegin(), steps_.crend(), [n = getZoom()](int i) { return (i < n); });
    
    setZoom ((r != steps_.crend()) ? *r : steps_.front());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::mouseWheelMove (const juce::MouseEvent &e, const juce::MouseWheelDetails &wheel)
{
    const float step = 100.0f / getScale();
    
    float x = (wheel.isReversed ? -wheel.deltaX : wheel.deltaX) * step;
    float y = (wheel.isReversed ? -wheel.deltaY : wheel.deltaY) * step;

    if (e.mods.isCommandDown()) { const int n = (y > 0.0f) ? 10 : -10; setZoom (getZoom() + n); }
    else {
    //
    #if JUCE_LINUX
    
    if (e.mods.isShiftDown()) { x = y; y = 0.0f; }
    
    #endif
    
    auto map = [](float f)      /* At least one step increment. */
    {
        if (f) {
            return static_cast<int> (std::signbit (f) ? juce::jmin (-1.0f, f) : juce::jmax (1.0f, f));
        }
        
        return 0;
    };
    
    setTopLeft (getTopLeft().translated (-map (x), -map (y)));
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
    
    zoom_ = juce::jlimit (min, max, n);
    
    view_.setScale (getScale());
    
    update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Point<int> EditPort::getTopLeft() const
{
    return origin_;
}

juce::Point<int> EditPort::getCentre() const
{
    return juce::Point<int>();
}

void EditPort::setTopLeft (juce::Point<int> pt)
{
    origin_ = pt; update();
}

void EditPort::setCentre (juce::Point<int> pt)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditPort::update()
{
    const int x = static_cast<int> (origin_.getX() * getScale());
    const int y = static_cast<int> (origin_.getY() * getScale());
    
    view_.setBounds (core::Canvas::getBounds().translated (-x, -y));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
