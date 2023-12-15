
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font LNF::getPopupMenuFont()
{
    return getMenuFont();
}

int LNF::getPopupMenuBorderSize()
{
    return 0;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::drawPopupMenuBackgroundProceed (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::menubarPopupBackground));
}
    
void LNF::drawPopupMenuBackgroundWithOptions (juce::Graphics& g,
    int width,
    int height,
    const juce::PopupMenu::Options& options)
{
    drawPopupMenuBackgroundProceed (g);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LNF::getIdealPopupMenuItemSize (const juce::String& text,
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

void LNF::drawPopupMenuItemSelector (juce::Graphics& g, const juce::Rectangle<int>& area)
{
    g.setColour (Colours::fetchColour (Colours::menubarPopupSeparator).withAlpha (0.25f)); g.fillRect (area);
}

void LNF::drawPopupMenuItemBackground (juce::Graphics& g, const juce::Rectangle<int>& area)
{
    g.setColour (Colours::fetchColour (Colours::menubarPopupBackgroundHighlighted));
    g.fillRect (area);
}

void LNF::drawPopupMenuItemTick (juce::Graphics& g, juce::Rectangle<int>& t)
{
    LNF::drawTick (g, t.reduced (5));
}

void LNF::drawPopupMenuItemSubMenu (juce::Graphics& g, juce::Rectangle<int>& r)
{
    const int h = r.getHeight();
    
    LNF::drawArrowRight (g, r.removeFromRight (h / 2).reduced (0, h / 4));
}

void LNF::drawPopupMenuItemShortcut (juce::Graphics& g,
    const juce::Rectangle<int>& r,
    const juce::String& shortcutText)
{
    /* Keep only the first if multiple shortcuts are defined. */
    
    juce::String parsed (shortcutText.upToFirstOccurrenceOf (",", false, true));
    
    g.setFont (getMenuShortcutsFont());
    
    g.drawText (parsed, r, juce::Justification::centredRight, true);
}

void LNF::drawPopupMenuItemProceed (juce::Graphics& g,
    const juce::Rectangle<int>& area,
    const bool isSeparator,
    const bool isActive,
    const bool isHighlighted,
    const bool isTicked,
    const bool hasSubMenu,
    const bool isComboBox,
    const juce::String& text,
    const juce::String& shortcutText)
{
    if (isSeparator) { drawPopupMenuItemSelector (g, area); }
    else {
    //
    if (isHighlighted && isActive) { drawPopupMenuItemBackground (g, area); }
    
    const Colours::ColourIds n = isHighlighted ?
                                    Colours::menubarPopupTextHighlighted :
                                    Colours::menubarPopupText;
    const juce::Colour c1 = Colours::fetchColour (n);
    const juce::Colour c2 = c1.withMultipliedAlpha (0.5f);
    
    g.setColour (isActive ? c1 : c2);
    
    const int border = juce::jmin (5, area.getWidth() / 20);
    
    juce::Rectangle<int> r = area.reduced (border, 0);
    juce::Rectangle<int> t = r.removeFromLeft (r.getHeight() * 0.75);
    
    if (isTicked) { if (!isComboBox) { drawPopupMenuItemTick (g, t); } }
    else if (hasSubMenu) {
    //
    drawPopupMenuItemSubMenu (g, r);
    //
    }
    
    r.removeFromRight (3);
    g.setFont (getPopupMenuFont());
    g.drawText (text, r, juce::Justification::centredLeft, true);

    if (shortcutText.isNotEmpty()) { g.setColour (c2); drawPopupMenuItemShortcut (g, r, shortcutText); }
    //
    }
}

void LNF::drawPopupMenuItemWithOptions (juce::Graphics& g,
    const juce::Rectangle<int>& area,
    bool  isHighlighted,
    const juce::PopupMenu::Item& i,
    const juce::PopupMenu::Options& options)
{
    const bool hasSubMenu = (i.subMenu != nullptr) && (i.itemID == 0 || i.subMenu->getNumItems() > 0);
    const bool isComboBox = (dynamic_cast<juce::ComboBox*> (options.getTargetComponent()) != nullptr);
    
    jassert (!hasSubMenu || !isComboBox);
    
    drawPopupMenuItemProceed (g,
        area,
        i.isSeparator,
        i.isEnabled,
        isHighlighted,
        i.isTicked,
        hasSubMenu,
        isComboBox,
        i.text,
        i.shortcutKeyDescription);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
