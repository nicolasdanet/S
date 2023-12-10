
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int LookAndFeel::getToolbarHeight()
{
    #if JUCE_LINUX
        return 30;
    #else
        return 32;
    #endif
}
    
void LookAndFeel::paintToolbarBackground (juce::Graphics& g, int w, int h, juce::Toolbar& toolbar)
{
    g.fillAll (findColour (Colours::toolbarBackground));
}

void LookAndFeel::paintToolbarButtonBackground (juce::Graphics& g,
    int width,
    int height,
    bool isMouseOver,
    bool isMouseDown,
    juce::ToolbarItemComponent& component)
{
    if (isMouseDown) {
        if (Icons::getInstance().isToggle (component.getItemId()) == false) {
            g.fillAll (findColour (Colours::toolbarBackgroundDown));
        }
        
    } else if (isMouseOver) { g.fillAll (findColour (Colours::toolbarBackgroundOver)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
