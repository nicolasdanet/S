
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BangPainter::BangPainter (juce::Component& owner, const core::Object& object) : PainterPolicy (owner, object),
    background_ (Spaghettis()->getCachedColour (Tags::BangBackground)),
    flashOff_ (Spaghettis()->getCachedColour (Tags::BangFlashOff)),
    flashOn_ (Spaghettis()->getCachedColour (Tags::BangFlashOn)),
    flashed_ (fetchParameter<bool> (Tags::Flashed)),
    width_ (fetchParameter<int> (Tags::Width))
{
    bind (background_);
    bind (flashOff_);
    bind (flashOn_);
    bind (flashed_);
    bind (width_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BangPainter::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    g.setColour (background_.get());
    g.fillRect (r);
    g.setColour (flashed_.get() ? flashOn_.get() : flashOff_.get());
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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
