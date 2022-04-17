
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void update();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void updateBounds();
    juce::Rectangle<int> getPaintedBounds() const;
    juce::Rectangle<int> getInletBounds (int) const;
    juce::Rectangle<int> getOutletBounds (int) const;

private:
    std::vector<std::unique_ptr<PinComponent>> updatePins (const juce::StringArray&, bool);
    void updateInlets (const juce::StringArray&);
    void updateOutlets (const juce::StringArray&);
    void updateInletsAndOutlets();
    void removeInletsAndOultets();
    
private:
    juce::Component& owner_;
    core::Object object_;
    core::Cached<bool> visible_;
    core::Cached<juce::String> inlets_;
    core::Cached<juce::String> outlets_;
    core::Cached<juce::Colour> background_;
    std::unique_ptr<PainterPolicy> painter_;
    std::vector<std::unique_ptr<PinComponent>> iPins_;
    std::vector<std::unique_ptr<PinComponent>> oPins_;
    bool showPins_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

