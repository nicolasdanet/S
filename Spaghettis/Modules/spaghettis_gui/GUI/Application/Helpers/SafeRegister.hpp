
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class K, class V> class SafeRegister : private juce::Timer {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SafeRegisterElement {

public:
    explicit SafeRegisterElement (K* parent, V* child) : parent_ (parent), child_ (child)
    {
    };
    
    ~SafeRegisterElement() = default;

    SafeRegisterElement (const SafeRegisterElement&) = default;
    SafeRegisterElement (SafeRegisterElement&&) = default;
    SafeRegisterElement& operator = (const SafeRegisterElement&) = default;
    SafeRegisterElement& operator = (SafeRegisterElement&&) = default;
    
public:
    juce::Component::SafePointer<K> parent_;
    juce::Component::SafePointer<V> child_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    SafeRegister()          = default;
    virtual ~SafeRegister() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void add (K* parent, V* child)
    {
        v_.emplace_back (parent, child); const int primeInterval = 5009; startTimer (primeInterval);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    template <class F> void perform (F f) const
    {
        for (const auto& e : v_) {
        //
        if (e.parent_.getComponent() != nullptr && e.child_.getComponent() != nullptr) {
            f (e.parent_.getComponent(), e.child_.getComponent());
        }
        //
        }
    }
    
    /* Use the function above only to consume the result into a range based loop. */
    /* https://www.fluentcpp.com/2021/05/22/the-subtle-dangers-of-temporaries-in-for-loops/ */
    
    std::vector<V*> getChilds (K* component) const
    {
        std::vector<V*> childs;
        
        for (const auto& e : v_) {
        //
        if (e.parent_.getComponent() == component && e.child_.getComponent() != nullptr) {
            childs.push_back (e.child_.getComponent());
        }
        //
        }
        
        return childs;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void prune()
    {
        auto f = [] (const SafeRegisterElement& e)
        {
            return (e.parent_.getComponent() == nullptr) || (e.child_.getComponent() == nullptr);
        };
        
        v_.erase (std::remove_if (v_.begin(), v_.end(), f), v_.end());
    }
    
    void timerCallback() override
    {
        prune(); if (v_.empty()) { stopTimer(); }
    }
    
private:
    std::vector<SafeRegisterElement> v_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafeRegister)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

