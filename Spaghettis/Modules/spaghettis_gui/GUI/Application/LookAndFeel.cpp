
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

void LookAndFeel::drawPopupMenuItemSelector (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    g.setColour (findColour (Colours::menubarSeparator).withAlpha (0.25f));
    g.fillRect (r);
}

void LookAndFeel::drawPopupMenuItemBackground (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    g.setColour (findColour (Colours::menubarBackgroundHighlighted));
    g.fillRect (r);
}

void LookAndFeel::drawPopupMenuItemTick (juce::Graphics& g, const juce::Rectangle<int>& r)
{
    juce::Path tick = getTickShape (1.0f);
    g.fillPath (tick, tick.getTransformToScaleToFit (r.reduced (r.getWidth() / 5, 0).toFloat(), true));
}

void LookAndFeel::drawPopupMenuItemSubMenu (juce::Graphics& g, juce::Rectangle<int>& r)
{
    int arrowH  = static_cast<int> (0.6f * getPopupMenuFont().getAscent());
    float x     = static_cast<float> (r.removeFromRight (arrowH).getX());
    float halfH = static_cast<float> (r.getCentreY());

    juce::Path path;
    path.startNewSubPath (x, halfH - arrowH * 0.5f);
    path.lineTo (x + arrowH * 0.6f, halfH);
    path.lineTo (x, halfH + arrowH * 0.5f);

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
    
    g.setColour (findColour (Colours::menubarText).withMultipliedAlpha (isActive ? 1.0f : 0.5f));

    const int border = juce::jmin (5, area.getWidth() / 20);
    
    juce::Rectangle<int> r = area.reduced (border, 0);
    juce::Rectangle<int> t = r.removeFromLeft (r.getHeight() * 0.75);
    
    if (isTicked)   { drawPopupMenuItemTick (g, t); }
    if (hasSubMenu) { drawPopupMenuItemSubMenu (g, r); }

    g.setFont (getPopupMenuFont());
    
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
