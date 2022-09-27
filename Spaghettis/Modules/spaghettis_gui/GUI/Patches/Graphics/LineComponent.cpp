
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

LineComponent::LineComponent (View* view, const core::Line& line) :
    view_ (view),
    line_ (line),
    source_ (view->getObjectComponent (line.getIdentifierOfSource())),
    destination_ (view->getObjectComponent (line.getIdentifierOfDestination())),
    lineColour_ (Spaghettis()->getCachedColour (Tags::Line)),
    lineSelectedColour_ (Spaghettis()->getCachedColour (Tags::LineSelected)),
    lineSignalColour_ (Spaghettis()->getCachedColour (Tags::LineSignal)),
    isSignal_ (false),
    isOver_ (false),
    isSelected_ (false)
{
    jassert (view);
    
    lineColour_.attach (PainterPolicy::repaint (this));
    lineSelectedColour_.attach (PainterPolicy::repaint (this));
    lineSignalColour_.attach (PainterPolicy::repaint (this));
    
    update();
    
    view_->addChildComponent (this);
    
    if (source_.getComponent())      { source_->addChangeListener (this);         }
    if (destination_.getComponent()) { destination_->addChangeListener (this);    }
    
    line_.addObserver (this);
    
    updateOrder();
}

LineComponent::~LineComponent()
{
    line_.removeObserver (this);
    
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
    const juce::Colour c (getLineColour());
    
    g.setColour (isOver_ ? (c.contrasting (isSignal_ ? 0.5f : 0.35f)) : c);
    g.fillPath (linePath_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::mouseDown (const juce::MouseEvent& e)
{
    view_->mouseDown (e);

    if (Mouse::isShiftClick (e))       { setSelected (!isSelected()); }
    else if (Mouse::isSimpleClick (e)) { setSelected (true); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::scaleChanged()
{
    update(); repaint();
}

float LineComponent::getScale() const
{
    return view_->getScale();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool LineComponent::isSelected() const
{
    return isSelected_;
}

void LineComponent::setSelected (bool isSelected)
{
    if (isSelected_ != isSelected) { isSelected_ = isSelected; repaint(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

auto getLineStartAndEnd (const juce::Rectangle<int>& bounds,
    const juce::Rectangle<int>& iPin,
    const juce::Rectangle<int>& oPin,
    float f)
{
    const juce::Point<float> position (bounds.getPosition().toFloat());

    juce::Point<float> p1 (oPin.toFloat().getCentre() - position);
    juce::Point<float> p2 (iPin.toFloat().getCentre() - position);
    
    /* If horizontally closed, use the centre of pins. */
    /* If the inlet is above the outlet, use the inside borders of pins. */
    /* Normal case, use the outside borders. */
    
    if (bounds.getHeight() > PainterPolicy::pinHeight (f) * 2) {
    //
    const float h = PainterPolicy::pinHeight (f) / 2.0f;
        
    if (iPin.getY() < oPin.getY()) { p1.addXY (0, -h); p2.addXY (0,  h); }
    else                           { p1.addXY (0,  h); p2.addXY (0, -h); }
    //
    }

    return std::make_tuple (p1, p2);
}

void makeLinePaths (juce::Point<float> p1, juce::Point<float> p2, juce::Path& line, juce::Path& hit, float f)
{
    const juce::Point<float> controlPoint1 (p1.x, p1.y + (p2.y - p1.y) * 0.33f);
    const juce::Point<float> controlPoint2 (p2.x, p1.y + (p2.y - p1.y) * 0.66f);
    
    line.clear();
    line.startNewSubPath (p1);
    line.cubicTo (controlPoint1, controlPoint2, p2);

    const juce::PathStrokeType s1 (5.0f * f); s1.createStrokedPath (hit,  line);
    const juce::PathStrokeType s2 (2.5f * f); s2.createStrokedPath (line, line);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::parameterHasChanged()
{
    update(); repaint();
}

void LineComponent::updateOrder()
{
    juce::Component* c = destination_.getComponent(); if (c) { toBehind (c); }
}

void LineComponent::update()
{
    bool isVisible = false;
    
    if (source_.getComponent() && destination_.getComponent()) {
    //
    jassert (source_->getIdentifier()      == line_.getIdentifierOfSource());
    jassert (destination_->getIdentifier() == line_.getIdentifierOfDestination());
    
    PinComponent *outlet = source_->getOutletAt (line_.get<int> (Tags::Attributes, Tags::Outlet));
    PinComponent *inlet  = destination_->getInletAt (line_.get<int> (Tags::Attributes, Tags::Inlet));
    
    if (outlet && inlet) {
    //
    const float scale = getScale();
    
    const juce::Rectangle<int> iPin (inlet->getPinBoundsInView());
    const juce::Rectangle<int> oPin (outlet->getPinBoundsInView());
    const juce::Rectangle<int> bounds (oPin.getUnion (iPin));
    
    const auto [start, end] = getLineStartAndEnd (bounds, iPin, oPin, scale);
    
    makeLinePaths (start, end, linePath_, hitPath_, scale);
    
    isSignal_ = outlet->isSignal() && inlet->isSignal();
    
    setBounds (bounds); isVisible = true;
    //
    }
    //
    }
    
    setVisible (isVisible);
}

juce::Colour LineComponent::getLineColour() const
{
    if (isSelected())   { return lineSelectedColour_.get(); }
    else if (isSignal_) { return lineSignalColour_.get(); }
    else {
        return lineColour_.get();
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
