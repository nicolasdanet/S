
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ParameterDirectory :  public PropertyLookAndFeel,
                            public juce::PropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParameterDirectory (const data::Parameter& p,
        const PropertyLookAndFeel& lnf,
        const juce::String& s,
        bool isEditable) :
            PropertyLookAndFeel (lnf),
            juce::PropertyComponent (s),
            editor_ (p.getValueAsValue (false)),
            selector_ (p.getValueAsValue (false))
    {
        setPreferredHeight (lnf.getRequiredHeight());
        
        addAndMakeVisible (editor_);
        addAndMakeVisible (selector_);
        setEnabled (isEditable);
        editor_.setEnabled (isEditable);
        selector_.setEnabled (isEditable);
    }

    ~ParameterDirectory() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void resized() override
    {
        juce::Rectangle<int> bounds (getLookAndFeel().getPropertyComponentContentPosition (*this));
        
        selector_.setBounds (bounds.removeFromLeft (bounds.getHeight()));
        
        editor_.setBounds (bounds);
    }

    void refresh() override
    {
    }

private:
    DirectoryEditor editor_;
    DirectorySelector selector_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterDirectory)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

