
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

PinComponent::PinComponent (View* view, const core::Object& object, const juce::String& type) :
    view_ (view),
    selected_ (object.getCached<bool> (Tags::Attributes, Tags::Selected)),
    pinColour_ (getColourFromType (type)),
    pinOverColour_ (Spaghettis()->getCachedColour (Tags::PinOver)),
    boxSelectedColour_ (Spaghettis()->getCachedColour (Tags::BoxSelected)),
    isSignal_ (isPinSignal (type)),
    isOver_ (false)
{
    setOpaque (false); setPaintingIsUnclipped (true);
    
    setTooltip (getTooltipText (type));
    
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
    juce::Colour c (selected_.get() ? boxSelectedColour_.get().contrasting (0.25f) : pinColour_.get());
        
    g.setColour (isOver_ ? pinOverColour_.get() : c);
    
    g.fillRect (getBoundWithoutGrip (getLocalBounds(), getScale()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
