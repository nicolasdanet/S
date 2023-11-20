
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class VuPainter : public PainterPolicy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit VuPainter (ObjectComponent*);
    
    ~VuPainter() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paintObject (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForObject() override;
    std::optional<core::Vector::Real> getDimensions() override;
    void setDimensions (core::Vector::Real) override;

private:
    core::Cached<juce::Colour> vuBackgroundColour_;
    core::Cached<juce::Colour> vuBarColdColour_;
    core::Cached<juce::Colour> vuBarWarmColour_;
    core::Cached<juce::Colour> vuBarHotColour_;
    core::Cached<double> value_;
    core::Cached<double> peak_;
    core::Cached<int> width_;
    core::Cached<int> height_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VuPainter)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

