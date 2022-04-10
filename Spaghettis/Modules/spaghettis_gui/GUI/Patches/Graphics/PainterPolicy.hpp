
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
    
    virtual juce::Rectangle<int> getBounds() = 0;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    core::Cached<juce::Colour> getColour (const juce::String& key) const
    {
        return Spaghettis()->getCachedColour (key);
    }
    
    template <class T> core::Cached<T> getAttribute (const juce::String& key) const
    {
        return object_.getCachedAttribute<T> (key);
    }
    
    template <class T> core::Cached<T> getParameter (const juce::String& key) const
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
        return [c = component, p = painter]() { c->setBounds (p->getBounds()); };
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
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

