
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
    
    bool isGraphic() const
    {
        jassert (isObject()); return data_.hasGroup (Tags::Parameters);
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
    Cached<T> getCached (const juce::String& group, const juce::String& key, bool synchronous = false) const
    {
        return Cached<T>::make (data_, group, key, synchronous);
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
