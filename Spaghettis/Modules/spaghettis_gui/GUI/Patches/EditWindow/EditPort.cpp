
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
    const float step = 100.0f;
    
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

void EditPort::setZoom (int n)
{
    constexpr int min = steps_.front();
    constexpr int max = steps_.back();
    
    zoom_ = juce::jlimit (min, max, n);
    
    view_.setScale (getScale());
    
    setOrigin (getOrigin());
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
    origin_ = pt; view_.setBounds (core::Canvas::getBoundsAlignedOnZero() - origin_.toInt());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
