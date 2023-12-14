
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ParameterColourEditor::paint (juce::Graphics& g)
{
    const juce::Colour c (Colours::getColourFromValue (value_));
    
    g.fillAll (Colours::fetchColour<Colours::parametersColourBackground>());
    
    g.fillCheckerBoard (getColourBounds().toFloat(),
        11.0f,
        11.0f,
        juce::Colours::grey.overlaidWith (c),
        juce::Colours::white.overlaidWith (c));
    
    g.setFont (font_);
    g.setColour (Colours::fetchColour<Colours::parametersColourText>());
    g.drawText (Colours::getDisplayStringFromColour (c),
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
