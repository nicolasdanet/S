
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class ParameterNumber :  public PropertyLookAndFeel,
                                            public juce::TextPropertyComponent {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ParameterNumber (const data::Parameter& p,
        const PropertyLookAndFeel& lnf,
        const juce::String& s,
        bool isEditable) :
            PropertyLookAndFeel (lnf),
            juce::TextPropertyComponent (p.getValueAsValue (false), s, 32, false),
            v_(),
            range_ (p)
    {
        setEnabled (isEditable);
    }

    ~ParameterNumber() = default;
    
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

private:
    static T convert (const juce::String& s)
    {
        if constexpr (std::is_same_v<T, int>)    { return s.getIntValue();    }
        if constexpr (std::is_same_v<T, double>) { return s.getDoubleValue(); }
    }

public:
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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

