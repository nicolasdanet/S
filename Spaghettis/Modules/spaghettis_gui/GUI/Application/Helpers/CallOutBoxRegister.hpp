
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class CallOutBoxRegister : private juce::Timer {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class CallOutBoxElement {

public:
    explicit CallOutBoxElement (juce::Component* owner, juce::CallOutBox* child) :
        owner_ (owner),
        child_ (child)
    {
    };
    
    ~CallOutBoxElement() = default;

    CallOutBoxElement (const CallOutBoxElement&) = default;
    CallOutBoxElement (CallOutBoxElement&&) = default;
    CallOutBoxElement& operator = (const CallOutBoxElement&) = default;
    CallOutBoxElement& operator = (CallOutBoxElement&&) = default;
    
public:
    juce::Component::SafePointer<juce::Component> owner_;
    juce::Component::SafePointer<juce::CallOutBox> child_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    CallOutBoxRegister()  = default;
    ~CallOutBoxRegister() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void add (juce::Component* owner, juce::CallOutBox* child)
    {
        v_.emplace_back (owner, child); const int interval = 5000; startTimer (interval);
    }
    
    void dismiss (juce::Component* component) const
    {
        for (const auto& e : v_) {
        //
        if (e.owner_.getComponent() == component && e.child_.getComponent() != nullptr) {
            e.child_->dismiss();
        }
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void prune()
    {
        auto f = [] (const CallOutBoxElement& e)
        {
            return (e.owner_.getComponent() == nullptr) || (e.child_.getComponent() == nullptr);
        };
        
        v_.erase (std::remove_if (v_.begin(), v_.end(), f), v_.end());
    }
    
    void timerCallback() override
    {
        prune(); if (v_.empty()) { stopTimer(); }
    }
    
private:
    std::vector<CallOutBoxElement> v_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CallOutBoxRegister)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

