
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ObjectComponent : public juce::Component {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ObjectComponent (juce::Component&, const core::Object&);
    
    ~ObjectComponent();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    core::UniqueId getIdentifier() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void update();
    void updateBounds (const juce::Rectangle<int>&);
    juce::Rectangle<int> getPainted (const juce::Rectangle<int>&) const;
    void updateInletsAndOutlets();
    void removeInletsAndOultets();
    
    
private:
    juce::Component& owner_;
    core::Object object_;
    core::Cached<bool> visible_;
    core::Cached<juce::String> inlets_;
    core::Cached<juce::String> outlets_;
    core::Cached<juce::Colour> backgroundColour_;
    std::unique_ptr<PainterPolicy> painter_;
    bool showPins_;

private:
    constexpr static int pinsHeight_ = 2;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

