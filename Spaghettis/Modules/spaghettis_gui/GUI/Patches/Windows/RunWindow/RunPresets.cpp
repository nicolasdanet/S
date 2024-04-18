
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
    
RunPresets::RunPresets (RunView& view) :
    view_ (view),
    active_ (view_.getPatchRoot().getPresetsManager().getTabState().value_or (false)),
    resizer_ (*this),
    presetsLoad_ (NEEDS_TRANS ("Load")),
    presetsStore_ (NEEDS_TRANS ("Store"))
{
    const int w = view_.getPatchRoot().getPresetsManager().getTabWidth().value_or (0);
    
    view_.attach (this);    /* Must be the first. */
    
    setDefaultWidth (w ? w : resizer_.getDefaultWidth());
    
    setOpaque (true);
    
    presetsLoad_.onClick  = [this] { load();  };
    presetsStore_.onClick = [this] { store(); };
    
    publish();
}

RunPresets::~RunPresets()
{
    hide();
    
    view_.detach (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool RunPresets::isActive() const
{
    return active_;
}
    
void RunPresets::setActive (bool isActive)
{
    active_ = isActive; notify(); update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::update()
{
    triggerAsyncUpdate();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::setDefaultWidth (int w)
{
    if (w >= resizer_.getMinimumWidth()) { setSize (w, getHeight()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::windowsBackground));
}

void RunPresets::resized()
{
    arrange();
    
    resizer_.update();
    
    if (isActive()) { notify(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::notify()
{
    view_.getPatchRoot().getPresetsManager().setTabState (isActive());
    view_.getPatchRoot().getPresetsManager().setTabWidth (getWidth());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::arrange()
{
    if (presetsView_ != nullptr) {
    //
    juce::Rectangle<int> presets (getLocalBounds());
    juce::Rectangle<int> buttons (presets.removeFromBottom (LNF::getButtonHeight()).reduced (1, 0));
    
    presetsLoad_.setBounds (buttons.removeFromLeft (buttons.getWidth() / 2).reduced (1, 0));
    presetsStore_.setBounds (buttons.reduced (1, 0));
    
    presetsView_->resizeConcertinaPanel (presets);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void RunPresets::show()
{
    if (presetsView_ == nullptr) {
    //
    const auto t = ParametersBase (Fonts::getFont(), Fonts::getMonospacedFont(), resizer_.getMinimumWidth());
    
    presetsElements_ = view_.getAllPresetElements();
    presetsView_     = std::make_unique<PresetsView> (presetsElements_, t);
    
    arrange();
    
    addAndMakeVisible (&presetsView_->getConcertinaPanel());
    addAndMakeVisible (&presetsLoad_);
    addAndMakeVisible (&presetsStore_);
    //
    }
}

void RunPresets::hide()
{
    if (presetsView_ != nullptr) {
    //
    removeChildComponent (&presetsStore_);
    removeChildComponent (&presetsLoad_);
    removeChildComponent (&presetsView_->getConcertinaPanel());
    
    presetsView_ = nullptr;
    presetsElements_.clear();
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void RunPresets::publish()
{
    hide(); if (isActive()) { show(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::handleAsyncUpdate()
{
    publish();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::load()
{
    view_.getPatchRoot().getPresetsManager().load (presetsElements_, juce::String());
}

void RunPresets::store()
{
    if (view_.getPatchRoot().getPresetsManager().store (presetsElements_, juce::String())) {
    //
    Broadcast::dirty (view_.getIdentifier());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
