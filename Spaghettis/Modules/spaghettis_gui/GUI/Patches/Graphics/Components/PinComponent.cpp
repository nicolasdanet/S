
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

data::Cached<juce::Colour> getColourFromType (const juce::String& type)
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PinComponent::PinComponent (PatchView* v,
    const core::Object& o,
    int i,
    const juce::String& type,
    bool isOutlet) :
        MouseHandler (v),
        object_ (o),
        selected_ (Painted (o.getCached<bool> (Tag::Attributes, Tag::Selected), this)),
        pinColour_ (Painted (getColourFromType (type), this)),
        boxSelectedColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxSelected), this)),
        index_ (i),
        isOutlet_ (isOutlet),
        isSignal_ (isPinSignal (type)),
        isOver_ (false)
{
    getView()->addChildComponent (this);
}

PinComponent::~PinComponent()
{
    getView()->removeChildComponent (this);
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int PinComponent::getIndex() const
{
    return index_;
}

core::UniqueId PinComponent::getIdentifier() const
{
    return object_.getIdentifier();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> getBoundWithoutGrip (juce::Rectangle<int> r, float scale)
{
    return r.reduced (Painter::pinGripX (scale), Painter::pinGripY (scale));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PinComponent::getPinBoundsInView() const
{
    return getBoundWithoutGrip (getBoundsInParent(), getView()->getScale());
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

void PinComponent::mouseDown (const juce::MouseEvent& e)
{
    if (auto view = getEditView()) { view->hideLocator (e); }
}

void PinComponent::mouseDrag (const juce::MouseEvent& e)
{
    forwardMouseDrag (e, nullptr, DragFlag::Pin);
}

void PinComponent::mouseUp (const juce::MouseEvent& e)
{
    forwardMouseUp (e, nullptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PinComponent::paint (juce::Graphics& g)
{
    if (isOver_)              { g.setColour (boxSelectedColour_.get()); }
    else if (selected_.get()) { g.setColour (boxSelectedColour_.get().contrasting (0.25f)); }
    else                      { g.setColour (pinColour_.get()); }
    
    g.fillRect (getBoundWithoutGrip (getLocalBounds(), getView()->getScale()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
