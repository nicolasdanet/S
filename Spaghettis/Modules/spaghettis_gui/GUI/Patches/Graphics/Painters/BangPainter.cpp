
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BangPainter::BangPainter (ObjectComponent* owner, const core::Object& object) : PainterPolicy (owner, object),
    bangBackgroundColour_ (Spaghettis()->getCachedColour (Tag::BangBackground)),
    bangFlashOffColour_ (Spaghettis()->getCachedColour (Tag::BangFlashOff)),
    bangFlashOnColour_ (Spaghettis()->getCachedColour (Tag::BangFlashOn)),
    flashed_ (object_.getCached<bool> (Tag::Parameters, Tag::Flashed)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width))
{
    bangBackgroundColour_.attach (repaint (component_));
    bangFlashOffColour_.attach (repaint (component_));
    bangFlashOnColour_.attach (repaint (component_));
    flashed_.attach (repaint (component_));
    width_.attach (resized (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BangPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (bangBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (flashed_.get() ? bangFlashOnColour_.get() : bangFlashOffColour_.get());
    g.fillEllipse (r.toFloat().reduced (1));
}

juce::Rectangle<int> BangPainter::getRequiredBoundsForObject()
{
    const int w = width_.get();
    
    const core::Vector::Scaled v (core::Vector::Real (w, w), getScale());
    const core::Point::Scaled pt (getPosition());
    
    return juce::Rectangle<int> (v.getPoint().getX(), v.getPoint().getY()) + pt.getPoint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> BangPainter::getDimensions()
{
    const int w = width_.get(); return core::Vector::Real (w, w);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
