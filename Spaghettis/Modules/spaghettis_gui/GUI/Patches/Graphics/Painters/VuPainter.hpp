
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class VuPainter : public PainterStrategy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit VuPainter (ObjectComponent*);
    
    ~VuPainter() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::ColourGradient getGradient (const juce::Rectangle<int>&);

private:
    void paintBar (juce::Rectangle<int>, const juce::ColourGradient&, juce::Graphics&);
    void paintPeak (juce::Rectangle<int>, const juce::ColourGradient&, juce::Graphics&);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paintWidget (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForWidget() override;
    std::optional<core::Vector::Real> getDimensions() override;
    void setDimensions (core::Vector::Real) override;

private:
    data::Cached<juce::Colour> vuBackgroundColour_;
    data::Cached<juce::Colour> vuBarColdColour_;
    data::Cached<juce::Colour> vuBarWarmColour_;
    data::Cached<juce::Colour> vuBarHotColour_;
    data::Cached<double> value_;
    data::Cached<double> peak_;
    data::Cached<int> width_;
    data::Cached<int> height_;

public:
    static constexpr double rangeLow_  = -60.0;
    static constexpr double rangeHigh_ =  12.0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VuPainter)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

