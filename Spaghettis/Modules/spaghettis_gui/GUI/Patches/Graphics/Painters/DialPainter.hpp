
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class DialPainter : public PainterStrategy {

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
    void paintDialNeedle (juce::Rectangle<float>, juce::Graphics&, float, float);
    juce::Rectangle<float> paintDialForeground (juce::Rectangle<float>, juce::Graphics&, float, float);
    juce::Rectangle<float> paintDial (juce::Rectangle<float>, juce::Graphics&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paintObject (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForObject() override;
    std::optional<core::Vector::Real> getDimensions() override;
    void setDimensions (core::Vector::Real) override;

private:
    data::Cached<juce::Colour> dialBackgroundColour_;
    data::Cached<juce::Colour> dialForegroundColour_;
    data::Cached<juce::Colour> dialNeedleColour_;
    data::Cached<juce::Colour> dialTextColour_;
    data::Cached<double> value_;
    data::Cached<double> low_;
    data::Cached<double> high_;
    data::Cached<double> interval_;
    data::Cached<bool> isLogarithmic_;
    data::Cached<int> width_;
    data::Cached<int> digits_;
    juce::Rectangle<float> painted_;
    float v_;
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

