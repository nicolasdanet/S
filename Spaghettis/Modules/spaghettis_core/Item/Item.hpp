
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class Item {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend struct Tree;

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
    juce::String toString() const
    {
        return data::Data::toDebugString (tree_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::ValueTree getTree() const
    {
        return tree_;
    }
    
    data::Data getData() const
    {
        return data_;
    }
    
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
    void addObserver (data::Observer* observer)
    {
        data_.addObserver (observer);
    }
    
    void removeObserver (data::Observer* observer)
    {
        data_.removeObserver (observer);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void apply (const Item& item)
    {
        data_.changeValues (item.data_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void changeIdentifier (UniqueId i)
    {
        tree_.setProperty (Id::identifier, data::Cast::toVar (i), nullptr);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isLocked() const
    {
        return tree_.getProperty (Id::locked);      /* Is inside abstraction. */
    }
    
    UniqueId getIdentifier() const
    {
        return data::Cast::fromVar<UniqueId> (tree_.getProperty (Id::identifier));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool hasParameter (const juce::String& group, const juce::String& key) const
    {
        return data_.hasParameter (group, key);
    }
    
    data::Parameter getParameter (const juce::String& group, const juce::String& key) const
    {
        return data_.getParameter (group, key);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class T>
    data::Cached<T> getCached (const juce::String& group,
        const juce::String& key,
        bool synchronous = false) const
    {
        return data::Cached<T>::make (data_, group, key, synchronous);
    }
    
    template <class T>
    T getChecked (const juce::String& group, const juce::String& key) const
    {
        if (hasParameter (group, key)) { return get<T> (group, key); }
        
        return T();
    }
    
    template <class T>
    T get (const juce::String& group, const juce::String& key) const
    {
        return getParameter (group, key).getValueTyped<T>();
    }

protected:
    juce::ValueTree tree_;

private:
    data::Data data_;
    
private:
    JUCE_LEAK_DETECTOR (Item)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
