
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core { struct Tree; }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::data {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum class ParameterScope { local, delegate };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Parameter {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend class  Group;
friend class  Observer;
friend struct core::Tree;

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

public:
    Parameter (const Parameter&) = default;
    Parameter (Parameter&&) = default;
    Parameter& operator = (const Parameter&) = default;
    Parameter& operator = (Parameter&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isBoolean() const;
    bool isColour() const;
    bool isRectangle() const;
    bool isInteger() const;
    bool isFloat() const;
    bool isString() const;
    bool isText() const;
    bool isDirectory() const;
    bool isDevice() const;

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
    template <class T> T getValueTyped() const
    {
        jassert (getType() == ParameterType<T>::get()); return Cast::fromVar<T> (getValue());
    }
    
    template <class T> T getValueTypedUnchecked() const
    {
        return Cast::fromVar<T> (getValue());
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Value getValueAsValue (bool updateSynchronously) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void changeValue (const juce::var&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Parameter& setHidden (bool,   ParameterScope scope = ParameterScope::delegate);
    Parameter& setEditable (bool, ParameterScope scope = ParameterScope::delegate);

    bool isHidden() const;
    bool isEditable() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class T> Parameter& setRange (juce::Range<T> range)
    {
        set (Id::minimum, range.getStart(), ParameterScope::delegate);
        set (Id::maximum, range.getEnd(),   ParameterScope::delegate);
        
        return *this;
    }

    template <class T> Parameter& setPositive()
    {
        return setRange (juce::Range<T> (0, std::numeric_limits<T>::max()));
    }
    
    template <class T> operator juce::Range<T>() const
    {
        auto m = static_cast<T> (get (Id::minimum));
        auto n = static_cast<T> (get (Id::maximum));
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

public:
    Parameter& setChoicesSource (ChoicesSource);
    
    ChoicesSource getChoicesSource() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::Value getSource (const juce::Identifier&, bool) const;
    const juce::var& get (const juce::Identifier&) const;
    void change (const juce::Identifier&, const juce::var&);
    void set (const juce::Identifier&, const juce::var&, ParameterScope);

private:
    juce::var forceType (const juce::var&) const;
    juce::var forceRange (const juce::var&) const;
    juce::Value filtered (const juce::Value&) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::ValueTree parameter_;

private:
    JUCE_LEAK_DETECTOR (Parameter)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
