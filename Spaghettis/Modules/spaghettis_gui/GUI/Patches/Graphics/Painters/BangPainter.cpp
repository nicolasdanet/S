
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
    bangBackgroundColour_ (Spaghettis()->getCachedColour (Tag::BangBackground)),
    bangFlashOffColour_ (Spaghettis()->getCachedColour (Tag::BangFlashOff)),
    bangFlashOnColour_ (Spaghettis()->getCachedColour (Tag::BangFlashOn)),
    flashed_ (object_.getCached<bool> (Tag::Parameters, Tag::Flashed)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width))
{
    bangBackgroundColour_.attach (Painter::repaint (component_));
    bangFlashOffColour_.attach (Painter::repaint (component_));
    bangFlashOnColour_.attach (Painter::repaint (component_));
    flashed_.attach (Painter::repaint (component_));
    width_.attach (Painter::resized (component_));
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
