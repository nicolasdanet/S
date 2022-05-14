
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

core::Cached<juce::Colour> getColourFromType (const juce::String& type)
{
    juce::String key = Tags::PinSignal;
    
    if (type == "bang")          { key = Tags::PinBang;     }
    else if (type == "float")    { key = Tags::PinFloat;    }
    else if (type == "symbol")   { key = Tags::PinSymbol;   }
    else if (type == "list")     { key = Tags::PinList;     }
    else if (type == "anything") { key = Tags::PinAnything; }
            
    return Spaghettis()->getCachedColour (key);
}

bool isPinSignal (const juce::String& type)
{
    return (type == "signal");
}

juce::String getTooltipText (const juce::String& type)
{
    return type.substring (0, 1).toUpperCase() + type.substring (1);
}

juce::Rectangle<int> getBoundWithoutGrip (juce::Rectangle<int> r)
{
    return r.reduced (PainterPolicy::pinGripX(), PainterPolicy::pinGripY());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PinComponent::PinComponent (EditView* view, const juce::String& type) :
    view_ (view),
    pinColour_ (getColourFromType (type)),
    pinOverColour_ (Spaghettis()->getCachedColour (Tags::PinOver)),
    isSignal_ (isPinSignal (type)),
    isOver_ (false)
{
    setOpaque (false); setPaintingIsUnclipped (true);
    
    setTooltip (getTooltipText (type));
    
    pinColour_.attach (PainterPolicy::repaint (this));
    
    view_->addChildComponent (this);
}

PinComponent::~PinComponent()
{
    view_->removeChildComponent (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool PinComponent::isSignal() const
{
    return isSignal_;
}

juce::Rectangle<int> PinComponent::getPinBoundsInParent() const
{
    return getBoundWithoutGrip (getBoundsInParent());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PinComponent::paint (juce::Graphics& g)
{
    g.setColour (isOver_ ? pinOverColour_.get() : pinColour_.get());
    g.fillRect (getBoundWithoutGrip (getLocalBounds()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void PinComponent::mouseEnter (const juce::MouseEvent&)
{
    isOver_ = true;  repaint();
}

void PinComponent::mouseExit (const juce::MouseEvent&)
{
    isOver_ = false; repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
