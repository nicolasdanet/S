
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if JUCE_DEBUG

#define CHECK_REENTRANCY    jassert (reentrant_ == false); \
                            const juce::ScopedValueSetter<bool> setter (reentrant_, true, false);
                                
#else
    
#define CHECK_REENTRANCY
    
#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
        CHECK_REENTRANCY
        
        v_.push_back (std::make_unique<T> (owner, t));
    }
    
    void remove (core::UniqueId u)
    {
        CHECK_REENTRANCY
        
        v_.erase (std::remove_if (v_.begin(), v_.end(), hasSameIdentifier (u)), v_.end());
    }
    
    void sort (const std::vector<core::UniqueId>& t)
    {
        CHECK_REENTRANCY
        
        jassert (v_.size() == t.size());
        
        int i = 0; for (auto u : t) { moveAt (i++, u); }
    }
    
    void clear()
    {
        CHECK_REENTRANCY
        
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
        CHECK_REENTRANCY
        
        for_each (v_.cbegin(), v_.cend(), f);
    }
    
    template <class F> void doForEachReversed (F f) const
    {
        CHECK_REENTRANCY
        
        for_each (v_.crbegin(), v_.crend(), f);
    }
    
    template <class F> void doForUnique (F f, core::UniqueId identifier) const
    {
        CHECK_REENTRANCY
        
        auto r = std::find_if (v_.cbegin(), v_.cend(), hasSameIdentifier (identifier));
        
        if (r != v_.cend()) { f (*r); }
    }
    
    template <class F> void doForEachSelected (F f) const
    {
        auto g = [h = f](const auto& p) { if (p->isSelected()) { h (p); } };
        
        doForEach (g);
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
        else {
            jassertfalse;
        }
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

#if JUCE_DEBUG

private:
    mutable bool reentrant_ { false };
    
#endif
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Table)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#undef CHECK_REENTRANCY

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
