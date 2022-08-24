
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Item {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit Item (const juce::ValueTree& t) : tree_ (t), data_ (t.getChildWithName (Ids::DATA))
    {
    }

public:
    ~Item() = default;

public:
    Item (const Item&) = default;
    Item (Item&&) = default;
    Item& operator = (const Item&) = default;
    Item& operator = (Item&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isPatch() const
    {
        return Report::isPatch (tree_);
    }
    
    bool isObject() const
    {
        return Report::isObject (tree_);
    }
    
    bool isLine() const
    {
        return Report::isLine (tree_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    UniqueId getIdentifier() const
    {
        return Cast::fromVar<UniqueId> (tree_.getProperty (Ids::identifier));
    }

    template <class T>
    core::Cached<T> getCached (const juce::String& group, const juce::String& key, bool sync = false) const
    {
        return core::Cached<T>::make (data_, group, key, sync);
    }
    
    template <class T>
    core::Cached<T> getCachedAttribute (const juce::String& name, bool updateSynchronously = false) const
    {
        return core::Cached<T>::make (data_, Tags::Attributes, name, updateSynchronously);
    }
    
    template <class T>
    core::Cached<T> getCachedParameter (const juce::String& name, bool updateSynchronously = false) const
    {
        return core::Cached<T>::make (data_, Tags::Parameters, name, updateSynchronously);
    }
    
    template <class T>
    T get (const juce::String& group, const juce::String& key) const
    {
        return getCached<T> (group, key).get();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void copyFrom (const core::Report& v)
    {
        data_.copyFrom (v);
    }

protected:
    juce::ValueTree tree_;
    core::Data data_;
    
private:
    JUCE_LEAK_DETECTOR (Item)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
