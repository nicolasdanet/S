
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::TextLayout LNF::getTooltipLayout (const juce::String& text)
{
    const float maximum = 400;
    
    juce::AttributedString s;
    s.setJustification (juce::Justification::centred);
    s.append (text, Fonts::getTooltipsFont(), Colours::fetchColour (Colours::tooltipText));

    juce::TextLayout t;
    t.createLayoutWithBalancedLineLengths (s, maximum);
    return t;
}

juce::Rectangle<int> LNF::getTooltipBounds (const juce::String& text,
    juce::Point<int> pt,
    juce::Rectangle<int> area)
{
    const float extra = Fonts::getTooltipsFont().getHeight();
    const int offsetX = 12;
    const int offsetY = 6;
    
    const juce::TextLayout t (getTooltipLayout (text));
    
    const int w = static_cast<int> (t.getWidth()  + extra);
    const int h = static_cast<int> (t.getHeight() + extra / 2.0);
    
    const int x = pt.x > area.getCentreX() ? (pt.x - (w + offsetX)) : (pt.x + offsetX);
    const int y = pt.y > area.getCentreY() ? (pt.y - (h + offsetY)) : (pt.y + offsetY);

    return juce::Rectangle<int> (x, y, w, h).constrainedWithin (area);
}

void LNF::drawTooltip (juce::Graphics& g, const juce::String& text, int width, int height)
{
    g.fillAll (Colours::fetchColour (Colours::tooltipBackground));

    getTooltipLayout (text).draw (g, juce::Rectangle<float> (width, height));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
