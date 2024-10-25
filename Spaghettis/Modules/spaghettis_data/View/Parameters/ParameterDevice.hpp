
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ParameterDevice : public PropertyLookAndFeel,
                        public juce::PropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParameterDevice (const data::Parameter& p,
        const PropertyLookAndFeel& lnf,
        const juce::String& s,
        bool isEditable) :
            PropertyLookAndFeel (lnf),
            juce::PropertyComponent (s),
            selector_ (p.getValueAsValue (false))
    {
        addAndMakeVisible (selector_);
        setEnabled (isEditable);
        selector_.setEnabled (isEditable);
    }

    ~ParameterDevice() = default;

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

private:
    DeviceSelector selector_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterDevice)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

