
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

class PainterPolicy : public juce::MouseListener {

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
// MARK: -

public:
    void paint (const juce::Rectangle<int>&, juce::Graphics&);
    
    juce::Rectangle<int> getRequiredBounds();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    float getScale() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    virtual void paintObject (juce::Rectangle<float>, juce::Graphics&) = 0;
    virtual juce::Rectangle<float> getRequiredBoundsForObject() = 0;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    juce::Rectangle<float> paintLabel (juce::Rectangle<float>, juce::Graphics&);
    juce::Rectangle<float> getRequiredBoundsWithLabel (juce::Rectangle<float>);
    
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
    static float scaled (int n, float f)
    {
        return (n * f);
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
    ObjectComponent* component_;
    core::Object object_;
    core::Cached<int> x_;
    core::Cached<int> y_;
    core::Cached<juce::Colour> patchBackgroundColour_;
    core::Cached<juce::Colour> labelBackgroundColour_;
    core::Cached<juce::Colour> labelTextColour_;
    float objectWidth_;

public:
    static int pinHeight (float f) { return static_cast<int> (std::round (4 * f)); }
    static int pinWidth  (float f) { return static_cast<int> (std::round (8 * f)); }
    static int pinGripX  (float f) { return (f >= 1.0f) ? 1 : 0; }
    static int pinGripY  (float f) { return (f >= 1.0f) ? 3 : 0; }
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

