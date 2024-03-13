
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

RunComponent::RunComponent (const PatchBase& base) :
    RunFactoryHelper (this),
    BaseComponent (getIconsFactory(), Spaghettis()->getMenu(), Spaghettis()->getCommandManager()),
    runView_ (base),
    runPresets_ (runView_)
{
    CommandsHandler::addCloseWindowCommand (this);
    
    addAndMakeVisible (runView_);
    addChildComponent (runPresets_);
    
    addMenuCommand (MenuCommand (Commands::save)
        .setInvoke ([this] (const auto&) { runView_.getPatchRoot().save(); }));
        
    addMenuCommand (MenuCommand (Commands::inspector)
        .setInvoke ([this] (const auto&) { togglePresets(); })
        .setName   ([this]() { return getPresetsMenuText (runPresets_.isActive()); }));
    
    addMenuCommand (MenuCommand (Commands::newRunView)
        .setInvoke ([this] (const auto&) { runView_.getPatchRoot().openMainRunWindow(); }));
    
    addMenuCommand (MenuCommand (Commands::newEditView)
        .setInvoke ([this] (const auto&) { runView_.getPatchRoot().openMainEditWindow(); }));
    
    setButtonState (Icons::presets, runPresets_.isActive());
    
    setOpaque (true); setSize (600, 300);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PatchRoot& RunComponent::getPatchRoot()
{
    return runView_.getPatchRoot();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunComponent::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::windowsBackground));
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
    runPresets_.setActive (true);  updateLayout(); Spaghettis()->updateMenuBar();
}

void RunComponent::hidePresets()
{
    runPresets_.setActive (false); updateLayout(); Spaghettis()->updateMenuBar();
}

void RunComponent::togglePresets()
{
    if (toggleButtonState (Icons::presets)) { showPresets(); } else { hidePresets(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool RunComponent::tryGrabFocus()
{
    return tryGrabFocusForComponent (this);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunComponent::updateLayout()
{
    juce::Rectangle<int> bounds (setBoundsForBarsAndGetRemaining());
    
    if (runPresets_.isActive()) {
    //
    const int w = bounds.getWidth() - runPresets_.getWidth();
    
    runPresets_.setBounds (bounds.withTrimmedLeft (w));
    //
    }
    
    runPresets_.setVisible (runPresets_.isActive());
    
    runView_.setBounds (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
