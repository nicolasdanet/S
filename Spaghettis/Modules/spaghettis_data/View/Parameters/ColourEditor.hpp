
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ColourEditor :    public  juce::Component,
                        private juce::Value::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ColourEditor (const juce::Value& v, const juce::Font& font) :
        tracker_ (this),
        value_ (v),
        font_ (font)
    {
        value_.addListener (this);
    }

    ~ColourEditor() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override;

    void mouseDown (const juce::MouseEvent&) override
    {
        if (isEnabled()) {
        //
        auto t = std::make_unique<ColourSelector> (value_);
        auto r = localAreaToGlobal (getColourBounds());
        
        tracker_.track (juce::CallOutBox::launchAsynchronously (std::move (t), r, nullptr));
        //
        }
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
    juce::Font font_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourEditor)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

