
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

protected:
    explicit Item (const juce::ValueTree& t) : tree_ (t), data_ (t.getChildWithName (Id::DATA))
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
    juce::ValueTree asValueTree() const
    {
        return tree_;
    }
    
    core::Data getData() const
    {
        return data_;
    }

    juce::String debug() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isObject() const
    {
        return Tree::isObject (tree_);
    }
    
    bool isLine() const
    {
        return Tree::isLine (tree_);
    }

    bool isPatch() const
    {
        return Tree::isPatch (tree_);
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
    void changeIdentifier (UniqueId i)
    {
        tree_.setProperty (Id::identifier, Cast::toVar (i), nullptr);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isLocked() const
    {
        return tree_.getProperty (Id::locked);
    }
    
    UniqueId getIdentifier() const
    {
        return Cast::fromVar<UniqueId> (tree_.getProperty (Id::identifier));
    }
    
    template <class T>
    Cached<T> getCached (const juce::String& group, const juce::String& key, bool synchronous = false) const
    {
        return Cached<T>::make (data_, group, key, synchronous);
    }
    
    template <class T>
    T get (const juce::String& group, const juce::String& key) const
    {
        return data_.getParameter (group, key).getValueTyped<T>();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void apply (const Item&);

public:
    static Item createCopy (const Item&);
    
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
