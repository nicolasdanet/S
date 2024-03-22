
/* Copyright (c) 2022 Jojo and others. */

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
    
    setDefaultWidth (w ? w : resizer_.getDefaultWidth());
    
    setOpaque (true);
    
    publish();
}

RunPresets::~RunPresets()
{
    hide();
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
    /*
    if (presets_ != nullptr) {
        presets_->resizeConcertinaPanel (getLocalBounds());
    }
    */
    
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
    if (presets_ == nullptr) {
    //
    const int w = resizer_.getMinimumWidth();
    presets_ = std::make_unique<PresetsView> (w);
    // presets_->resizeConcertinaPanel (getLocalBounds());
    // addAndMakeVisible (&presets_->getConcertinaPanel());
    //
    }
}

void RunPresets::hide()
{
    if (presets_ != nullptr) {
    //
    // removeChildComponent (&presets_->getConcertinaPanel());
    presets_ = nullptr;
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
