
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
    active_ (view_.getPatchRoot().getPresets().getTabState().value_or (false)),
    resizer_ (*this)
{
    const int w = view_.getPatchRoot().getPresets().getTabWidth().value_or (0);
    
    view_.attach (this);    /* Must be the first. */
    
    setDefaultWidth (w ? w : resizer_.getDefaultWidth());
    
    setOpaque (true);
    
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
    if (presetsView_ != nullptr) {
        presetsView_->resizeConcertinaPanel (getLocalBounds());
    }
    
    resizer_.update();
    
    if (isActive()) { notify(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::notify()
{
    view_.getPatchRoot().getPresets().setTabState (isActive());
    view_.getPatchRoot().getPresets().setTabWidth (getWidth());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RunPresets::show()
{
    if (presetsView_ == nullptr) {
    //
    const auto t = ParametersBase (Fonts::getFont(), Fonts::getMonospacedFont(), resizer_.getMinimumWidth());
    
    presetsElements_ = view_.getAllPresetElements();
    presetsView_     = std::make_unique<PresetsView> (presetsElements_, t);
    presetsView_->resizeConcertinaPanel (getLocalBounds());
    addAndMakeVisible (&presetsView_->getConcertinaPanel());
    //
    }
}

void RunPresets::hide()
{
    if (presetsView_ != nullptr) {
    //
    removeChildComponent (&presetsView_->getConcertinaPanel());
    presetsView_ = nullptr;
    presetsElements_.clear();
    //
    }
}

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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
