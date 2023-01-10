
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String getPresetsMenuText (bool hasPresets)
{
    return hasPresets ? juce::String ("Hide Presets") : juce::String ("Show Presets");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

RunComponent::RunComponent (PatchRoot& patch, const juce::ValueTree& tree) :
    RunFactoryHelper (this),
    BaseComponent (getIconsFactory()),
    runView_ (patch, tree),
    hasPresets_ (false)
{
    addAndMakeVisible (runView_);
    addChildComponent (runPresets_);
    
    addMenuCommand (MenuCommand (Commands::save,        [this]() { runView_.getPatchRoot().save(); }));
    addMenuCommand (MenuCommand (Commands::inspector,   [this]() { togglePresets(); },
                                                        []() { return true; },
                                                        [this]()
                                                        {
                                                            return getPresetsMenuText (hasPresets_);
                                                        }
                                                        ));
                                                            
    setOpaque (true); setSize (600, 300);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunComponent::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
}

void RunComponent::resized()
{
    updateLayout();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunComponent::showPresets()
{
    hasPresets_ = true;  updateLayout(); Spaghettis()->updateMenuBar();
}

void RunComponent::hidePresets()
{
    hasPresets_ = false; updateLayout(); Spaghettis()->updateMenuBar();
}

void RunComponent::togglePresets()
{
    if (toggleButtonState (Icons::presets)) { showPresets(); } else { hidePresets(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunComponent::updateLayout()
{
    juce::Rectangle<int> bounds (setBoundsForBarsAndGetRemaining());
    
    if (hasPresets_) {
    //
    const int w = bounds.getWidth() - runPresets_.getWidth();
    
    runPresets_.setBounds (bounds.withTrimmedLeft (w));
    //
    }
    
    runPresets_.setVisible (hasPresets_);
    
    runView_.setBounds (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
