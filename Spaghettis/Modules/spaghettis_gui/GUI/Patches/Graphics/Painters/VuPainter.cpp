
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

VuPainter::VuPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    vuBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::VuBackground), getOwner())),
    vuBarColdColour_ (Painted (Spaghettis()->getCachedColour (Tag::VuBarCold), getOwner())),
    vuBarWarmColour_ (Painted (Spaghettis()->getCachedColour (Tag::VuBarWarm), getOwner())),
    vuBarHotColour_ (Painted (Spaghettis()->getCachedColour (Tag::VuBarHot), getOwner())),
    value_ (Painted (getObject().getCached<double> (Tag::Parameters, Tag::Value), getOwner())),
    peak_ (Painted (getObject().getCached<double> (Tag::Parameters, Tag::Peak), getOwner())),
    width_ (Resized (getObject().getCached<int> (Tag::Parameters, Tag::Width), getOwner())),
    height_ (Resized (getObject().getCached<int> (Tag::Parameters, Tag::Height), getOwner()))
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double getNormalizedValue (double f)
{
    return 1.0 - Normalized (false, VuPainter::rangeLow_, VuPainter::rangeHigh_).convert (f);
}

int getNormalizedPosition (const juce::Rectangle<int>& r, double f)
{
    const int h = static_cast<int> (r.getHeight() * getNormalizedValue (f));
    
    return h + r.getY();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ColourGradient VuPainter::getGradient (const juce::Rectangle<int>& r)
{
    const juce::Colour hot  (vuBarHotColour_.get());
    const juce::Colour warm (vuBarWarmColour_.get());
    const juce::Colour cold (vuBarColdColour_.get());
    
    juce::ColourGradient c (juce::ColourGradient::vertical (hot, cold, r));
    
    c.addColour (getNormalizedValue (  3.0), hot);
    c.addColour (getNormalizedValue (  0.0), warm);
    c.addColour (getNormalizedValue (-12.0), warm);
    c.addColour (getNormalizedValue (-15.0), cold);
    
    return c;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void VuPainter::paintBar (juce::Rectangle<int> r, const juce::ColourGradient& gradient, juce::Graphics& g)
{
    const double f = value_.get();
    
    if (f > rangeLow_) {
    //
    g.setGradientFill (gradient);
    g.fillRect (r.reduced (4.0f * getScale(), 0).withTop (getNormalizedPosition (r, f)));
    //
    }
}

void VuPainter::paintPeak (juce::Rectangle<int> r, const juce::ColourGradient& gradient, juce::Graphics& g)
{
    const double f = peak_.get();
    
    if (f > rangeLow_) {
    //
    g.setColour (gradient.getColourAtPosition (getNormalizedValue (f)));

    const juce::Rectangle<int> marker (r.getX(), getNormalizedPosition (r, f), r.getWidth(), 1);
    
    g.fillRect (marker.expanded (0, 1));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void VuPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    const juce::ColourGradient gradient (getGradient (r));
    
    g.setColour (vuBackgroundColour_.get());
    g.fillRect (r);
    
    paintBar (r.reduced (0, 2), gradient, g);
    paintPeak (r.reduced (1, 2), gradient, g);
}

juce::Rectangle<int> VuPainter::getRequiredBoundsForWidget()
{
    return getRequiredBoundsFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> VuPainter::getDimensions()
{
    return core::Vector::Real (width_.get(), height_.get());
}

void VuPainter::setDimensions (core::Vector::Real v)
{
    setDimensionsByParameters (v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
