
/* Copyright (c) 2020 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::AlertWindow* LookAndFeel::createAlertWindow (const juce::String& title,
    const juce::String& message,
    const juce::String& button1,
    const juce::String& button2,
    const juce::String& button3,
    juce::MessageBoxIconType iconType,
    int numButtons,
    juce::Component* associatedComponent)
{
    auto boundsOffset = 50;

    auto* aw = LookAndFeel_V2::createAlertWindow (title, message, button1, button2, button3,
                                                  iconType, numButtons, associatedComponent);

    auto bounds = aw->getBounds();
    bounds = bounds.withSizeKeepingCentre (bounds.getWidth() + boundsOffset, bounds.getHeight() + boundsOffset);
    aw->setBounds (bounds);

    for (auto* child : aw->getChildren())
        if (auto* button = dynamic_cast<juce::TextButton*> (child))
            button->setBounds (button->getBounds() + juce::Point<int> (25, 40));

    return aw;
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

void LookAndFeel::drawAlertBoxBackground (juce::Graphics& g,
    juce::Rectangle<int> bounds,
    float cornerSize)
{
    g.setColour (findColour (Colours::alertWindowBackground));
    g.fillRoundedRectangle (bounds.toFloat(), cornerSize);
    g.setColour (findColour (Colours::alertWindowOutline));
    g.drawRoundedRectangle (bounds.expanded (1).toFloat(), cornerSize, 2.0f);
}

void LookAndFeel::drawAlertBoxIcon (juce::Graphics& g,
    juce::Rectangle<int> iconArea,
    juce::AlertWindow& alert)
{
    g.setColour (juce::Colour (findColour (Colours::alertWindowIcon)));
    g.fillPath (drawAlertBoxIconPath (iconArea.toFloat(), alert.getAlertType()));
}

void LookAndFeel::drawAlertBoxText (juce::Graphics& g,
    juce::Rectangle<int> bounds,
    const juce::TextLayout& textLayout)
{
    g.setColour (findColour (Colours::alertWindowText));

    textLayout.draw (g, bounds.toFloat());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void LookAndFeel::drawAlertBox (juce::Graphics& g,
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
    drawAlertBoxText (g, bounds.reduced (0, 30).withTrimmedLeft (80).withTrimmedBottom (h), textLayout);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int LookAndFeel::getAlertWindowButtonHeight()
{
    return static_cast<int> (getMenuFont().getHeight() * 1.75);
}

juce::Font LookAndFeel::getAlertWindowTitleFont()
{
    return getMenuFont();
}

juce::Font LookAndFeel::getAlertWindowMessageFont()
{
    return getTooltipsFont();
}

juce::Font LookAndFeel::getAlertWindowFont()
{
    return getMenuFont();
}

juce::Font LookAndFeel::getTextButtonFont (juce::TextButton&, int)
{
    return getMenuFont();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
