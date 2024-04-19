
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ParameterSlider : public ParametersBase,
                        public juce::SliderPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParameterSlider (const data::Parameter& p,
        const ParametersBase& base,
        const juce::String& s,
        bool isEditable) :
            ParametersBase (base),
            SliderPropertyComponent (p.getValueAsValue (false),
                s,
                p.getMinimumAsDouble(),
                p.getMaximumAsDouble(),
                p.getStep())
    {
        slider.valueFromTextFunction = [this] (const juce::String& text)
        {
            return ParameterNumber<double>::parsedWithDefault (text, slider.getValue());
        };
        
        setEnabled (isEditable);
    }

    ~ParameterSlider() = default;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

