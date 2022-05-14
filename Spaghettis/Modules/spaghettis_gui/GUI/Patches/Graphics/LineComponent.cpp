
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

LineComponent::LineComponent (EditView* view, const core::Line& line) :
    view_ (view),
    line_ (line),
    source_ (view->getObject (line.getIdentifierOfSource())),
    destination_ (view->getObject (line.getIdentifierOfDestination())),
    controlColour_ (Spaghettis()->getCachedColour (Tags::Line)),
    signalColour_ (Spaghettis()->getCachedColour (Tags::LineSignal)),
    isSignal_ (false),
    isOver_ (false)
{
    controlColour_.attach (PainterPolicy::repaint (this));
    signalColour_.attach (PainterPolicy::repaint (this));
    
    update();
    
    view_->addChildComponent (this);
    
    if (source_.getComponent())      { source_->addChangeListener (this);         }
    if (destination_.getComponent()) { destination_->addChangeListener (this);    }
}

LineComponent::~LineComponent()
{
    if (destination_.getComponent()) { destination_->removeChangeListener (this); }
    if (source_.getComponent())      { source_->removeChangeListener (this);      }
    
    view_->removeChildComponent (this);
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
    const juce::Colour c (isSignal_ ? signalColour_.get() : controlColour_.get());
    
    g.setColour (isOver_ ? c.contrasting (0.35) : c);
    
    g.fillPath (linePath_);
}

bool LineComponent::hitTest (int x, int y)
{
    return hitPath_.contains (juce::Point<float> (x, y));
}

void LineComponent::changeListenerCallback (juce::ChangeBroadcaster*)
{
    update(); repaint();
}

void LineComponent::mouseEnter (const juce::MouseEvent&)
{
    isOver_ = true;  repaint();
}

void LineComponent::mouseExit (const juce::MouseEvent&)
{
    isOver_ = false; repaint();
}

void LineComponent::scaleChanged()
{
    DBG (view_->getScale());
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
    /* Normal case, use the outside borders. */
    
    if (bounds.getHeight() > PainterPolicy::pinHeight() * 2) {
    //
    const float f = PainterPolicy::pinHeight() / 2.0f;
        
    if (iPin.getY() < oPin.getY()) { p1.addXY (0, -f); p2.addXY (0,  f); }
    else                           { p1.addXY (0,  f); p2.addXY (0, -f); }
    //
    }

    return std::make_tuple (p1, p2);
}

void makeLinePaths (juce::Point<float> p1, juce::Point<float> p2, juce::Path& line, juce::Path& hit)
{
    const juce::Point<float> controlPoint1 (p1.x, p1.y + (p2.y - p1.y) * 0.33f);
    const juce::Point<float> controlPoint2 (p2.x, p1.y + (p2.y - p1.y) * 0.66f);
    
    line.clear();
    line.startNewSubPath (p1);
    line.cubicTo (controlPoint1, controlPoint2, p2);

    const juce::PathStrokeType s1 (5.0f); s1.createStrokedPath (hit,  line);
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
    
    setVisible (isVisible);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
