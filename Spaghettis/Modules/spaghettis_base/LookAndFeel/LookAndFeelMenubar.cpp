
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int LNF::getDefaultMenuBarHeight()
{
    return WindowsProperties::getMenuBarHeight();
}

juce::Font LNF::getMenuBarFont (juce::MenuBarComponent&, int, const juce::String&)
{
    return Fonts::getMenuFont();
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
