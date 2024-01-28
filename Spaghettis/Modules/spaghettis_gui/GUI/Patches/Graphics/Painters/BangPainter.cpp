
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
    bangBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::BangBackground), getOwner())),
    bangFlashOffColour_ (Painted (Spaghettis()->getCachedColour (Tag::BangFlashOff), getOwner())),
    bangFlashOnColour_ (Painted (Spaghettis()->getCachedColour (Tag::BangFlashOn), getOwner())),
    flashed_ (Painted (getObject().getCached<bool> (Tag::Parameters, Tag::Flashed), getOwner())),
    width_ (Resized (getObject().getCached<int> (Tag::Parameters, Tag::Width), getOwner()))
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

void BangPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (bangBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (flashed_.get() ? bangFlashOnColour_.get() : bangFlashOffColour_.get());
    g.fillEllipse (r.reduced (1).toFloat());
}

juce::Rectangle<int> BangPainter::getRequiredBoundsForWidget()
{
    return PainterHelpers::getRequiredBoundsFromDimensions (*this);
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
    PainterHelpers::setDimensionsByParameters (*this, v);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
