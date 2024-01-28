
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int PainterHelpers::getTextMargins (float f)
{
    return std::round (3 * f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PainterHelpers::getRequiredBoundsFromDimensions (PainterStrategy& p)
{
    std::optional<core::Vector::Real> t = p.getDimensions();
    
    jassert (t.has_value());
    
    return getRequiredBoundsFromVector (p, core::Vector::Scaled (t.value(), p.getScale()));
}

juce::Rectangle<int> PainterHelpers::getRequiredBoundsFromVector (PainterStrategy& p, core::Vector::Scaled v)
{
    return juce::Rectangle<int> (v.getPoint().getX(), v.getPoint().getY()) + p.getPosition().getPoint();
}

juce::Rectangle<int> PainterHelpers::getRequiredBoundsFromText (PainterStrategy& p,
    const juce::String& text,
    const juce::Font& font,
    int extra)
{
    const float f = p.getScale();
    const int w   = font.getStringWidth (text);
    const int h   = font.getHeight();
    const int k   = getTextMargins (f) * 2;
    
    core::Vector::Scaled v (w + k, h + k, f);
    juce::Rectangle<int> t (getRequiredBoundsFromVector (p, v));
    
    if (extra > 0) { t.setWidth (t.getWidth() + extra); }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
