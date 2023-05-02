
/* Copyright (c) 2020 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font LookAndFeel::getComboBoxFont() const
{
    return getMenuFont();
}

juce::Font LookAndFeel::getComboBoxFont (juce::ComboBox& box)
{
    return getComboBoxFont();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawComboBox (juce::Graphics& g,
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
    
    g.setColour (findColour (popupActive ? Colours::devicesComboBoxBackgroundActive
                                         : Colours::devicesComboBoxBackground));
    g.fillRect (b);
    g.setColour (findColour (Colours::devicesComboBoxArrow));
    
    if (!popupActive) {
        b.removeFromRight (b.getCentreY());
        LookAndFeel::drawArrowDown (g, b.removeFromRight (b.getCentreY()));
    }
}

void LookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    const juce::Rectangle<int> b (0, 0, box.getWidth(), box.getHeight());
    
    label.setBounds (b.withTrimmedRight (b.getCentreY() * 2).reduced (1));
    label.setFont (getComboBoxFont());
}

void LookAndFeel::drawComboBoxTextWhenNothingSelected (juce::Graphics&, juce::ComboBox&, juce::Label&)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
