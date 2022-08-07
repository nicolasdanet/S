
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class PinComponent;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ObjectComponent : public Scalable,
                        public juce::Component,
                        public juce::ChangeBroadcaster,
                        public juce::SettableTooltipClient {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ObjectComponent (View*, const core::Object&);
    
    ~ObjectComponent();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void mouseMove (const juce::MouseEvent&) override;
    void mouseEnter (const juce::MouseEvent&) override;
    void mouseExit (const juce::MouseEvent&) override;
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    core::UniqueId getIdentifier() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    PinComponent* getInletAt (int) const;
    PinComponent* getOutletAt (int) const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics&) override;
    void resized() override;

public:
    void scaleChanged() override;
    float getScale() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isInsideRunView() const;
    bool hasLabel() const;
    juce::String getLabel() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void update (bool notify = true);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getNumberOfInlets() const;
    int getNumberOfOutlets() const;
    
private:
    void createInletsAndOutlets();
    void removeInletsAndOultets();
    
private:
    View* view_;
    core::Object object_;
    core::Cached<bool> selected_;
    core::Cached<bool> visible_;
    core::Cached<juce::String> inlets_;
    core::Cached<juce::String> outlets_;
    core::Cached<juce::String> label_;
    core::Cached<juce::Colour> boxPinBackgroundColour_;
    core::Cached<juce::Colour> boxSelectedColour_;
    std::unique_ptr<PainterPolicy> painter_;
    std::vector<std::unique_ptr<PinComponent>> iPins_;
    std::vector<std::unique_ptr<PinComponent>> oPins_;
    bool isRunView_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

