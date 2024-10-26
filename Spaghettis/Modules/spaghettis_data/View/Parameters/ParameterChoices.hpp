
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
            juce::PropertyComponent (s, lnf.getRequiredHeight() * getChoices().size()),
            view_ (lnf.getFont(), getChoices()),
            selector_ (p.getValueAsValue (false))
    {
        addAndMakeVisible (view_);
        addAndMakeVisible (selector_);
        setEnabled (isEditable);
        view_.setEnabled (false);
        selector_.setEnabled (isEditable);
    }

    ~ParameterChoices() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void resized() override
    {
        juce::Rectangle<int> bounds (getLookAndFeel().getPropertyComponentContentPosition (*this));
        
        selector_.setBounds (bounds.removeFromLeft (getRequiredHeight()));
        
        view_.setBounds (bounds);
    }

    void refresh() override
    {
    }

private:
    ChoicesView view_;
    ChoicesSelector selector_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterChoices)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

