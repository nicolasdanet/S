
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SliderPainter : public PainterPolicy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit SliderPainter (ObjectComponent*);
    
    ~SliderPainter() = default;

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
    void mouseProceed (juce::Point<int>);
    
private:
    float getNormalizedValue() const;
    
private:
    void paintBar (juce::Rectangle<int>, juce::Graphics&);
    
public:
    void paintObject (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForObject() override;
    std::optional<core::Vector::Real> getDimensions() override;
    void setDimensions (core::Vector::Real) override;

private:
    core::Cached<juce::Colour> sliderBackgroundColour_;
    core::Cached<juce::Colour> sliderBarColour_;
    core::Cached<double> value_;
    core::Cached<double> low_;
    core::Cached<double> high_;
    core::Cached<bool> isVertical_;
    core::Cached<bool> isLogarithmic_;
    core::Cached<int> width_;
    core::Cached<int> height_;
    juce::Rectangle<int> painted_;
    bool dragged_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderPainter)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

