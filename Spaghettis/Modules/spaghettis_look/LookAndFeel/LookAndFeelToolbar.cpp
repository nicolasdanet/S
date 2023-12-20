
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
  
void LNF::paintToolbarBackground (juce::Graphics& g, int w, int h, juce::Toolbar& toolbar)
{
    g.fillAll (Colours::fetchColour (Colours::toolbarBackground));
}

void LNF::paintToolbarButtonBackground (juce::Graphics& g,
    int width,
    int height,
    bool isMouseOver,
    bool isMouseDown,
    juce::ToolbarItemComponent& component)
{
    if (isMouseDown) {
        if (Icons::getInstance()->isToggle (component.getItemId()) == false) {
            g.fillAll (Colours::fetchColour (Colours::toolbarBackgroundDown));
        }
        
    } else if (isMouseOver) { g.fillAll (Colours::fetchColour (Colours::toolbarBackgroundOver)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
