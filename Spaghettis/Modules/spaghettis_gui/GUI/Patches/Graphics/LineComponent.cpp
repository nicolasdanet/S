
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
    isSignal_ (false)
{
    setPaintingIsUnclipped (true);
    
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
    g.setColour (juce::Colours::orange); g.drawRect (getLocalBounds());
}

void LineComponent::changeListenerCallback (juce::ChangeBroadcaster* broadcaster)
{
    update();
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
    isSignal_   = outlet->isSignal() && inlet->isSignal();
    o_          = outlet->getHook();
    i_          = inlet->getHook();
    
    setBounds (o_.getUnion (i_)); isVisible = true;
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
