
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BangPainter::BangPainter (juce::Component& owner, const core::Object& object) : PainterPolicy (owner, object),
    backgroundColour_ (getColour (Tags::BangBackground)),
    flashOffColour_ (getColour (Tags::BangFlashOff)),
    flashOnColour_ (getColour (Tags::BangFlashOn)),
    flashed_ (getParameter<bool> (Tags::Flashed)),
    width_ (getParameter<int> (Tags::Width))
{
    bind (backgroundColour_);
    bind (flashOffColour_);
    bind (flashOnColour_);
    bind (flashed_);
    bind (width_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BangPainter::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    g.setColour (backgroundColour_.get());
    g.fillRect (r);
    g.setColour (flashed_.get() ? flashOnColour_.get() : flashOffColour_.get());
    g.fillEllipse (r.reduced (1).toFloat());
}

juce::Rectangle<int> BangPainter::getBounds()
{
    const int x = x_.get();
    const int y = y_.get();
    const int w = width_.get();
    
    return juce::Rectangle<int> (x, y, w, w);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
