
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class PinComponent;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ObjectComponent : public juce::Component,
                        public juce::ChangeBroadcaster,
                        public juce::SettableTooltipClient,
                        public MouseHandler,
                        public Scalable {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ObjectComponent (PatchView*, const core::Object&);
    
    ~ObjectComponent();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool isInsideRunView() const;
    
public:
    bool isLocked() const;
    bool isPatch() const;
    bool isAbstraction() const;
    bool isGraphic() const;
    bool isSelected() const;
    bool isIncluded() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setSelected (bool);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    core::UniqueId getIdentifier() const;
    core::Object getObject() const;
    core::Point::Real getPosition() const;
    int getNumberOfInlets() const;
    int getNumberOfOutlets() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getHelpName() const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    PinComponent* getInletAt (int) const;
    PinComponent* getOutletAt (int) const;
    
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
    void dragStart();
    void drag (core::Vector::Real);
    void resize (core::Vector::Real);

public:
    void moveBehind (juce::Component*);
    void moveBack();
    void moveFront();
    void snap();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics&) override;
    void resized() override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void scaleChanged() override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void updateLabel();
    void updateTooltip();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void update (bool notify = true);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void createInletsAndOutlets();
    void removeInletsAndOultets();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    bool canResize (const juce::MouseEvent&) const;
    
private:
    core::Object object_;
    data::Cached<int> x_;
    data::Cached<int> y_;
    data::Cached<bool> selected_;
    data::Cached<bool> visible_;
    data::Cached<juce::String> inlets_;
    data::Cached<juce::String> outlets_;
    data::Cached<bool> included_;
    data::Cached<juce::String> label_;
    std::unique_ptr<PainterStrategy> painter_;
    std::vector<std::unique_ptr<PinComponent>> iPins_;
    std::vector<std::unique_ptr<PinComponent>> oPins_;
    core::Point::Real origin_;
    std::optional<core::Vector::Real> dimensions_;
    bool hasResize_;
    bool isLocked_;
    bool isInsideRunView_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

