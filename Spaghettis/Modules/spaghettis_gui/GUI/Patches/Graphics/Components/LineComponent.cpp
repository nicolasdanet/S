
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

LineComponent::LineComponent (PatchView* view, const core::Line& line) :
    MouseHandler (view),
    line_ (line),
    source_ (view->getObjectComponent (line.getIdentifierOfSource())),
    destination_ (view->getObjectComponent (line.getIdentifierOfDestination())),
    selected_ (line.getCached<bool> (Tag::Attributes, Tag::Selected)),
    lineColour_ (Painted (Spaghettis()->getCachedColour (Tag::Line), this)),
    lineSelectedColour_ (Painted (Spaghettis()->getCachedColour (Tag::LineSelected), this)),
    lineSignalColour_ (Painted (Spaghettis()->getCachedColour (Tag::LineSignal), this)),
    isSignal_ (false),
    isOver_ (false),
    isLocked_ (line_.isLocked())
{
    jassert (view);
    
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

bool LineComponent::isSelected() const
{
    return selected_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::setSelected (bool selected)
{
    if (selected != isSelected()) { selected_.set (selected); Spaghettis()->updateMenuBar(); }
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

void LineComponent::disconnect() const
{
    if (!isLocked()) {
    //
    const core::UniqueId u = line_.getIdentifierOfSource();
    const core::UniqueId v = line_.getIdentifierOfDestination();
    const int m = line_.get<int> (Tag::Attributes, Tag::Outlet);
    const int n = line_.get<int> (Tag::Attributes, Tag::Inlet);
    
    Broadcast::disconnect (u, m, v, n);
    //
    }
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

bool LineComponent::intersects (const juce::Rectangle<float>& r) const
{
    return r.intersects (straight_);
}

bool LineComponent::hitTest (int x, int y)
{
    return hitPath_.contains (juce::Point<float> (x, y));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::paint (juce::Graphics& g)
{
    const juce::Colour c = [&]()
        {
            if (isSelected())   { return lineSelectedColour_.get(); }
            else if (isSignal_) { return lineSignalColour_.get(); }
            else {
                return lineColour_.get();
            }
        }();
    
    g.setColour (isOver_ ? (c.contrasting (isSignal_ ? 0.5f : 0.35f)) : c);
    g.fillPath (linePath_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::changeListenerCallback (juce::ChangeBroadcaster*)
{
    update(); repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::scaleChanged()
{
    update(); repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::parameterHasChanged (const data::Group&, const data::Parameter& p)
{
    if (p.getKey() != Tag::Selected) { update(); }
    
    repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
