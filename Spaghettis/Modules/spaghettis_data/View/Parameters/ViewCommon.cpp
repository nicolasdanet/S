
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void paintArrow (juce::Graphics& g, const juce::Rectangle<int>& r, int numberOfPanels, bool expanded)
{
    const juce::Colour c (Colours::fetchColour (Colours::parametersHeaderArrow));
    
    g.setColour ((numberOfPanels > 1) ? c : c.withAlpha (0.25f));

    if (expanded) { LNF::drawArrowDown (g, r); }
    else {
        LNF::drawArrowRight (g, r);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ViewCommon::paintHeader (juce::Graphics& g,
    const juce::Rectangle<int>& bounds,
    const juce::Font& font,
    const juce::String& name,
    int numberOfPanels,
    bool expanded)
{
    juce::Rectangle<int> b (bounds.reduced (1, 0).withTrimmedBottom (1));
    
    g.setColour (Colours::fetchColour (Colours::parametersHeaderBackground));
    g.fillRoundedRectangle (b.toFloat(), 2.0f);
    
    paintArrow (g, b.removeFromLeft (b.getCentreY()), numberOfPanels, expanded);
    
    g.setColour (Colours::fetchColour (Colours::parametersHeaderText));
    g.setFont (font);
    g.drawText (name, b.reduced (4, 0), juce::Justification::centredLeft, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
