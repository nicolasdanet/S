
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class Cached : private juce::Value::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    explicit Cached (const core::Data& data,
        const juce::String& group,
        const juce::String& key,
        bool updateSynchronously) :
            value_ (data.getParameter (group, key).getValueAsValue (updateSynchronously))
    {
        value_.addListener (this);
    }

    explicit Cached()
    {
    
    }
    
public:
    ~Cached() = default;

public:
    Cached (Cached&&) = default;
    Cached& operator = (Cached&&) = default;

public:
    Cached (const Cached&) = delete;                            /* Movable only. */
    Cached& operator = (const Cached&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T get() const
    {
        return Cast::fromVar<T> (value_.getValue());
    }
    
    operator T() const
    {
        return get();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void attach (std::function<void()> f)
    {
        onChange = f;
    }
    
private:
    void valueChanged (juce::Value& value) override
    {
        if (onChange != nullptr) { onChange(); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static Cached make (const core::Data& data,
        const juce::String& group,
        const juce::String& key,
        bool updateSynchronously)
    {
        if (!data.hasParameter (group, key)) { return Cached(); }
        else {
        //
        jassert (data.getParameter (group, key).getType() == ParameterType<T>::get());
        
        return Cached (data, group, key, updateSynchronously);
        //
        }
    }
    
private:
    std::function<void()> onChange;
    
private:
    juce::Value value_;
    
private:
    JUCE_LEAK_DETECTOR (Cached)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

