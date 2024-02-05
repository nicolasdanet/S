
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

auto getStraightLine (const juce::Rectangle<int>& bounds,
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
    
    if (bounds.getHeight() > Painter::pinHeight (f) * 2) {
    //
    const float h = Painter::pinHeight (f) / 2.0f;
        
    if (iPin.getY() < oPin.getY()) { p1.addXY (0, -h); p2.addXY (0,  h); }
    else                           { p1.addXY (0,  h); p2.addXY (0, -h); }
    //
    }
    
    return std::make_tuple (juce::Line<float> (p1, p2), juce::Line<float> (p1 + position, p2 + position));
}

void makeLinePaths (juce::Line<float> straight, juce::Path& line, juce::Path& hit, float f)
{
    const juce::Point<float> p1 (straight.getStart());
    const juce::Point<float> p2 (straight.getEnd());
    
    const juce::Point<float> controlPoint1 (p1.x, p1.y + (p2.y - p1.y) * 0.33f);
    const juce::Point<float> controlPoint2 (p2.x, p1.y + (p2.y - p1.y) * 0.66f);
    
    juce::Path path;
    
    path.clear();
    path.startNewSubPath (p1);
    path.cubicTo (controlPoint1, controlPoint2, p2);

    const juce::PathStrokeType s1 (7.5f * f); s1.createStrokedPath (hit,  path);
    const juce::PathStrokeType s2 (2.5f * f); s2.createStrokedPath (line, path);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
    
    PinComponent *outlet = source_->getOutletAt (line_.get<int> (Tag::Attributes, Tag::Outlet));
    PinComponent *inlet  = destination_->getInletAt (line_.get<int> (Tag::Attributes, Tag::Inlet));
    
    if (outlet && inlet) {
    //
    const float scale = getView()->getScale();
    
    const juce::Rectangle<int> iPin (inlet->getPinBoundsInView());
    const juce::Rectangle<int> oPin (outlet->getPinBoundsInView());
    const juce::Rectangle<int> bounds (oPin.getUnion (iPin));
    
    const auto [local, top] = getStraightLine (bounds, iPin, oPin, scale);
    
    makeLinePaths (local, linePath_, hitPath_, scale);
    
    straight_ = top;
    
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
