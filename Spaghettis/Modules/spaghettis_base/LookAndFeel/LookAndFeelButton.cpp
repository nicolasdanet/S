
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawButtonBackground (juce::Graphics& g,
    juce::Button& button,
    const juce::Colour&,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    const float cornerSize = 6.0f;
    const juce::Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    const juce::Colour background = (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            ? Colours::fetchColour (Colours::buttonBackgroundOver)
            : Colours::fetchColour (Colours::buttonBackground);
            
    g.setColour (background);
    g.fillRoundedRectangle (bounds, cornerSize);
    g.setColour (Colours::fetchColour (Colours::buttonOutline));
    g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
}

void LNF::drawButtonText (juce::Graphics& g, juce::TextButton& button, bool, bool)
{
    const juce::Rectangle<int> r (button.getWidth(), button.getHeight());
    
    const bool hightlighted   = button.isRegisteredForShortcut (juce::KeyPress (juce::KeyPress::returnKey));
    const juce::Colour colour = (Colours::fetchColour (hightlighted
                                    ? Colours::buttonTextHighlighted
                                    : Colours::buttonText));
        
    g.setFont (getTextButtonFont());
    g.setColour (colour);
    g.drawText (button.getButtonText(), r.reduced (10, 5), juce::Justification::centred, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font LNF::getTextButtonFont()
{
    return Fonts::getFont();
}

juce::Font LNF::getTextButtonFont (juce::TextButton&, int)
{
    return getTextButtonFont();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int LNF::getButtonHeight()
{
    return static_cast<int> (Fonts::getFont().getHeight() * 1.75);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
