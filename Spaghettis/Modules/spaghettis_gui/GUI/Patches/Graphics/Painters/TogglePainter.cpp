
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

TogglePainter::TogglePainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    toggleBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::ToggleBackground), component_)),
    toggleColour_ (Painted (Spaghettis()->getCachedColour (Tag::Toggle), component_)),
    state_ (Painted (object_.getCached<bool> (Tag::Parameters, Tag::State), component_)),
    width_ (Resized (object_.getCached<int> (Tag::Parameters, Tag::Width), component_))
{
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

void TogglePainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (toggleBackgroundColour_.get());
    g.fillRect (r);
    
    if (state_.get()) {
        g.setColour (toggleColour_.get()); LNF::drawCross (g, r.reduced (1));
    }
}

juce::Rectangle<int> TogglePainter::getRequiredBoundsForWidget()
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
