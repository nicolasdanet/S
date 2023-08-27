
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

RadioPainter::RadioPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    radioBackgroundColour_ (Spaghettis()->getCachedColour (Tag::ToggleBackground)),
    radioButtonsOffColour_ (Spaghettis()->getCachedColour (Tag::Toggle)),
    radioButtonsOnColour_ (Spaghettis()->getCachedColour (Tag::Toggle)),
    isVertical_ (object_.getCached<bool> (Tag::Parameters, Tag::Vertical)),
    buttons_ (object_.getCached<int> (Tag::Parameters, Tag::Buttons)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width))
{
    radioBackgroundColour_.attach (repaint (component_));
    radioButtonsOffColour_.attach (repaint (component_));
    radioButtonsOnColour_.attach (repaint (component_));
    isVertical_.attach (resized (component_));
    buttons_.attach (resized (component_));
    width_.attach (resized (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RadioPainter::mouseDown (const juce::MouseEvent& e)
{
    
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void RadioPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (radioBackgroundColour_.get());
    g.fillRect (r);
}

juce::Rectangle<int> RadioPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> RadioPainter::getDimensions()
{
    const int w = width_.get();
    const int h = w * buttons_.get();
    
    if (isVertical_.get()) { return core::Vector::Real (w, h); }
    else {
        return core::Vector::Real (h, w);
    }
}

void RadioPainter::setDimensions (core::Vector::Real v)
{
    // setDimensionsByParameters (v);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
