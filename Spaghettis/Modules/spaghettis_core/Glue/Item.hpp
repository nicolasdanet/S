
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Report;

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
    bool isObject() const
    {
        return isObject (tree_);
    }
    
    bool isLine() const
    {
        return isLine (tree_);
    }

    bool isPatch() const
    {
        return isPatch (tree_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addObserver (Observer* observer)
    {
        data_.addObserver (observer);
    }
    
    void removeObserver (Observer* observer)
    {
        data_.removeObserver (observer);
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

/* An object can be a patch. */
/* A patch is a kind of object. */

public:
    static bool isObject (const juce::ValueTree& tree)
    {
        return (tree.getType() == Ids::OBJECT || tree.getType() == Ids::PATCH);
    }
    
    static bool isLine (const juce::ValueTree& tree)
    {
        return (tree.getType() == Ids::LINE);
    }

    static bool isPatch (const juce::ValueTree& tree)
    {
        return (tree.getType() == Ids::PATCH);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void copyFrom (const core::Report&);

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
