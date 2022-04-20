
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

juce::String getTooltipText (const juce::String& type)
{
    return type.substring (0, 1).toUpperCase() + type.substring (1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PinComponent::PinComponent (juce::Component& owner, const juce::String& type, int index) :
    owner_ (owner),
    type_ (type),
    index_ (index),
    pin_ (getColourFromType (type)),
    pinOver_ (Spaghettis()->getCachedColour (Tags::PinOver)),
    isOver_ (false)
{
    setOpaque (true); setPaintingIsUnclipped (true);
    
    setTooltip (getTooltipText (type_));
    
    pin_.attach (PainterPolicy::repainter (this));
    
    owner_.addChildComponent (this);
}

PinComponent::~PinComponent()
{
    owner_.removeChildComponent (this);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PinComponent::paint (juce::Graphics& g)
{
    g.setColour (isOver_ ? pinOver_.get() : pin_.get());
    g.fillRect (getLocalBounds());
}
    
void PinComponent::resized()
{

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
