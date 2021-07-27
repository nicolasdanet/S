
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

class ColourEditor : public juce::Component, private juce::Value::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ColourEditor (const juce::Value& v) : value_ (v)
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
        g.drawText (LookAndFeel::getColourAsDisplayString (c),
            r.withTrimmedLeft (6),
            juce::Justification::centredLeft,
            true);
    }

    juce::Colour getColour() const
    {
        return LookAndFeel::parseColour (value_.toString());
    }

    /*
    void setColour (juce::Colour newColour)
    {
        if (getColour() != newColour)
        {
            if (newColour == defaultColour && canResetToDefault)
                colourValue = var();
            else
                colourValue = newColour.toDisplayString (true);
        }
    }
    */

    /*
    void mouseDown (const MouseEvent&) override
    {
        if (undoManager != nullptr)
            undoManager->beginNewTransaction();

        CallOutBox::launchAsynchronously (std::make_unique<PopupColourSelector> (colourValue,
                                                                                 defaultColour,
                                                                                 canResetToDefault),
                                          getScreenBounds(),
                                          nullptr);
    }
    */
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void valueChanged (juce::Value&) override
    {
        repaint();
    }
    
private:
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
// MARK: -

/*


    //==============================================================================
    struct PopupColourSelector   : public Component,
                                   private ChangeListener,
                                   private Value::Listener
    {
        PopupColourSelector (const Value& colour,
                             Colour defaultCol,
                             const bool canResetToDefault)
            : defaultButton ("Reset to Default"),
              colourValue (colour),
              defaultColour (defaultCol)
        {
            addAndMakeVisible (selector);
            selector.setName ("Colour");
            selector.setCurrentColour (getColour());
            selector.addChangeListener (this);

            if (canResetToDefault)
            {
                addAndMakeVisible (defaultButton);
                defaultButton.onClick = [this]
                {
                    setColour (defaultColour);
                    selector.setCurrentColour (defaultColour);
                };
            }

            colourValue.addListener (this);
            setSize (300, 400);
        }

        void resized() override
        {
            if (defaultButton.isVisible())
            {
                selector.setBounds (0, 0, getWidth(), getHeight() - 30);
                defaultButton.changeWidthToFitText (22);
                defaultButton.setTopLeftPosition (10, getHeight() - 26);
            }
            else
            {
                selector.setBounds (getLocalBounds());
            }
        }

        Colour getColour() const
        {
            if (colourValue.toString().isEmpty())
                return defaultColour;

            return Colour::fromString (colourValue.toString());
        }

        void setColour (Colour newColour)
        {
            if (getColour() != newColour)
            {
                if (newColour == defaultColour && defaultButton.isVisible())
                    colourValue = var();
                else
                    colourValue = newColour.toDisplayString (true);
            }
        }

    private:
        void changeListenerCallback (ChangeBroadcaster*) override
        {
            if (selector.getCurrentColour() != getColour())
                setColour (selector.getCurrentColour());
        }

        void valueChanged (Value&) override
        {
            selector.setCurrentColour (getColour());
        }

        StoredSettings::ColourSelectorWithSwatches selector;
        TextButton defaultButton;
        Value colourValue;
        Colour defaultColour;
    };
*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Parameters

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

