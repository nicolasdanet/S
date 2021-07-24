
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

class Range {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Range (const juce::ValueTree& p) : p_ (p) { }

public:
    bool isSet() const
    {
        return (p_.hasProperty (Ids::minimum) &&  p_.hasProperty (Ids::maximum));
    }
    
    double getMinimumAsDouble() const
    {
        double m = static_cast<double> (p_.getProperty (Ids::minimum));
        double n = static_cast<double> (p_.getProperty (Ids::maximum));
        
        return juce::jmin (m, n);
    }
    
    double getMaximumAsDouble() const
    {
        double m = static_cast<double> (p_.getProperty (Ids::minimum));
        double n = static_cast<double> (p_.getProperty (Ids::maximum));
        
        return juce::jmax (m, n);
    }
    
    double getStep() const
    {
        return 0.001;
    }
    
    template <class T> operator juce::Range<T>() const
    {
        auto m = static_cast<T> (p_.getProperty (Ids::minimum));
        auto n = static_cast<T> (p_.getProperty (Ids::maximum));
        auto minimum = juce::jmin (m, n);
        auto maximum = juce::jmax (m, n);
            
        if (minimum != maximum) { return juce::Range<T> (minimum, maximum); }
        else {
            return juce::Range<T>();
        }
    }

private:
    const juce::ValueTree& p_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Boolean : public juce::BooleanPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Boolean (juce::ValueTree p) :
        juce::BooleanPropertyComponent (p.getPropertyAsValue (Ids::value, nullptr),
            p.getProperty (Ids::text).toString(),
            "")
    {
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Boolean)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Text : public juce::TextPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Text (juce::ValueTree p) :
        juce::TextPropertyComponent (p.getPropertyAsValue (Ids::value, nullptr),
            p.getProperty (Ids::text).toString(),
            64,
            false)
    {
        setInterestedInFileDrag (false);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Text)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class Number : public juce::TextPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Number (juce::ValueTree p, const Range& range) :
        juce::TextPropertyComponent (p.getPropertyAsValue (Ids::value, nullptr),
            p.getProperty (Ids::text).toString(),
            32,
            false),
        v_ (),
        range_ (range)
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
    explicit Slider (juce::ValueTree p, const Range& range) :
        SliderPropertyComponent (p.getPropertyAsValue (Ids::value, nullptr),
            p.getProperty (Ids::text).toString(),
            range.getMinimumAsDouble(),
            range.getMaximumAsDouble(),
            range.getStep())
    {
        slider.valueFromTextFunction = [this](const juce::String& text)
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

