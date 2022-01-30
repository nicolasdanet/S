
/* Copyright (c) 2021 Jojo and others. */

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

template <class T> class Number : public juce::TextPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Number (juce::ValueTree p, const Base& b) :
        juce::TextPropertyComponent (p.getPropertyAsValue (Ids::value, nullptr),
            p.getProperty (Ids::text).toString(),
            32,
            false),
        v_(),
        range_ (b)
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
        if (s.isNotEmpty() && s.containsOnly ("-.e+0123456789")) { return convert (s); }    // --
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Number)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using Integer = Number<int>;
using Float   = Number<double>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Slider : public juce::SliderPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Slider (juce::ValueTree p, const Base& b) :
        SliderPropertyComponent (p.getPropertyAsValue (Ids::value, nullptr),
            p.getProperty (Ids::text).toString(),
            b.getMinimumAsDouble(),
            b.getMaximumAsDouble(),
            b.getStep())
    {
        slider.valueFromTextFunction = [this] (const juce::String& text)
        {
            return Number<double>::parsedWithDefault (text, slider.getValue());
        };
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Slider)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace Parameters

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

