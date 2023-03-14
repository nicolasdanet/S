
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

void BangPainter::paintObject (juce::Rectangle<float> r, juce::Graphics& g)
{
    const float w = getScaled (std::round (width_.get() / 15.0f));
    
    g.setColour (bangBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (flashed_.get() ? bangFlashOnColour_.get() : bangFlashOffColour_.get());
    g.fillEllipse (r.reduced (w));
}

juce::Rectangle<float> BangPainter::getRequiredBoundsForObject()
{
    const float w = getScaled (width_.get());
    
    return juce::Rectangle<float> (w, w) + getLocalPositionScaled();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<juce::Point<int>> BangPainter::getWidth()
{
    const int w = width_.get(); return juce::Point<int> (w, w);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
