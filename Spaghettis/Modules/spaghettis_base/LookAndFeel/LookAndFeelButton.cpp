
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

void drawAlertWindowButtonBackground (juce::Graphics& g,
    juce::Button& button,
    const juce::Colour&,
    bool buttonAsHighlighted,
    bool buttonAsDown)
{
    const float cornerSize = 6.0f;
    const juce::Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    const juce::Colour background = (buttonAsDown || buttonAsHighlighted)
            ? Colours::fetchColour (Colours::alertWindowButtonBackgroundOver)
            : Colours::fetchColour (Colours::alertWindowButtonBackground);
            
    g.setColour (background);
    g.fillRoundedRectangle (bounds, cornerSize);
    g.setColour (Colours::fetchColour (Colours::alertWindowButtonOutline));
    g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
}

void drawAlertWindowButtonText (juce::Graphics& g,
    juce::TextButton& button,
    const juce::Font& font,
    bool,
    bool)
{
    const juce::Rectangle<int> r (button.getWidth(), button.getHeight());
    
    const bool hightlighted   = button.isRegisteredForShortcut (juce::KeyPress (juce::KeyPress::returnKey));
    const juce::Colour colour = (Colours::fetchColour (hightlighted
                                    ? Colours::alertWindowButtonTextHighlighted
                                    : Colours::alertWindowButtonText));
        
    g.setFont (font);
    g.setColour (colour);
    g.drawText (button.getButtonText(), r.reduced (10, 5), juce::Justification::centred, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void drawCommonButtonBackground (juce::Graphics& g,
    juce::Button& button,
    const juce::Colour&,
    bool buttonAsHighlighted,
    bool buttonAsDown)
{
    const juce::Rectangle<int> bounds = button.getLocalBounds();
    const juce::Colour background = (buttonAsDown || buttonAsHighlighted)
            ? Colours::fetchColour (Colours::buttonBackgroundOver)
            : Colours::fetchColour (Colours::buttonBackground);
            
    g.setColour (background);
    g.fillRect (bounds);
}

void drawCommonButtonText (juce::Graphics& g,
    juce::TextButton& button,
    const juce::Font& font,
    bool buttonAsHighlighted,
    bool buttonAsDown)
{
    const juce::Rectangle<int> r (button.getWidth(), button.getHeight());
    
    const juce::Colour colour = (Colours::fetchColour (buttonAsDown
            ? Colours::buttonTextHighlighted
            : Colours::buttonText));
        
    g.setFont (font);
    g.setColour (colour);
    g.drawText (button.getButtonText(), r.reduced (10, 5), juce::Justification::centred, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawButtonBackground (juce::Graphics& g,
    juce::Button& button,
    const juce::Colour& colour,
    bool buttonAsHighlighted,
    bool buttonAsDown)
{
    if (isButtonInsideAlertWindow (button)) {
        drawAlertWindowButtonBackground (g, button, colour, buttonAsHighlighted, buttonAsDown);
    } else {
        drawCommonButtonBackground (g, button, colour, buttonAsHighlighted, buttonAsDown);
    }
}

void LNF::drawButtonText (juce::Graphics& g,
    juce::TextButton& button,
    bool buttonAsHighlighted,
    bool buttonAsDown)
{
    if (isButtonInsideAlertWindow (button)) {
        drawAlertWindowButtonText (g, button, getTextButtonFont(), buttonAsHighlighted, buttonAsDown);
    } else {
        drawCommonButtonText (g, button, getTextButtonFont(), buttonAsHighlighted, buttonAsDown);
    }
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
// MARK: -

bool LNF::isButtonInsideAlertWindow (const juce::Button& button)
{
    juce::AlertWindow* w = dynamic_cast<juce::AlertWindow*> (button.getParentComponent());
    
    return (w != nullptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
