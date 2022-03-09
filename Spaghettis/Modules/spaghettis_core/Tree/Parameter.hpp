
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

template <class T> struct ParameterType { };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template<> struct ParameterType<bool>
{
    static const char* get() { return "boolean"; }
};

template<> struct ParameterType<juce::Colour>
{
    static const char* get() { return "color"; }
};

template<> struct ParameterType<juce::Rectangle<int>>
{
    static const char* get() { return "rectangle"; }
};

template<> struct ParameterType<int>
{
    static const char* get() { return "integer"; }
};

template<> struct ParameterType<double>
{
    static const char* get() { return "float"; }
};

template<> struct ParameterType<juce::String>
{
    static const char* get() { return "text"; }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Parameter {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend class Group;
friend class Listener;

template <class T> friend struct Iterator;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    explicit Parameter (const juce::ValueTree& parameter = juce::ValueTree()) : parameter_ (parameter)
    {
    }

public:
    ~Parameter() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isBoolean() const;
    bool isColour() const;
    bool isRectangle() const;
    bool isInteger() const;
    bool isFloat() const;
    bool isText() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getKey() const;
    juce::String getType() const;
    juce::String getLabel() const;
    juce::String getInfo() const;
    juce::var getValue() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Value getValueSource() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void changeValue (const juce::var&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class T> Parameter& setRange (juce::Range<T> range)
    {
        set (Ids::minimum, range.getStart());
        set (Ids::maximum, range.getEnd());
        
        return *this;
    }

    template <class T> operator juce::Range<T>() const
    {
        auto m = static_cast<T> (get (Ids::minimum));
        auto n = static_cast<T> (get (Ids::maximum));
        auto minimum = juce::jmin (m, n);
        auto maximum = juce::jmax (m, n);
            
        if (minimum != maximum) { return juce::Range<T> (minimum, maximum); }
        else {
            return juce::Range<T>();
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool hasRange() const;
    double getMinimumAsDouble() const;
    double getMaximumAsDouble() const;
    double getStep() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::Value getSource (const juce::Identifier& identifier) const;
    const juce::var& get (const juce::Identifier&) const;
    void set (const juce::Identifier&, const juce::var&);
    void change (const juce::Identifier&, const juce::var&);

private:
    juce::var forceType (const juce::var&) const;
    juce::var forceRange (const juce::var&) const;
    juce::Value filtered (const juce::Value&) const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
