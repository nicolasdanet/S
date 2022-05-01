
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

ObjectComponent* getSourceComponent (juce::Component& owner, const core::Line& line)
{
    EditView* view = dynamic_cast<EditView*> (&owner);
    
    jassert (view);
    
    return view->getObject (line.getIdentifierOfSource());
}

ObjectComponent* getDestinationComponent (juce::Component& owner, const core::Line& line)
{
    EditView* view = dynamic_cast<EditView*> (&owner);
    
    jassert (view);
    
    return view->getObject (line.getIdentifierOfDestination());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

LineComponent::LineComponent (juce::Component& owner, const core::Line& line) :
    owner_ (owner),
    line_ (line),
    source_ (getSourceComponent (owner, line)),
    destination_ (getDestinationComponent (owner, line)),
    control_ (Spaghettis()->getCachedColour (Tags::Line)),
    signal_ (Spaghettis()->getCachedColour (Tags::LineSignal)),
    isSignal_ (false)
{
    control_.attach (PainterPolicy::repainter (this));
    signal_.attach (PainterPolicy::repainter (this));
    
    if (source_.getComponent())      { source_->addChangeListener (this);      }
    if (destination_.getComponent()) { destination_->addChangeListener (this); }
    
    update();
    
    owner_.addChildComponent (this);
}

LineComponent::~LineComponent()
{
    owner_.removeChildComponent (this);
    
    if (destination_.getComponent()) { destination_->removeChangeListener (this); }
    if (source_.getComponent())      { source_->removeChangeListener (this);      }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::UniqueId LineComponent::getIdentifier() const
{
    return line_.getIdentifier();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::paint (juce::Graphics& g)
{
    g.setColour (isSignal_ ? signal_.get() : control_.get());
    g.fillPath (linePath_);
}

void LineComponent::changeListenerCallback (juce::ChangeBroadcaster* broadcaster)
{
    update();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

auto getLineStartAndEnd (const juce::Rectangle<int>& bounds,
    const juce::Rectangle<int>& iPin,
    const juce::Rectangle<int>& oPin)
{
    const juce::Point<float> position (bounds.getPosition().toFloat());

    juce::Point<float> p1 (oPin.toFloat().getCentre() - position);
    juce::Point<float> p2 (iPin.toFloat().getCentre() - position);
    
    /* If horizontally closed, use the centre of pins. */
    /* If the inlet is above the outlet, use the inside borders of pins. */
    
    const float f = PainterPolicy::pinHeight() / 2.0f;
    
    if (bounds.getHeight() <= PainterPolicy::pinHeight() * 2) { return std::make_tuple (p1, p2); }
    else if (iPin.getY() < oPin.getY()) {
        return std::make_tuple (p1.translated (0, -f), p2.translated (0, f));
    }

    /* Normal case, use the outside borders. */
    
    return std::make_tuple (p1.translated (0, f), p2.translated (0, -f));
}

void makeLinePaths (juce::Point<float> p1, juce::Point<float> p2, juce::Path& line, juce::Path& hit)
{
    const juce::Point<float> controlPoint1 (p1.x, p1.y + (p2.y - p1.y) * 0.33f);
    const juce::Point<float> controlPoint2 (p2.x, p1.y + (p2.y - p1.y) * 0.66f);
    
    line.clear();
    line.startNewSubPath (p1);
    line.cubicTo (controlPoint1, controlPoint2, p2);

    const juce::PathStrokeType s1 (8.0f); s1.createStrokedPath (hit, line);
    const juce::PathStrokeType s2 (2.5f); s2.createStrokedPath (line, line);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::update()
{
    bool isVisible = false;
    
    if (source_.getComponent() && destination_.getComponent()) {
    //
    PinComponent *outlet = source_->getOutletAt (line_.getOutlet());
    PinComponent *inlet  = destination_->getInletAt (line_.getInlet());
    
    if (outlet && inlet) {
    //
    const juce::Rectangle<int> iPin (inlet->getPinBoundsInParent());
    const juce::Rectangle<int> oPin (outlet->getPinBoundsInParent());
    const juce::Rectangle<int> bounds (oPin.getUnion (iPin));
    
    const auto [start, end] = getLineStartAndEnd (bounds, iPin, oPin);
    
    makeLinePaths (start, end, linePath_, hitPath_);
    
    isSignal_ = outlet->isSignal() && inlet->isSignal();
    
    setBounds (bounds); isVisible = true;
    //
    }
    //
    }
    
    setVisible (isVisible); repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
