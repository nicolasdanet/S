
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawMenuBarItem (juce::Graphics& g,
    int width,
    int height,
    int index,
    const juce::String& text,
    bool isMouseOverItem,
    bool isMenuOpen,
    bool,
    juce::MenuBarComponent& m)
{
    const bool highlighted = (isMenuOpen || isMouseOverItem) && m.isEnabled();
        
    if (highlighted) { g.fillAll (findColour (Colours::menubarBackgroundHighlighted)); }

    g.setColour (findColour (Colours::menubarText));
    g.setFont (getMenuBarFont (m, index, text));
    g.drawFittedText (text, 0, 0, width, height, juce::Justification::centred, 1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::getIdealPopupMenuItemSize (const juce::String& text,
    const bool isSeparator,
    int,
    int& w,
    int& h)
{
    if (isSeparator) { w = 50; h = 2; }
    else {
        juce::Font font = getPopupMenuFont();
        h = static_cast<int>(font.getHeight() * 1.6);
        w = static_cast<int>(font.getStringWidth (text) + h);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LookAndFeel::drawPopupMenuItemSelector (juce::Graphics& g, const juce::Rectangle<int>& area)
{
    g.setColour (findColour (Colours::menubarSeparator).withAlpha (0.25f)); g.fillRect (area);
}

void LookAndFeel::drawPopupMenuItemBackground (juce::Graphics& g, const juce::Rectangle<int>& area)
{
    g.setColour (findColour (Colours::menubarBackgroundHighlighted)); g.fillRect (area);
}

void LookAndFeel::drawPopupMenuItemTick (juce::Graphics& g, juce::Rectangle<int> t)
{
    juce::Path path = getTickShape (1.0f);
    g.fillPath (path, path.getTransformToScaleToFit (t.reduced (5).toFloat(), true));
}

void LookAndFeel::drawPopupMenuItemSubMenu (juce::Graphics& g, juce::Rectangle<int>& r)
{
    int w = static_cast<int> (0.6f * getPopupMenuFont().getAscent());
    
    juce::Rectangle<int> t = r.removeFromRight (w);
    
    float x = static_cast<float> (t.getX());
    float y = static_cast<float> (t.getCentreY());
    float hArrow = w;
    float wArrow = w * 0.6f;
    
    juce::Path path;
    path.startNewSubPath (x, y - (hArrow * 0.5f));
    path.lineTo (x + wArrow, y);
    path.lineTo (x, y + (hArrow * 0.5f));

    g.strokePath (path, juce::PathStrokeType (2.0f));
}

void LookAndFeel::drawPopupMenuItem (juce::Graphics& g,
    const juce::Rectangle<int>& area,
    const bool isSeparator,
    const bool isActive,
    const bool isHighlighted,
    const bool isTicked,
    const bool hasSubMenu,
    const juce::String& text,
    const juce::String& shortcutKeyText,
    const juce::Drawable*,
    const juce::Colour* const)
{
    if (isSeparator) { drawPopupMenuItemSelector (g, area); }
    else {
    //
    if (isHighlighted && isActive) { drawPopupMenuItemBackground (g, area); }
    
    juce::Colour c   = findColour (Colours::menubarText).withMultipliedAlpha (isActive ? 1.0f : 0.5f);
    const int border = juce::jmin (5, area.getWidth() / 20);
    
    g.setColour (c);
    
    juce::Rectangle<int> r = area.reduced (border, 0);
    juce::Rectangle<int> t = r.removeFromLeft (r.getHeight() * 0.75);
    
    if (isTicked) { drawPopupMenuItemTick (g, std::move (t)); }
    else if (hasSubMenu) {
        drawPopupMenuItemSubMenu (g, r);
    }

    g.setColour (c); g.setFont (getPopupMenuFont());
    
    r.removeFromRight (3);
    g.drawFittedText (text, r, juce::Justification::centredLeft, 1);

    if (shortcutKeyText.isNotEmpty())
    {
        juce::Font f2 = getPopupMenuFont();
        f2.setHeight (f2.getHeight() * 0.75f);
        f2.setHorizontalScale (0.95f);
        g.setFont (f2);

        g.drawText (shortcutKeyText, r, juce::Justification::centredRight, true);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
