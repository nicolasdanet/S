
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ChoicesSelector::ChoicesSelector (const juce::Value& v, const juce::StringArray& choices) : value_ (v)
{
    for (const auto& s : choices)  { buttons_.push_back (std::make_unique<juce::ToggleButton> (s)); }
    
    const bool enabled = isEnabled();
    
    int index = 0;
    
    for (const auto& b : buttons_) {
    //
    b->onClick = [this, n = index++]() { setValue (n); };
    
    b->setEnabled (enabled);
    
    addAndMakeVisible (*b);
    //
    }
    
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void resetAllExcluding (std::vector<std::unique_ptr<juce::ToggleButton>>& buttons, int n)
{
    int index = 0;
    
    for (const auto& b : buttons) {
        if (n != index++) {
            b->setToggleState (false, juce::NotificationType::dontSendNotification);
        }
    }
}

juce::String getChoice (std::vector<std::unique_ptr<juce::ToggleButton>>& buttons)
{
    for (const auto& b : buttons) { if (b->getToggleState()) { return b->getButtonText(); } }
    
    return juce::String();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ChoicesSelector::setValue (int i)
{
    resetAllExcluding (buttons_, i);
    
    const juce::String t = getChoice (buttons_);
    
    DBG (t);
    
    value_.setValue (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ChoicesSelector::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::parametersColourBackground));
}

void ChoicesSelector::resized()
{
    if (buttons_.size()) {
    //
    juce::Rectangle<int> bounds (getLocalBounds());
    
    const int h = bounds.getHeight() / buttons_.size();
    
    for (const auto& b : buttons_) { b->setBounds (bounds.removeFromTop (h)); }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
