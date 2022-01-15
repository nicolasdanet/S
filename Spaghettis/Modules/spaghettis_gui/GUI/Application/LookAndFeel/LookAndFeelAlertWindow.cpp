
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
    juce::AlertWindow* w = LookAndFeel_V4::createAlertWindow (title,
        message,
        button1,
        button2,
        button3,
        iconType,
        numButtons,
        nullptr);

    return w;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawButtonBackground (juce::Graphics& g,
    juce::Button& button,
    const juce::Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    auto cornerSize = 6.0f;
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

    auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                      .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);

    // g.setColour (baseColour);
    
    g.setColour (juce::Colours::orange);

    auto flatOnLeft   = button.isConnectedOnLeft();
    auto flatOnRight  = button.isConnectedOnRight();
    auto flatOnTop    = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
    {
        juce::Path path;
        path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                                  bounds.getWidth(), bounds.getHeight(),
                                  cornerSize, cornerSize,
                                  ! (flatOnLeft  || flatOnTop),
                                  ! (flatOnRight || flatOnTop),
                                  ! (flatOnLeft  || flatOnBottom),
                                  ! (flatOnRight || flatOnBottom));

        g.fillPath (path);

        // g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.setColour (juce::Colours::red);
        
        g.strokePath (path, juce::PathStrokeType (1.0f));
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);

        // g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.setColour (juce::Colours::orange);
        g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
    }
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
