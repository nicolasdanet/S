
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MakerDocumentation::MakerDocumentation (const juce::Value& v) : v_ (v), shown_ (&default_)
{
    v_.addListener (this);
    
    setOpaque (true);
    
    addAndMakeVisible (shown_);
}
    
MakerDocumentation::~MakerDocumentation()
{
    removeChildComponent (shown_);
    
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
    shown_->setBounds (getLocalBounds());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MakerDocumentation::showDocumentation (const juce::String& s)
{
    removeChildComponent (shown_);
    
    if (Documentation::has (s)) {
        fetched_ = std::make_unique<ParameterView> (Documentation::get (s), 150);
        shown_   = &fetched_->getPanel();
    } else {
        fetched_ = nullptr;
        shown_   = &default_;
    }
    
    resized(); addAndMakeVisible (shown_);
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
