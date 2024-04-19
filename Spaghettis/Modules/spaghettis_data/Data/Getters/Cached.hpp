
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
    Cached (const Cached& other) : onChange_ (other.onChange_), value_ (other.value_)
    {
        value_.addListener (this);
    }
    
public:
    Cached& operator = (const Cached&) = delete;
    Cached (Cached&&) = delete;
    Cached& operator = (Cached&&) = delete;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/*
public:
    bool isValid() const
    {
        return (value_.getValue().isVoid() == false);
    }
*/

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
    
    explicit operator T() const
    {
        return get();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void attach (std::function<void()> f)
    {
        onChange_ = f;
    }
    
private:
    void valueChanged (juce::Value& value) override
    {
        if (onChange_ != nullptr) { onChange_(); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static Cached make (const Group& group, const juce::String& key, bool updateSynchronously)
    {
        if (group.hasParameter (key)) {
            jassert (group.getParameter (key).getType() == ParameterType<T>::get());
            return Cached (group, key, updateSynchronously);
        }
        
        return Cached();
    }

public:
    static Cached make (const Data& data,
        const juce::String& group,
        const juce::String& key,
        bool updateSynchronously)
    {
        if (data.hasGroup (group)) {
            return make (data.getGroup (group), key, updateSynchronously);
        }
        
        return Cached();
    }
    
private:
    std::function<void()> onChange_;
    
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

