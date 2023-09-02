
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class RadioPainter : public PainterPolicy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    explicit RadioPainter (ObjectComponent*);
    
    ~RadioPainter() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void mouseDown (const juce::MouseEvent&) override;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paintObject (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForObject() override;
    std::optional<core::Vector::Real> getDimensions() override;
    void setDimensions (core::Vector::Real) override;

private:
    void paintStateSingle (juce::Rectangle<int>, juce::Graphics&);
    void paintStateMultiple (juce::Rectangle<int>, juce::Graphics&);
    void paintBackground (juce::Rectangle<int>, juce::Graphics&);
    
private:
    core::Cached<juce::Colour> radioBackgroundColour_;
    core::Cached<juce::Colour> radioButtonColour_;
    core::Cached<double> value_;
    core::Cached<bool> isVertical_;
    core::Cached<bool> isMultiple_;
    core::Cached<int> buttons_;
    core::Cached<int> width_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadioPainter)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

