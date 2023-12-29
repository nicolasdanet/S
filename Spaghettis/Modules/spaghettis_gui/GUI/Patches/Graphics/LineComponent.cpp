
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

LineComponent::LineComponent (PatchView* view, const core::Line& line) :
    DragHandler (view),
    line_ (line),
    source_ (view->getObjectComponent (line.getIdentifierOfSource())),
    destination_ (view->getObjectComponent (line.getIdentifierOfDestination())),
    selected_ (line.getCached<bool> (Tag::Attributes, Tag::Selected)),
    lineColour_ (Spaghettis()->getCachedColour (Tag::Line)),
    lineSelectedColour_ (Spaghettis()->getCachedColour (Tag::LineSelected)),
    lineSignalColour_ (Spaghettis()->getCachedColour (Tag::LineSignal)),
    isSignal_ (false),
    isOver_ (false),
    isLocked_ (line_.isLocked())
{
    jassert (view);
    
    lineColour_.attach (PainterPolicy::repaint (this));
    lineSelectedColour_.attach (PainterPolicy::repaint (this));
    lineSignalColour_.attach (PainterPolicy::repaint (this));
    
    update();
    
    getView()->addChildComponent (this);
    
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
    
    getView()->removeChildComponent (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool LineComponent::isLocked() const
{
    return isLocked_;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::disconnect() const
{
    if (!isLocked()) {
    //
    const core::UniqueId u = line_.getIdentifierOfSource();
    const core::UniqueId v = line_.getIdentifierOfDestination();
    const int m = line_.get<int> (Tag::Attributes, Tag::Outlet);
    const int n = line_.get<int> (Tag::Attributes, Tag::Inlet);
    
    EditCommands::disconnect (u, m, v, n);
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::UniqueId LineComponent::getIdentifier() const
{
    return line_.getIdentifier();
}

core::Line LineComponent::getLine() const
{
    return line_;
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

bool LineComponent::intersects (const juce::Rectangle<float>& r) const
{
    return r.intersects (straight_);
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
    if (auto view = getEditView()) {
    //
    view->handleMouseDown (e);

    if (Mouse::isShiftClick (e))       { setSelected (!isSelected()); }
    else if (Mouse::isSimpleClick (e)) { setSelected (true); }
    //
    }
}

void LineComponent::mouseDrag (const juce::MouseEvent& e)
{
    forwardMouseDrag (e, nullptr, DragFlag::None);
}

void LineComponent::mouseUp (const juce::MouseEvent& e)
{
    forwardMouseUp (e, nullptr);
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
    return getView()->getScale();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool LineComponent::isSelected() const
{
    return selected_.get();
}

void LineComponent::setSelected (bool selected)
{
    if (selected != isSelected()) { selected_.set (selected); Spaghettis()->updateMenuBar(); }
}

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
    
    if (bounds.getHeight() > PainterPolicy::pinHeight (f) * 2) {
    //
    const float h = PainterPolicy::pinHeight (f) / 2.0f;
        
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

void LineComponent::parameterHasChanged (const core::Group&, const core::Parameter& p)
{
    if (p.getKey() != Tag::Selected) { update(); }
    
    repaint();
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
    
    PinComponent *outlet = source_->getOutletAt (line_.get<int> (Tag::Attributes, Tag::Outlet));
    PinComponent *inlet  = destination_->getInletAt (line_.get<int> (Tag::Attributes, Tag::Inlet));
    
    if (outlet && inlet) {
    //
    const float scale = getScale();
    
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
