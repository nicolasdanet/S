
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
    juce::String key = Tag::PinSignal;
    
    if (type == "bang")          { key = Tag::PinBang;     }
    else if (type == "float")    { key = Tag::PinFloat;    }
    else if (type == "symbol")   { key = Tag::PinSymbol;   }
    else if (type == "list")     { key = Tag::PinList;     }
    else if (type == "anything") { key = Tag::PinAnything; }
            
    return Spaghettis()->getCachedColour (key);
}

bool isPinSignal (const juce::String& type)
{
    return (type == "signal");
}

juce::Rectangle<int> getBoundWithoutGrip (juce::Rectangle<int> r, float scale)
{
    return r.reduced (PainterPolicy::pinGripX (scale), PainterPolicy::pinGripY (scale));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PinComponent::PinComponent (View* view, const core::Object& object, const juce::String& type, bool isOutlet) :
    DragWatcher (view),
    view_ (view),
    selected_ (object.getCached<bool> (Tag::Attributes, Tag::Selected)),
    pinColour_ (getColourFromType (type)),
    pinOverColour_ (Spaghettis()->getCachedColour (Tag::PinOver)),
    boxSelectedColour_ (Spaghettis()->getCachedColour (Tag::BoxSelected)),
    isOutlet_ (isOutlet),
    isSignal_ (isPinSignal (type)),
    isOver_ (false)
{
    setOpaque (false); setPaintingIsUnclipped (true);
    
    selected_.attach (PainterPolicy::repaint (this));
    pinColour_.attach (PainterPolicy::repaint (this));
    boxSelectedColour_.attach (PainterPolicy::repaint (this));
    
    view_->addChildComponent (this);
}

PinComponent::~PinComponent()
{
    view_->removeChildComponent (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool PinComponent::isOutlet() const
{
    return isOutlet_;
}

bool PinComponent::isSignal() const
{
    return isSignal_;
}

juce::Rectangle<int> PinComponent::getPinBoundsInView() const
{
    return getBoundWithoutGrip (getBoundsInParent(), getScale());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

float PinComponent::getScale() const
{
    return view_->getScale();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PinComponent::paint (juce::Graphics& g)
{
    const juce::Colour c (selected_.get() ? boxSelectedColour_.get().contrasting (0.25f) : pinColour_.get());
        
    g.setColour (isOver_ ? pinOverColour_.get() : c);
    
    g.fillRect (getBoundWithoutGrip (getLocalBounds(), getScale()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PinComponent::setHighlighted (bool isHighlighted)
{
    if (isOver_ != isHighlighted) { isOver_ = isHighlighted; repaint(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PinComponent::mouseEnter (const juce::MouseEvent&)
{
    setHighlighted (true);
}

void PinComponent::mouseExit (const juce::MouseEvent&)
{
    setHighlighted (false);
}

void PinComponent::mouseDrag (const juce::MouseEvent& e)
{
    handleMouseDrag (e);
}

void PinComponent::mouseUp (const juce::MouseEvent& e)
{
    handleMouseUp (e);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
