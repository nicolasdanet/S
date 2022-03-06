
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class ParameterNumber : public juce::TextPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParameterNumber (const core::Parameter& p) :
        juce::TextPropertyComponent (p.getValueSource(), p.getLabel(), 32, false),
        v_(),
        range_ (p)
    {
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getText() const override
    {
        return parsed (TextPropertyComponent::getText());
    }
        
    void setText (const juce::String& s) override
    {
        TextPropertyComponent::setText (parsed (s));
    }

public:
    template <class Q = T>
    static typename std::enable_if<std::is_same<int, Q>::value, T>::type convert (const juce::String& s)
    {
        return s.getIntValue();
    }
    
    template <class Q = T>
    static typename std::enable_if<std::is_same<double, Q>::value, T>::type convert (const juce::String& s)
    {
        return s.getDoubleValue();
    }
    
    static T parsedWithDefault (const juce::String& s, T t)
    {
        if (s.isNotEmpty() && s.containsOnly ("-.e+0123456789")) { return convert (s); }
        else {
            return t;
        }
    }

private:
    juce::String parsed (const juce::String& s) const
    {
        auto t = parsedWithDefault (s, v_);
        
        if (range_.isEmpty()) { v_ = t; } else { v_ = range_.clipValue (t); }
        
        return juce::String (v_);
    }
    
private:
    mutable T v_;
    juce::Range<T> range_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterNumber)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using ParameterInteger = ParameterNumber<int>;
using ParameterFloat   = ParameterNumber<double>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ParameterSlider : public juce::SliderPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParameterSlider (const core::Parameter& p) :
        SliderPropertyComponent (p.getValueSource(),
            p.getLabel(),
            p.getMinimumAsDouble(),
            p.getMaximumAsDouble(),
            p.getStep())
    {
        slider.valueFromTextFunction = [this] (const juce::String& text)
        {
            return ParameterNumber<double>::parsedWithDefault (text, slider.getValue());
        };
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

