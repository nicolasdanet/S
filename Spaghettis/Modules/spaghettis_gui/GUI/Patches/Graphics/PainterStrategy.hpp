
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

class PainterStrategy : public juce::MouseListener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit PainterStrategy (ObjectComponent*);
    
    virtual ~PainterStrategy() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    virtual void paintObject (juce::Rectangle<int>, juce::Graphics&) = 0;
    virtual juce::Rectangle<int> getRequiredBoundsForObject() = 0;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    virtual juce::Colour getPinsBackground()
    {
        return boxPinsBackgroundColour_.get();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    virtual std::optional<core::Vector::Real> getDimensions()
    {
        return std::nullopt;
    }
    
    virtual void setDimensions (core::Vector::Real)
    {
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void paintLabel (juce::Rectangle<int>, juce::Graphics&);
    
public:
    void paint (juce::Rectangle<int>, juce::Graphics&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::Rectangle<int> getRequiredBounds();
    
    int getWidthOfPinsBackground() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    float getScale() const;
    core::Point::Scaled getPosition() const;
    core::UniqueId getIdentifier() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::Rectangle<int> withMinimumWidthForPins (juce::Rectangle<int>);
    
protected:
    juce::Rectangle<int> getRequiredBoundsFromDimensions();
    juce::Rectangle<int> getRequiredBoundsFromVector (core::Vector::Scaled);
    juce::Rectangle<int> getRequiredBoundsFromText (const juce::String&, const juce::Font&, int extra = 0);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    void paintText (juce::Rectangle<int>,
            juce::Graphics&,
            const juce::String&,
            const juce::Font&,
            juce::Justification);
                    
    void paintTextAsDigits (juce::Rectangle<int>,
            juce::Graphics&,
            const juce::String&,
            const juce::Font&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    void setDimensionsByParameters (core::Vector::Real);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static auto repaint (juce::Component* component)
    {
        return [c = component]() { c->repaint(); };
    }

    static auto resized (juce::Component* component)
    {
        return [c = component]() { c->resized(); };
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    ObjectComponent* component_;
    core::Object object_;
    data::Cached<juce::Colour> boxPinsBackgroundColour_;
    data::Cached<juce::Colour> labelBackgroundColour_;
    data::Cached<juce::Colour> labelTextColour_;
    int objectWidth_;

public:
    static int pinHeight (float f) { return static_cast<int> (std::round (4 * f)); }
    static int pinWidth  (float f) { return static_cast<int> (std::round (8 * f)); }
    static int pinGripX  (float f) { return (f >= 1.0f) ? 1 : 0; }
    static int pinGripY  (float f) { return (f >= 1.0f) ? 3 : 0; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PainterStrategy)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
