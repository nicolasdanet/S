
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ColourEditor::paint (juce::Graphics& g)
{
    const juce::Colour c (Colour::getColourFromValue (value_));
    
    g.fillAll (Colours::fetchColour (Colours::parametersColourBackground));
    
    g.fillCheckerBoard (getColourBounds().toFloat(),
        11.0f,
        11.0f,
        juce::Colours::grey.overlaidWith (c),
        juce::Colours::white.overlaidWith (c));
    
    g.setFont (font_);
    g.setColour (Colours::fetchColour (Colours::parametersColourText));
    g.drawText (Colour::getDisplayStringFromColour (c),
        getTextBounds(),
        juce::Justification::centredLeft,
        true);
    
    g.drawRect (getColourBounds().toFloat());
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
