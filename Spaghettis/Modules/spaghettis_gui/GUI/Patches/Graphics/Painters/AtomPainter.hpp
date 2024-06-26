
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class AtomPainter : public PainterStrategy {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit AtomPainter (ObjectComponent*);
    
    ~AtomPainter() = default;

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
    int getDigits() const;
    int getTriangleWidth() const;
    
    juce::Font getFont() const;
    
    juce::String getPlaceholder() const;
    juce::String getText() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paintWidget (juce::Rectangle<int>, juce::Graphics&) override;
    juce::Rectangle<int> getRequiredBoundsForWidget() override;
    
private:
    data::Cached<juce::Colour> atomBackgroundColour_;
    data::Cached<juce::Colour> atomTextColour_;
    data::Cached<juce::Colour> atomClickedColour_;
    data::Cached<int> digits_;
    data::Cached<double> value_;
    data::Cached<double> low_;
    data::Cached<double> high_;
    data::Cached<double> interval_;
    double v_;
    bool dragged_;

private:
    static constexpr int width_ = 3;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AtomPainter)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

