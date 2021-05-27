
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String LookAndFeel::initializeFontsBest (const juce::StringArray& check)
{
    if (check.isEmpty() == false) {
    //
    juce::StringArray fonts (juce::Font::findAllTypefaceNames());
    
    for (const auto& f : check) { if (fonts.contains (f)) { return f; } }
    //
    }
    
    return juce::String();
}

void LookAndFeel::initializeFonts()
{
    #if JUCE_LINUX
    
    juce::String fontName (initializeFontsBest (juce::StringArray ("Ubuntu")));
    juce::String fontMonospacedName (initializeFontsBest (juce::StringArray ("Ubuntu Mono")));
    
    #else
    
    juce::String fontName (initializeFontsBest (juce::StringArray()));
    juce::String fontMonospacedName (initializeFontsBest (juce::StringArray ()));
    
    #endif
    
    if (fontName.isEmpty())           { fontName           = juce::Font::getDefaultSansSerifFontName();  }
    if (fontMonospacedName.isEmpty()) { fontMonospacedName = juce::Font::getDefaultMonospacedFontName(); }
    
    fontConsole_   = juce::Font (fontName, 18.0, juce::Font::plain);
    fontShortcuts_ = juce::Font (fontMonospacedName, 18.0, juce::Font::plain);
    
    setDefaultSansSerifTypefaceName (fontName);
}

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
    if (isSeparator) { w = 75; h = 2; }
    else {
        const juce::Font font = getPopupMenuFont();
        const float f = font.getHeight();
        #if JUCE_LINUX
        const float extra = (f * 5.0);      /* Extra space for shortcuts (Ubuntu monospaced is larger). */
        #else
        const float extra = (f * 2.5);
        #endif
        h = static_cast<int> (f * 1.6);
        w = static_cast<int> (font.getStringWidth (text) + extra);
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
    const juce::Path path = getTickShape (1.0f);
    g.fillPath (path, path.getTransformToScaleToFit (t.reduced (5).toFloat(), true));
}

void LookAndFeel::drawPopupMenuItemSubMenu (juce::Graphics& g, juce::Rectangle<int>& r)
{
    const int w = static_cast<int> (0.6f * getPopupMenuFont().getAscent());
    
    const juce::Rectangle<int> t = r.removeFromRight (w);
    
    const float x = static_cast<float> (t.getX());
    const float y = static_cast<float> (t.getCentreY());
    const float hArrow = w;
    const float wArrow = w * 0.6f;
    
    juce::Path path;
    path.startNewSubPath (x, y - (hArrow * 0.5f));
    path.lineTo (x + wArrow, y);
    path.lineTo (x, y + (hArrow * 0.5f));

    g.strokePath (path, juce::PathStrokeType (2.0f));
}

void LookAndFeel::drawPopupMenuItemShortcut (juce::Graphics& g,
    const juce::Rectangle<int>& r,
    const juce::String& shortcutText)
{
    g.setFont (getFontShortcuts());
    g.drawText (shortcutText, r, juce::Justification::centredRight, true);
}

void LookAndFeel::drawPopupMenuItem (juce::Graphics& g,
    const juce::Rectangle<int>& area,
    const bool isSeparator,
    const bool isActive,
    const bool isHighlighted,
    const bool isTicked,
    const bool hasSubMenu,
    const juce::String& text,
    const juce::String& shortcutText,
    const juce::Drawable*,
    const juce::Colour* const)
{
    if (isSeparator) { drawPopupMenuItemSelector (g, area); }
    else {
    //
    if (isHighlighted && isActive) { drawPopupMenuItemBackground (g, area); }
    
    const juce::Colour c1 = findColour (Colours::menubarText);
    const juce::Colour c2 = c1.withMultipliedAlpha (0.5f);
    
    g.setColour (isActive ? c1 : c2);
    
    const int border = juce::jmin (5, area.getWidth() / 20);
    
    juce::Rectangle<int> r = area.reduced (border, 0);
    juce::Rectangle<int> t = r.removeFromLeft (r.getHeight() * 0.75);
    
    if (isTicked) { drawPopupMenuItemTick (g, std::move (t)); }
    else if (hasSubMenu) {
    //
    drawPopupMenuItemSubMenu (g, r);
    //
    }
    
    r.removeFromRight (3);
    g.setFont (getPopupMenuFont());
    g.drawFittedText (text, r, juce::Justification::centredLeft, 1);

    if (shortcutText.isNotEmpty()) { g.setColour (c2); drawPopupMenuItemShortcut (g, r, shortcutText); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
