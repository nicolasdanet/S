
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
    active_ (view_.getPatch().get<bool> (Tag::Attributes, Tag::Inspector)),
    resizer_ (*this)
{
    const int w = view_.getPatch().get<int> (Tag::Attributes, Tag::InspectorWidth);
    
    view_.attach (this);    /* Must be the first. */
    
    setDefaultWidth (w ? w : resizer_.getDefaultWidth());
    
    setOpaque (true);
    
    publish();
}

EditInspector::~EditInspector()
{
    hide();
    
    view_.detach (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::setDefaultWidth (int w)
{
    if (w >= resizer_.getMinimumWidth()) { setSize (w, getHeight()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditInspector::isActive() const
{
    return active_;
}
    
void EditInspector::setActive (bool isActive)
{
    active_ = isActive; notify(); update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::update()
{
    triggerAsyncUpdate();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::parametersBackground));
}

void EditInspector::resized()
{
    if (parameters_ != nullptr) {
        parameters_->resizePanel (getLocalBounds());
    }
    
    resizer_.update();
    
    if (isActive()) { notify(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::notify()
{
    view_.getPort()->updateInspector();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::show()
{
    if (parameters_ == nullptr) {
    //
    const int w = resizer_.getMinimumWidth();
    parameters_ = std::make_unique<InspectorView> (view_.getSynchronized(), w);
    parameters_->resizePanel (getLocalBounds());
    addAndMakeVisible (&parameters_->getConcertinaPanel());
    //
    }
}

void EditInspector::hide()
{
    if (parameters_ != nullptr) {
    //
    removeChildComponent (&parameters_->getConcertinaPanel());
    parameters_ = nullptr;
    //
    }
}

void EditInspector::publish()
{
    hide(); if (isActive()) { show(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditInspector::handleAsyncUpdate()
{
    publish();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
