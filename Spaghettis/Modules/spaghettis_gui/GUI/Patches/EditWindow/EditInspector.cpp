
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditInspector::EditInspector (EditView& view) :
    view_ (view),
    active_ (false),
    resizer_ (*this)
{
    setOpaque (true); setSize (resizer_.getDefaultWidth(), getHeight());
    
    view_.attach (this);
}

EditInspector::~EditInspector()
{
    view_.detach (this);
    
    hide();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::setActive (bool isActive)
{
    active_ = isActive; triggerAsyncUpdate();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
}

void EditInspector::resized()
{
    if (parameters_ != nullptr) { parameters_->resizePanel (getLocalBounds()); }
    
    resizer_.update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::show()
{
    if (parameters_ == nullptr) {
    //
    const int w = resizer_.getMinimumWidth();
    parameters_ = std::make_unique<InspectorView> (core::Item::createCopy (view_.getItemForInspector()), w);
    parameters_->resizePanel (getLocalBounds());
    addAndMakeVisible (parameters_->getPanel());
    //
    }
}

void EditInspector::hide()
{
    if (parameters_ != nullptr) {
    //
    removeChildComponent (&parameters_->getPanel());
    parameters_ = nullptr;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::handleAsyncUpdate()
{
    hide(); if (isActive()) { show(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
