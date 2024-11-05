
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void initializeCreate (std::vector<std::unique_ptr<RadioButton>>& buttons, const juce::StringArray& choices)
{
    for (const auto& s : choices) { buttons.push_back (std::make_unique<RadioButton> (s)); }
}

void initializeAdd (std::vector<std::unique_ptr<RadioButton>>& buttons,
    bool isEnabled,
    ChoicesSelector* owner)
{
    int index = 0;
    
    for (const auto& b : buttons) {
        b->onClick = [owner, n = index++]() { owner->setChoiceAtIndex (n); };
        b->setEnabled (isEnabled);
        owner->addAndMakeVisible (*b);
    }
}

void initializeStatus (std::vector<std::unique_ptr<RadioButton>>& buttons, const juce::String& s)
{
    for (const auto& b : buttons) {
        if (b->getButtonText() == s) {
            b->setToggleState (true, juce::NotificationType::dontSendNotification);
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ChoicesSelector::ChoicesSelector (const juce::Value& v, const juce::StringArray& choices) : value_ (v)
{
    initialize (choices);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ChoicesSelector::initialize (const juce::StringArray& choices)
{
    initializeCreate (buttons_, choices);
    initializeAdd (buttons_, isEnabled(), this);
    initializeStatus (buttons_, value_.toString());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Only zero or one choice allowed at the same time. */

void setChoiceExclusive (std::vector<std::unique_ptr<RadioButton>>& buttons, int n)
{
    int index = 0;
    
    for (const auto& b : buttons) {
        if (n != index++) {
            b->setToggleState (false, juce::NotificationType::dontSendNotification);
        }
    }
}

juce::String getChoice (std::vector<std::unique_ptr<RadioButton>>& buttons)
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

void ChoicesSelector::setChoiceAtIndex (int i)
{
    setChoiceExclusive (buttons_, i);
    
    value_.setValue (getChoice (buttons_));
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
