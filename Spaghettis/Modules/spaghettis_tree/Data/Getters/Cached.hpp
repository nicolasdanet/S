
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::data {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class Cached : private juce::Value::Listener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    explicit Cached (const Group& group, const juce::String& key, bool updateSynchronously) :
        value_ (group.getParameter (key).getValueAsValue (updateSynchronously))
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
    
    void set (T v)
    {
        value_.setValue (Cast::toVar<T> (v));
    }
    
    operator T() const
    {
        return get();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isValid() const
    {
        return (value_.getValue().isVoid() == false);
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
    static Cached make (const Group& group, const juce::String& key, bool updateSynchronously)
    {
        if (group.hasParameter (key)) {
        //
        jassert (group.getParameter (key).getType() == ParameterType<T>::get());
        
        return Cached (group, key, updateSynchronously);
        //
        }
        
        return Cached();
    }
    
    static Cached make (const Data& data,
        const juce::String& group,
        const juce::String& key,
        bool updateSynchronously)
    {
        if (data.hasGroup (group)) { return make (data.getGroup (group), key, updateSynchronously); }
        
        return Cached();
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

