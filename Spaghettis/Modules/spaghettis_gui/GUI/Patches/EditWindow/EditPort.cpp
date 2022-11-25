
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
    view_.setOwner (this);
    
    update();
        
    addAndMakeVisible (&view_);
}
    
EditPort::~EditPort()
{
    removeChildComponent (&view_);
        
    view_.setOwner (nullptr);
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
    /*
    const float f                = getScale();
    const juce::Point<int> pt    = PainterPolicy::scaled (origin_, f).toInt();
    const int w                  = PainterPolicy::unscaled (getWidth(), f);
    const int h                  = PainterPolicy::unscaled (getHeight(), f);
    const juce::Rectangle<int> r = juce::Rectangle<int> (w, h) + pt;
    
    DBG (r.toString());
    */
    
    return {};
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
    
    origin_ += juce::Point<int> (-map (x), -map (y));
    
    update();
    //
    }
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

void EditPort::update()
{
    const float f = getScale();
    
    view_.setBounds (core::Canvas::getAreaScaled (f) - core::Distance::scaled (origin_, f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
