
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int LNF::getDefaultMenuBarHeight()
{
    return 28;
}

juce::Font LNF::getMenuBarFont (juce::MenuBarComponent&, int, const juce::String&)
{
    return getMenuFont();
}

void LNF::drawMenuBarBackground (juce::Graphics& g, int, int, bool, juce::MenuBarComponent&)
{
    g.fillAll (Colours::fetchColour (Colours::menubarBackground));
}
    
void LNF::drawMenuBarItem (juce::Graphics& g,
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
        
    if (highlighted) { g.fillAll (Colours::fetchColour (Colours::menubarPopupBackgroundHighlighted)); }

    g.setColour (Colours::fetchColour (Colours::menubarText));
    g.setFont (getMenuBarFont (m, index, text));
    g.drawText (text, 0, 0, width, height, juce::Justification::centred, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
