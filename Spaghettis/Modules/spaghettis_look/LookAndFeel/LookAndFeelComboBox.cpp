
/* Copyright (c) 2020 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font LNF::getComboBoxFont() const
{
    return Fonts::getMenuFont();
}

juce::Font LNF::getComboBoxFont (juce::ComboBox& box)
{
    return getComboBoxFont();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawComboBox (juce::Graphics& g,
    int width,
    int height,
    bool,
    int,
    int,
    int,
    int,
    juce::ComboBox& box)
{
    const bool popupActive = box.isPopupActive();
    
    juce::Rectangle<int> b (0, 0, width, height);
    
    g.setColour (Colours::fetchColour (popupActive ? Colours::devicesComboBoxBackgroundActive
                                                   : Colours::devicesComboBoxBackground));
    g.fillRect (b);
    g.setColour (Colours::fetchColour (Colours::devicesComboBoxArrow));
    
    if (!popupActive) {
        b.removeFromRight (b.getCentreY());
        LNF::drawArrowDown (g, b.removeFromRight (b.getCentreY()));
    }
}

void LNF::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    const juce::Rectangle<int> b (0, 0, box.getWidth(), box.getHeight());
    
    label.setBounds (b.withTrimmedRight (b.getCentreY() * 2).reduced (1));
    label.setFont (getComboBoxFont());
}

void LNF::drawComboBoxTextWhenNothingSelected (juce::Graphics&, juce::ComboBox&, juce::Label&)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
