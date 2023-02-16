
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MakerDocumentation::MakerDocumentation (juce::Value& v) : v_ (v)
{
    v_.addListener (this);
    
    setOpaque (true);
}
    
MakerDocumentation::~MakerDocumentation()
{
    hideDocumentation();
    
    v_.removeListener (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerDocumentation::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::parametersBackground));
}

void MakerDocumentation::resized()
{
    if (documentation_ != nullptr) { documentation_->resizePanel (getLocalBounds()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerDocumentation::showDocumentation (const juce::String& s)
{
    hideDocumentation();
    
    if (Documentation::has (s)) {
    //
    jassert (documentation_ == nullptr);
    
    documentation_ = std::make_unique<ParameterView> (Documentation::get (s), 150);
    documentation_->resizePanel (getLocalBounds());
    addAndMakeVisible (&documentation_->getPanel());
    //
    }
}
    
void MakerDocumentation::hideDocumentation()
{
    if (documentation_ != nullptr) {
    //
    removeChildComponent (&documentation_->getPanel());
    documentation_ = nullptr;
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerDocumentation::valueChanged (juce::Value& v)
{
    const juce::String s (Helpers::upToWhitespace (v.toString()));
    
    if (s != previous_) { showDocumentation (s); previous_ = s; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
