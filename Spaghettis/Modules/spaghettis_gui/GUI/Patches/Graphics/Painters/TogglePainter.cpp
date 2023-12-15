
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

TogglePainter::TogglePainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    toggleBackgroundColour_ (Spaghettis()->getCachedColour (Tag::ToggleBackground)),
    toggleColour_ (Spaghettis()->getCachedColour (Tag::Toggle)),
    state_ (object_.getCached<bool> (Tag::Parameters, Tag::State)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width))
{
    toggleBackgroundColour_.attach (repaint (component_));
    toggleColour_.attach (repaint (component_));
    state_.attach (repaint (component_));
    width_.attach (resized (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void TogglePainter::mouseDown (const juce::MouseEvent& e)
{
    Spaghettis()->handle (Inputs::sendObjectBang (getIdentifier()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void TogglePainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (toggleBackgroundColour_.get());
    g.fillRect (r);
    
    if (state_.get()) {
        g.setColour (toggleColour_.get()); LNF::drawCross (g, r.reduced (1));
    }
}

juce::Rectangle<int> TogglePainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> TogglePainter::getDimensions()
{
    const int w = width_.get(); return core::Vector::Real (w, w);
}

void TogglePainter::setDimensions (core::Vector::Real v)
{
    setDimensionsByParameters (v);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
