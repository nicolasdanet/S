
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class RadioPainter : public PainterStrategy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit RadioPainter (ObjectComponent*);
    
    ~RadioPainter() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    int getSelectorAt (juce::Point<int>) const;

private:
    void setStateProceed (double);
    void setStateSingle (int);
    void setStateMultiple (int);
    void setState (int);
    
public:
    void mouseDown (const juce::MouseEvent&) override;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paintWidget (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForWidget() override;
    std::optional<core::Vector::Real> getDimensions() override;
    void setDimensions (core::Vector::Real) override;

private:
    void paintStateSingle (const juce::Rectangle<int>&, juce::Graphics&);
    void paintStateMultiple (const juce::Rectangle<int>&, juce::Graphics&);
    void paintBackground (const juce::Rectangle<int>&, juce::Graphics&);

private:
    data::Cached<juce::Colour> radioBackgroundColour_;
    data::Cached<juce::Colour> radioButtonColour_;
    data::Cached<double> value_;
    data::Cached<bool> isVertical_;
    data::Cached<bool> isMultiple_;
    data::Cached<int> buttons_;
    data::Cached<int> width_;
    juce::Rectangle<int> painted_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadioPainter)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

