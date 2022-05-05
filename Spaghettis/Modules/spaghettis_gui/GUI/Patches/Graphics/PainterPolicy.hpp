
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PainterPolicy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PainterPolicy (juce::Component&, const core::Object&);
    
    virtual ~PainterPolicy() = default;
    
public:
    PainterPolicy (const PainterPolicy&) = delete;
    PainterPolicy (PainterPolicy&&) = delete;
    PainterPolicy& operator = (const PainterPolicy&) = delete;
    PainterPolicy& operator = (PainterPolicy&&) = delete;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    virtual void paint (const juce::Rectangle<int>&, juce::Graphics&) = 0;
    
    virtual juce::Rectangle<int> getRequiredBounds() = 0;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    template <class T> core::Cached<T> fetchAttribute (const juce::String& key) const
    {
        return object_.getCachedAttribute<T> (key);
    }
    
    template <class T> core::Cached<T> fetchParameter (const juce::String& key) const
    {
        return object_.getCachedParameter<T> (key);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static auto repainter (juce::Component* component)
    {
        return [c = component]() { c->repaint(); };
    }

    static auto resizer (juce::Component* component, PainterPolicy *painter)
    {
        return [c = component, p = painter]() { c->setBounds (p->getRequiredBounds()); };
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    template <class T> void bind (core::Cached<T>& t) const
    {
        t.attach (repainter (&owner_));
    }
    
protected:
    juce::Component& owner_;
    core::Object object_;
    core::Cached<int> x_;
    core::Cached<int> y_;

public:
    constexpr static int pinHeight()    { return 4; }
    constexpr static int pinWidth()     { return 8; }
    constexpr static int pinGripX()     { return 1; }
    constexpr static int pinGripY()     { return 3; }
    constexpr static int margins()      { return 3; }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

