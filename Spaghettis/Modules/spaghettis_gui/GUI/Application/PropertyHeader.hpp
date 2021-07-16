
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class PropertyHeader :  public juce::Component,
                        public juce::ChangeBroadcaster {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    PropertyHeader (const juce::String& name) : juce::Component (name)
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

    void mouseUp (const juce::MouseEvent& e) override
    {
        if (!e.mouseWasDraggedSinceMouseDown()) { sendChangeMessage(); }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertyHeader)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

