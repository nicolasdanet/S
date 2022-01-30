
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Parameter {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ColourEditor : public juce::Component, private juce::Value::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ColourEditor (const juce::Value& v) : tracker_ (this), value_ (v)
    {
        value_.addListener (this);
    }

    ~ColourEditor() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        const juce::Colour c (LookAndFeel::getColourFromValue (value_));
        
        g.fillAll (Spaghettis()->getColour (Colours::preferencesColourBackground));
        
        g.fillCheckerBoard (getColourBounds().toFloat(),
            11.0f,
            11.0f,
            juce::Colours::grey.overlaidWith (c),
            juce::Colours::white.overlaidWith (c));
        
        g.setFont (Spaghettis()->getLookAndFeel().getColourFont());
        g.setColour (Spaghettis()->getColour (Colours::preferencesColourText));
        g.drawText (LookAndFeel::getDisplayStringFromColour (c),
            getTextBounds(),
            juce::Justification::centredLeft,
            true);
        
        g.drawRect (getColourBounds().toFloat());
    }

    void mouseDown (const juce::MouseEvent&) override
    {
        auto t = std::make_unique<ColourSelector> (value_);
        auto r = localAreaToGlobal (getColourBounds());
        
        tracker_.track (juce::CallOutBox::launchAsynchronously (std::move (t), r, nullptr));
    }

private:
    void valueChanged (juce::Value&) override
    {
        repaint();
    }
    
private:
    juce::Rectangle<int> getColourBounds() const
    {
        auto r = getLocalBounds().reduced (4, 2); return r.removeFromLeft (r.getHeight() * 2);
    }

    juce::Rectangle<int> getTextBounds() const
    {
        auto r = getLocalBounds().reduced (4, 2); return r.withTrimmedLeft (r.getHeight() * 2 + 6);
    }

private:
    CallOutBoxTracker tracker_;
    juce::Value value_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourEditor)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Colour : public juce::PropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Colour (juce::ValueTree p) : juce::PropertyComponent (p.getProperty (Ids::text).toString()),
        editor_ (p.getPropertyAsValue (Ids::value, nullptr))
    {
        addAndMakeVisible (editor_);
    }

    ~Colour() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void resized() override
    {
        editor_.setBounds (getLookAndFeel().getPropertyComponentContentPosition (*this));
    }

    void refresh() override
    {
    }

private:
    ColourEditor editor_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Colour)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Parameter

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

