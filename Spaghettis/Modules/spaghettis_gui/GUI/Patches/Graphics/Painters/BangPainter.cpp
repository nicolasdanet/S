
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BangPainter::BangPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    bangBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::BangBackground), component_)),
    bangFlashOffColour_ (Painted (Spaghettis()->getCachedColour (Tag::BangFlashOff), component_)),
    bangFlashOnColour_ (Painted (Spaghettis()->getCachedColour (Tag::BangFlashOn), component_)),
    flashed_ (Painted (object_.getCached<bool> (Tag::Parameters, Tag::Flashed), component_)),
    width_ (Resized (object_.getCached<int> (Tag::Parameters, Tag::Width), component_))
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BangPainter::mouseDown (const juce::MouseEvent& e)
{
    Spaghettis()->handle (Inputs::sendObjectBang (getIdentifier()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BangPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (bangBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (flashed_.get() ? bangFlashOnColour_.get() : bangFlashOffColour_.get());
    g.fillEllipse (r.reduced (1).toFloat());
}

juce::Rectangle<int> BangPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> BangPainter::getDimensions()
{
    const int w = width_.get(); return core::Vector::Real (w, w);
}

void BangPainter::setDimensions (core::Vector::Real v)
{
    setDimensionsByParameters (v);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
