
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BangPainter::BangPainter (ObjectComponent* owner, const core::Object& object) : PainterPolicy (owner, object),
    bangBackgroundColour_ (Spaghettis()->getCachedColour (Tags::BangBackground)),
    bangFlashOffColour_ (Spaghettis()->getCachedColour (Tags::BangFlashOff)),
    bangFlashOnColour_ (Spaghettis()->getCachedColour (Tags::BangFlashOn)),
    flashed_ (fetchParameter<bool> (Tags::Flashed)),
    width_ (fetchParameter<int> (Tags::Width))
{
    bangBackgroundColour_.attach (repaint (owner_));
    bangFlashOffColour_.attach (repaint (owner_));
    bangFlashOnColour_.attach (repaint (owner_));
    flashed_.attach (repaint (owner_));
    width_.attach (repaint (owner_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BangPainter::paintObject (juce::Rectangle<float> r, juce::Graphics& g)
{
    g.setColour (bangBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (flashed_.get() ? bangFlashOnColour_.get() : bangFlashOffColour_.get());
    g.fillEllipse (r.reduced (scaled (std::round (width_.get() / 15.0f), getScale())));
}

juce::Rectangle<float> BangPainter::getRequiredBoundsForObject()
{
    const float f = getScale();
    const float x = scaled (x_.get(), f);
    const float y = scaled (y_.get(), f);
    const float w = scaled (width_.get(), f);
    
    return juce::Rectangle<float> (x, y, w, w);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
