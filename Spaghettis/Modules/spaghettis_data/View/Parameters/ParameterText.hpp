
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ParameterText :   public PropertyLookAndFeel,
                        public juce::PropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParameterText (const data::Parameter& p,
        const PropertyLookAndFeel& lnf,
        const juce::String& s,
        bool isEditable) :
            PropertyLookAndFeel (lnf),
            juce::PropertyComponent (s, getPropertyComponentHeight (p, isEditable)),
            editor_ (p, getFont())
    {
        addAndMakeVisible (editor_);
        setEnabled (isEditable);
        editor_.setEnabled (isEditable);
    }

    ~ParameterText() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int getPropertyComponentHeight (const data::Parameter& p, const bool isEditable) const
    {
        int k = getRequiredHeight();
        
        if (isEditable) { return k * 10; }
        else {
            int n = p.getValueTyped<TextBlock>().getNumberOfLines();
            if (n > 1) {
                k += (n - 1) * (getFont().getHeight() + 3);
            }
            return k;
        }
    }
    
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
    TextBlockEditor editor_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterText)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

