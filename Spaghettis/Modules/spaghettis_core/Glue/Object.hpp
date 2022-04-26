
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Object {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Object (const juce::ValueTree& t) : tree_ (t), data_ (t.getChildWithName (Ids::DATA))
    {
        jassert (tree_.hasType (Ids::OBJECT) || tree_.hasType (Ids::PATCH));
    }

public:
    ~Object() = default;

public:
    Object (const Object&) = default;
    Object (Object&&) = default;
    Object& operator = (const Object&) = default;
    Object& operator = (Object&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    UniqueId getIdentifier() const
    {
        return Cast::fromVar<UniqueId> (tree_.getProperty (Ids::identifier));
    }

    template <class T> core::Cached<T> getCachedAttribute (const juce::String& name) const
    {
        return core::Cached<T>::make (data_, Tags::Attributes, name, false);
    }
    
    template <class T> core::Cached<T> getCachedParameter (const juce::String& name) const
    {
        return core::Cached<T>::make (data_, Tags::Parameters, name, false);
    }
    
    template <class T> T getAttribute (const juce::String &name) const
    {
        return getCachedAttribute<T> (name).get();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void copyFrom (const core::Report& v)
    {
        data_.copyFrom (v);
    }

private:
    juce::ValueTree tree_;
    core::Data data_;
    
private:
    JUCE_LEAK_DETECTOR (Object)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
