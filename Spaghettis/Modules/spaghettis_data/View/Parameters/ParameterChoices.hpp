
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ParameterChoices :    public PropertyLookAndFeel,
                            public ChoicesList,
                            public juce::PropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParameterChoices (const data::Parameter& p,
        const PropertyLookAndFeel& lnf,
        const juce::String& s,
        bool isEditable) :
            PropertyLookAndFeel (lnf),
            ChoicesList (p),
            juce::PropertyComponent (s, getPropertyComponentHeight()),
            selector_ (p.getValueAsValue (false), getChoices())
    {
        addAndMakeVisible (selector_);
        setEnabled (isEditable);
        selector_.setEnabled (isEditable);
    }

    ~ParameterChoices() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int getPropertyComponentHeight() const
    {
        return getRequiredHeight() * juce::jmax (getChoices().size(), 1);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void resized() override
    {
        selector_.setBounds (getLookAndFeel().getPropertyComponentContentPosition (*this));
    }

    void refresh() override
    {
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void update();
    
public:
    void changeListenerCallback (juce::ChangeBroadcaster*) override
    {
        update();
    }
    
private:
    ChoicesSelector selector_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterChoices)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

