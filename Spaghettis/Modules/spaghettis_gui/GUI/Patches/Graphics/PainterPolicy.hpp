
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class ObjectComponent;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PainterPolicy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PainterPolicy (ObjectComponent*, const core::Object&);
    
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
    float getScale() const;
    
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
    static auto repaint (juce::Component* component)
    {
        return [c = component]() { c->repaint(); };
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static int scaled (int n, float f)
    {
        return static_cast<int> (std::round (n * f));
    }
    
    static juce::Point<float> scaled (juce::Point<float> pt, float f)
    {
        return juce::Point (pt.x * f, pt.y * f);
    }

    static juce::Point<float> unscaled (juce::Point<float> pt, float f)
    {
        return juce::Point (pt.x / f, pt.y / f);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    ObjectComponent* owner_;
    core::Object object_;
    core::Cached<int> x_;
    core::Cached<int> y_;

public:
    static int pinHeight (float f) { return static_cast<int> (std::round (4 * f)); }
    static int pinWidth  (float f) { return static_cast<int> (std::round (8 * f)); }
    static int pinGripX  (float f) { return static_cast<int> (std::round (1 * f)); }
    static int pinGripY  (float f) { return static_cast<int> (std::round (3 * f)); }
    static int margins   (float f) { return static_cast<int> (std::round (3 * f)); }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

