
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class DialPainter : public PainterPolicy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit DialPainter (ObjectComponent*);
    
    ~DialPainter() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::Font getFont (int) const;
    juce::String getText() const;
    
    float getNormalizedValue() const;
    float getAngle() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void paintDialMarker (juce::Rectangle<float>, juce::Graphics&, float);
    juce::Rectangle<float> paintDialForeground (juce::Rectangle<float>, juce::Graphics&, float, float);
    void paintDial (juce::Rectangle<float>, juce::Graphics&, float);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paintObject (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForObject() override;
    std::optional<core::Vector::Real> getDimensions() override;
    void setDimensions (core::Vector::Real) override;

private:
    core::Cached<juce::Colour> dialBackgroundColour_;
    core::Cached<juce::Colour> dialForegroundColour_;
    core::Cached<juce::Colour> dialNeedleColour_;
    core::Cached<juce::Colour> dialTextColour_;
    core::Cached<double> value_;
    core::Cached<double> low_;
    core::Cached<double> high_;
    core::Cached<double> interval_;
    core::Cached<bool> isLogarithmic_;
    core::Cached<int> width_;
    core::Cached<int> digits_;
    bool dragged_;

private:
    static constexpr float kDial_       = 0.57f;
    static constexpr float kOffset_     = 0.08f;
    static constexpr float kDigits_     = 0.25f;
    
    static constexpr float startAngle_  = juce::MathConstants<float>::pi * 1.2f;
    static constexpr float endAngle_    = juce::MathConstants<float>::pi * 2.8f;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DialPainter)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

