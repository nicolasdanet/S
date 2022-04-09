
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Object {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Object (const juce::ValueTree& t) : tree_ (t), data_ (t.getChildWithName (Ids::DATA))
    {
        jassert (tree_.hasType (Ids::OBJECT));
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
        return cast::fromVar<UniqueId> (tree_.getProperty (Ids::identifier));
    }

    template <class T> T getAttribute (const juce::String &name) const
    {
        const Parameter p (data_.getParameter (Tags::Attributes, name));
        
        jassert (p.getType() == ParameterType<T>::get());

        return cast::fromVar<T> (p.getValue());
    }

    template <class T> core::Cached<T> getCachedAttribute (const juce::String& name) const
    {
        return core::Cached<T>::make (data_, Tags::Attributes, name);
    }
    
    template <class T> core::Cached<T> getCachedParameter (const juce::String& name) const
    {
        return core::Cached<T>::make (data_, Tags::Parameters, name);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void copyFrom (const core::Description& v)
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

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
