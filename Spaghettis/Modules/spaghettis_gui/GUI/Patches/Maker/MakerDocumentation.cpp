
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
    g.fillAll (Colours::fetchColour (Colours::parametersBackground));
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
    const std::optional<data::Data> documentation (Spaghettis()->getDocumentation().get (s));
    
    removeChildComponent (shown_);
    
    if (documentation.has_value()) {
        const PropertyLookAndFeel lnf (Fonts::getFont(), Fonts::getMonospacedFont(), 150);
        fetched_ = std::make_unique<ParametersView> (documentation.value(), lnf);
        shown_   = &fetched_->getConcertinaPanel();
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
    const juce::String s (Strings::upToWhitespace (v.toString()));
    
    if (s != previous_) { showDocumentation (s); previous_ = s; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
