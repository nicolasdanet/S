
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

ObjectComponent* getSource (juce::Component& owner, const core::Line& line)
{
    EditView* view = dynamic_cast<EditView*> (&owner);
    
    jassert (view);
    
    return view->getObject (line.getSource());
}

ObjectComponent* getDestination (juce::Component& owner, const core::Line& line)
{
    EditView* view = dynamic_cast<EditView*> (&owner);
    
    jassert (view);
    
    return view->getObject (line.getDestination());
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
    source_ (getSource (owner, line)),
    destination_ (getDestination (owner, line))
{
    setPaintingIsUnclipped (true);
    
    if (source_.getComponent())      { source_->addChangeListener (this);      }
    if (destination_.getComponent()) { destination_->addChangeListener (this); }
    
    update();
    
    // owner_.addAndMakeVisible (this);
}

LineComponent::~LineComponent()
{
    // owner_.removeChildComponent (this);
    
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
    g.setColour (juce::Colours::orange); g.fillRect (getLocalBounds());
}

void LineComponent::changeListenerCallback (juce::ChangeBroadcaster* broadcaster)
{
    DBG ("?"); update(); repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void LineComponent::update()
{
    if (source_.getComponent() && destination_.getComponent()) {
    //
    PinComponent *outlet = source_->getOutletAt (line_.getOutlet());
    PinComponent *inlet  = destination_->getInletAt (line_.getInlet());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
