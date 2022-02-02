
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Parameter {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Parameter (const juce::ValueTree& parameter) : parameter_ (parameter)
    {
    }
    
    ~Parameter() = default;

public:
    template <class T> void setRange (juce::Range<T> range)
    {
        parameter_.setProperty (Ids::minimum, range.getStart(), nullptr);
        parameter_.setProperty (Ids::maximum, range.getEnd(), nullptr);
    }
    
public:
    Parameter (const Parameter&) = default;
    Parameter (Parameter&&) = default;
    Parameter& operator = (const Parameter&) = default;
    Parameter& operator = (Parameter&&) = default;
    
private:
    juce::ValueTree parameter_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Tree {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static Parameter addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        bool b);

    static Parameter addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        juce::Colour c);
        
    static Parameter addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        int n);

    static Parameter addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        double f);

    static Parameter addParameter (juce::ValueTree& group,
        const juce::String& item,
        const juce::String& text,
        const juce::String& info,
        const juce::String& s);
        
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::ValueTree addGroup (juce::ValueTree& tree, const juce::String& name);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static bool isValid (const juce::ValueTree& tree, const juce::Identifier& identifier);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
