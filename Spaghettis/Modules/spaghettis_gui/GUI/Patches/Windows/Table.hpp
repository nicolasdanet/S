
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

template <class T> class Table {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    Table()  = default;
    ~Table() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class U> void add (PatchView* owner, const U& t)
    {
        v_.push_back (std::make_unique<T> (owner, t));
    }
    
    void remove (core::UniqueId u)
    {
        v_.erase (std::remove_if (v_.begin(), v_.end(), hasSameIdentifier (u)), v_.end());
    }
    
    void clear()
    {
        v_.clear();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    T* get (core::UniqueId identifier) const
    {
        auto r = std::find_if (v_.cbegin(), v_.cend(), hasSameIdentifier (identifier));
        
        return (r != v_.cend()) ? r->get() : nullptr;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    template <class F> void doForEach (F f) const
    {
        for_each (v_.cbegin(), v_.cend(), f);
    }
    
    template <class F> void doForEachReversed (F f) const
    {
        for_each (v_.crbegin(), v_.crend(), f);
    }
    
    template <class F> void doForEachSelected (F f) const
    {
        auto g = [h = f](const auto& p) { if (p->isSelected()) { h (p); } };
        
        doForEach (g);
    }

    template <class F> void doForUnique (F f, core::UniqueId identifier) const
    {
        auto r = std::find_if (v_.cbegin(), v_.cend(), hasSameIdentifier (identifier));
        
        if (r != v_.cend()) { f (*r); }
    }
    
    template <class F> auto doCountIf (F f) const
    {
        return count_if (v_.cbegin(), v_.cend(), f);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void moveAt (int i, core::UniqueId identifier)
    {
        auto r = std::find_if (v_.begin(), v_.end(), hasSameIdentifier (identifier));
        
        if (r != v_.end()) { r->swap (v_[i]); }
    }

public:
    void sort (const std::vector<core::UniqueId>& t)
    {
        jassert (v_.size() == t.size());
        
        int i = 0; for (auto u : t) { moveAt (i++, u); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static auto hasSameIdentifier (core::UniqueId identifier)
    {
        return [i = identifier] (const std::unique_ptr<T>& p)
        {
            return (p->getIdentifier() == i);
        };
    }
    
private:
    std::vector<std::unique_ptr<T>> v_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Table)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
