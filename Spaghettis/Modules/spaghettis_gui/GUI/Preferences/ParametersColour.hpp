
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
    explicit ColourEditor (const juce::Value& v) : value_ (v), colour_ (juce::Colours::black)
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
        DBG ("?");
        
        const juce::Colour colour (getColour());

        g.fillAll (colour);
        
        /*
        g.fillAll (Colours::grey);
        g.fillCheckerBoard (getLocalBounds().reduced (2).toFloat(),
                            10.0f, 10.0f,
                            Colour (0xffdddddd).overlaidWith (colour),
                            Colour (0xffffffff).overlaidWith (colour));

        g.setColour (Colours::white.overlaidWith (colour).contrasting());
        g.setFont (Font ((float) getHeight() * 0.6f, Font::bold));
        g.drawFittedText (colour.toDisplayString (true), getLocalBounds().reduced (2, 1),
                          Justification::centred, 1);
        */
    }

    juce::Colour getColour() const
    {
        return LookAndFeel::parseColour (value_.toString());
    }

    void refresh()
    {
        juce::Colour t = getColour(); if (t != colour_) { colour_ = t; repaint(); }
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
        refresh();
    }
    
private:
    juce::Value  value_;
    juce::Colour colour_;

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

