
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace Parameters {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ColourSelector : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ColourSelector (juce::Colour colour)
    {
        setSize (300, 280);
    }
    
    ~ColourSelector() = default;
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::orange);
    }
    
    void resized() override
    {
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourSelector)
};

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
        const juce::Colour c (getColour());
        
        juce::Rectangle<int> r = getLocalBounds().reduced (2);

        g.fillAll (Spaghettis()->getColour (Colours::preferencesColourBackground));
        g.setColour (c);
        g.fillRect (r.removeFromLeft (r.getHeight() * 2));
        g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
        g.setColour (Spaghettis()->getColour (Colours::preferencesColourText));
        g.drawText (LookAndFeel::getDisplayStringFromColour (c),
            r.withTrimmedLeft (6),
            juce::Justification::centredLeft,
            true);
    }

    juce::Colour getColour() const
    {
        return LookAndFeel::getColourFromString (value_.toString());
    }

    void setColour (const juce::Colour& colour)
    {
        value_.setValue (LookAndFeel::getValueFromColour (colour));
    }

    void mouseDown (const juce::MouseEvent&) override
    {
        auto t = std::make_unique<ColourSelector> (getColour());
        
        tracker_.track (juce::CallOutBox::launchAsynchronously (std::move (t), getScreenBounds(), nullptr));
    }
    
private:
    void valueChanged (juce::Value&) override
    {
        repaint();
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

} // namespace Parameters

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

