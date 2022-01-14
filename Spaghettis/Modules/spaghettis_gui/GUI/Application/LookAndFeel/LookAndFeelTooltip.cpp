
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::TextLayout LookAndFeel::getTooltipLayout (const juce::String& text)
{
    const float maximum = 400;
    
    juce::AttributedString s;
    s.setJustification (juce::Justification::centred);
    s.append (text, getTooltipsFont(), findColour (Colours::tooltipText));

    juce::TextLayout t;
    t.createLayoutWithBalancedLineLengths (s, maximum);
    return t;
}

juce::Rectangle<int> LookAndFeel::getTooltipBounds (const juce::String& text,
    juce::Point<int> pt,
    juce::Rectangle<int> area)
{
    const float extra = getTooltipsFont().getHeight();
    const int offsetX = 12;
    const int offsetY = 6;
    
    const juce::TextLayout t (getTooltipLayout (text));
    
    const int w = static_cast<int> (t.getWidth()  + extra);
    const int h = static_cast<int> (t.getHeight() + extra / 2.0);
    
    const int x = pt.x > area.getCentreX() ? (pt.x - (w + offsetX)) : (pt.x + offsetX);
    const int y = pt.y > area.getCentreY() ? (pt.y - (h + offsetY)) : (pt.y + offsetY);

    return juce::Rectangle<int> (x, y, w, h).constrainedWithin (area);
}

void LookAndFeel::drawTooltip (juce::Graphics& g, const juce::String& text, int width, int height)
{
    g.fillAll (findColour (Colours::tooltipBackground));

    getTooltipLayout (text).draw (g, juce::Rectangle<float> (width, height));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
