
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

RunComponent::RunComponent (Patch& patch, const juce::ValueTree& tree) :
    RunFactoryHelper (this),
    BaseComponent (getIconsFactory()),
    runView_ (patch, tree),
    hasPresets_ (false)
{
    addAndMakeVisible (runView_);
    addChildComponent (runPresets_);
    
    addMenuBarCommand (Commands::save,          [this]() { runView_.getPatch().save(); });
    addMenuBarCommand (Commands::closeWindow,   [this]() { BaseWindow::getWindow (this)->close(); });
    
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
    hasPresets_ = true;  updateLayout();
}

void RunComponent::hidePresets()
{
    hasPresets_ = false; updateLayout();
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
