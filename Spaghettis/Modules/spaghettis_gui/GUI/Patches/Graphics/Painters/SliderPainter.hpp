
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class SliderPainter : public PainterStrategy {

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
    void paintWidget (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForWidget() override;
    std::optional<core::Vector::Real> getDimensions() override;
    void setDimensions (core::Vector::Real) override;

private:
    data::Cached<juce::Colour> sliderBackgroundColour_;
    data::Cached<juce::Colour> sliderBarColour_;
    data::Cached<double> value_;
    data::Cached<double> low_;
    data::Cached<double> high_;
    data::Cached<double> interval_;
    data::Cached<bool> isVertical_;
    data::Cached<bool> isLogarithmic_;
    data::Cached<int> width_;
    data::Cached<int> height_;
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

