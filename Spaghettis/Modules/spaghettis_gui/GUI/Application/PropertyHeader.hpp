
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class PreferencesComponent;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class PropertyHeader :  public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    PropertyHeader (const juce::String& name, int index, PreferencesComponent *owner) :
        juce::Component (name),
        index_ (index),
        owner_ (owner)
    {
    }

    ~PropertyHeader() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

    void resized() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        juce::Rectangle<int> b (getLocalBounds().reduced (2, 0).withTrimmedBottom (1));
        
        g.setColour (Spaghettis()->getColour (Colours::preferencesHeaderBackground));
        g.fillRoundedRectangle (b.toFloat(), 2.0f);
        g.setColour (Spaghettis()->getColour (Colours::preferencesHeaderText));
        g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
        g.drawText (getName(), b.reduced (8, 0), juce::Justification::centredLeft, true);
    }

    void mouseUp (const juce::MouseEvent&) override;

private:
    int index_;
    PreferencesComponent *owner_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertyHeader)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

