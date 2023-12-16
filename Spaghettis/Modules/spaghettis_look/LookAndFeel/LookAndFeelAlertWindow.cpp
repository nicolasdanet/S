
/* Copyright (c) 2020 Jojo and others. */

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

juce::KeyPress getShortcut (const juce::String& button)
{
    return juce::KeyPress (static_cast<int> (juce::CharacterFunctions::toLowerCase (button[0])));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void createAlertWindowAddButtons (const std::unique_ptr<juce::AlertWindow>& w, const juce::String& button1)
{
    w->addButton (button1,
        0,
        juce::KeyPress (juce::KeyPress::escapeKey),
        juce::KeyPress (juce::KeyPress::returnKey));
}

void createAlertWindowAddButtons (const std::unique_ptr<juce::AlertWindow>& w,
    const juce::String& button1,
    const juce::String& button2)
{
    const juce::KeyPress shortCut1 (getShortcut (button1));
    const juce::KeyPress shortCut2 (getShortcut (button2));
    
    jassert (shortCut1 != shortCut2);
    
    w->addButton (button1, 1, juce::KeyPress (juce::KeyPress::returnKey), shortCut1);
    w->addButton (button2, 0, juce::KeyPress (juce::KeyPress::escapeKey), shortCut2);
}

void createAlertWindowAddButtons (const std::unique_ptr<juce::AlertWindow>& w,
    const juce::String& button1,
    const juce::String& button2,
    const juce::String& button3)
{
    const juce::KeyPress shortCut1 (getShortcut (button1));
    const juce::KeyPress shortCut2 (getShortcut (button2));
    
    jassert (shortCut1 != shortCut2);
    
    w->addButton (button1, 1, juce::KeyPress (juce::KeyPress::returnKey), shortCut1);
    w->addButton (button2, 2, shortCut2);
    w->addButton (button3, 0, juce::KeyPress (juce::KeyPress::escapeKey));
}

void createAlertWindowAddButtons (const std::unique_ptr<juce::AlertWindow>& w,
    const juce::String& button1,
    const juce::String& button2,
    const juce::String& button3,
    int numberOfButtons)
{
    switch (numberOfButtons) {
        case 1  : createAlertWindowAddButtons (w, button1); break;
        case 2  : createAlertWindowAddButtons (w, button1, button2); break;
        case 3  : createAlertWindowAddButtons (w, button1, button2, button3); break;
        default : break;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void createAlertWindowSetBounds (const std::unique_ptr<juce::AlertWindow>& w)
{
    w->setBounds (w->getBounds().expanded (20));

    for (auto* child : w->getChildren()) {
        if (juce::TextButton* button = dynamic_cast<juce::TextButton*> (child)) {
            button->setBounds (button->getBounds().translated (20, 20));
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::AlertWindow* LNF::createAlertWindow (const juce::String& title,
    const juce::String& message,
    const juce::String& button1,
    const juce::String& button2,
    const juce::String& button3,
    juce::MessageBoxIconType iconType,
    int numberOfButtons,
    juce::Component* associatedComponent)
{
    #if ! ( JUCE_LINUX )
        jassertfalse;           /* Others OS use native alert windows. */
    #endif
    
    auto w = std::make_unique<juce::AlertWindow> (title, message, iconType, associatedComponent);
        
    createAlertWindowAddButtons (w, button1, button2, button3, numberOfButtons);
    createAlertWindowSetBounds (w);
    
    if (associatedComponent) {
        AlertWindowRegister::getInstance().add (associatedComponent, w.get());
    }
    
    w->setAlwaysOnTop (true);
    
    return w.release();
}

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
            ? Colours::fetchColour (Colours::alertWindowButtonBackgroundOver)
            : Colours::fetchColour (Colours::alertWindowButtonBackground);
            
    g.setColour (background);
    g.fillRoundedRectangle (bounds, cornerSize);
    g.setColour (Colours::fetchColour (Colours::alertWindowButtonOutline));
    g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
}

void LNF::drawButtonText (juce::Graphics& g, juce::TextButton& button, bool, bool)
{
    const juce::Rectangle<int> r (button.getWidth(), button.getHeight());
    
    const bool hightlighted   = button.isRegisteredForShortcut (juce::KeyPress (juce::KeyPress::returnKey));
    const juce::Colour colour = (Colours::fetchColour (hightlighted
                                    ? Colours::alertWindowButtonTextHighlighted
                                    : Colours::alertWindowButtonText));
        
    g.setFont (getTextButtonFont());
    g.setColour (colour);
    g.drawText (button.getButtonText(), r.reduced (10, 5), juce::Justification::centred, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void drawAlertBoxIconPathSetContent (const juce::Rectangle<float>& r, const juce::String& s, juce::Path& path)
{
    const juce::Font f (r.getHeight() * 0.9f, juce::Font::bold);
    
    juce::GlyphArrangement g;
    
    g.addFittedText (f, s, r.getX(), r.getY(), r.getWidth(), r.getHeight(), juce::Justification::centred, 0);
    g.createPath (path);
}

juce::Path drawAlertBoxIconPath (const juce::Rectangle<float>& r, juce::MessageBoxIconType type)
{
    const juce::String content = [type]()
        {
            if (type == juce::MessageBoxIconType::WarningIcon)   { return juce::String ("!"); }
            else if (type == juce::MessageBoxIconType::InfoIcon) { return juce::String ("i"); }
            else {
                return juce::String ("?");
            }
        }();
        
    juce::Path path;
    
    path.addEllipse (r);
    
    drawAlertBoxIconPathSetContent (r, content, path);
    
    path.setUsingNonZeroWinding (false);
    
    return path;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawAlertBoxBackground (juce::Graphics& g,
    juce::Rectangle<int> bounds,
    float cornerSize)
{
    g.setColour (Colours::fetchColour (Colours::alertWindowBackground));
    g.fillRoundedRectangle (bounds.toFloat(), cornerSize);
    g.setColour (Colours::fetchColour (Colours::alertWindowOutline));
    g.drawRoundedRectangle (bounds.expanded (1).toFloat(), cornerSize, 2.0f);
}

void LNF::drawAlertBoxIcon (juce::Graphics& g,
    juce::Rectangle<int> iconArea,
    juce::AlertWindow& alert)
{
    g.setColour (Colours::fetchColour (Colours::alertWindowIcon));
    g.fillPath (drawAlertBoxIconPath (iconArea.toFloat(), alert.getAlertType()));
}

void LNF::drawAlertBoxText (juce::Graphics& g,
    juce::Rectangle<int> bounds,
    const juce::TextLayout& textLayout)
{
    g.setColour (Colours::fetchColour (Colours::alertWindowText));

    textLayout.draw (g, bounds.toFloat());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void LNF::drawAlertBox (juce::Graphics& g,
    juce::AlertWindow& alert,
    const juce::Rectangle<int>& textArea,
    juce::TextLayout& textLayout)
{
    const juce::Rectangle<int> bounds   = alert.getLocalBounds().reduced (1);
    const juce::Rectangle<int> iconArea = juce::Rectangle<int> (-12, -12, 120, 120);
    const float cornerSize              = 4.0f;
    const int h                         = getAlertWindowButtonHeight();
    
    drawAlertBoxBackground (g, bounds, cornerSize);
    
    g.reduceClipRegion (bounds);
    
    drawAlertBoxIcon (g, iconArea, alert);
    drawAlertBoxText (g, bounds.reduced (0, 25).withTrimmedLeft (80).withTrimmedBottom (h), textLayout);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int LNF::getAlertWindowButtonHeight()
{
    return static_cast<int> (Fonts::getMenuFont().getHeight() * 1.75);
}

juce::Font LNF::getAlertWindowTitleFont()
{
    return Fonts::getMenuFont();
}

juce::Font LNF::getAlertWindowMessageFont()
{
    return Fonts::getTooltipsFont();
}

juce::Font LNF::getAlertWindowFont()
{
    return Fonts::getMenuFont();
}

juce::Font LNF::getTextButtonFont()
{
    return Fonts::getMenuFont();
}

juce::Font LNF::getTextButtonFont (juce::TextButton&, int)
{
    return getTextButtonFont();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
