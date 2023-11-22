
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

VuPainter::VuPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    vuBackgroundColour_ (Spaghettis()->getCachedColour (Tag::VuBackground)),
    vuBarColdColour_ (Spaghettis()->getCachedColour (Tag::VuBarCold)),
    vuBarWarmColour_ (Spaghettis()->getCachedColour (Tag::VuBarWarm)),
    vuBarHotColour_ (Spaghettis()->getCachedColour (Tag::VuBarHot)),
    value_ (object_.getCached<double> (Tag::Parameters, Tag::Value)),
    peak_ (object_.getCached<double> (Tag::Parameters, Tag::Peak)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width)),
    height_ (object_.getCached<int> (Tag::Parameters, Tag::Height))
{
    vuBackgroundColour_.attach (repaint (component_));
    vuBarColdColour_.attach (repaint (component_));
    vuBarWarmColour_.attach (repaint (component_));
    vuBarHotColour_.attach (repaint (component_));
    value_.attach (repaint (component_));
    peak_.attach (repaint (component_));
    width_.attach (resized (component_));
    height_.attach (resized (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double getNormalizedValue (double f)
{
    return 1.0 - Normalized (true, -100.0, 12.0).convert (f);
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
    
    //c.addColour (getNormalizedValue (0.0), warm);
    
    return c;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void VuPainter::paintBar (const juce::Rectangle<int>& r,
    const juce::ColourGradient& gradient,
    juce::Graphics& g)
{
    const int margins = static_cast<int> (4.0f * getScale());
    
    g.setGradientFill (gradient);
    g.fillRect (r.reduced (margins));
}

void VuPainter::paintPeak (const juce::Rectangle<int>& r,
    const juce::ColourGradient& gradient,
    juce::Graphics& g)
{
    const double f = peak_.get();
    
    juce::Rectangle<int> marker (r.getX(), getNormalizedPosition (r, f), r.getWidth(), 1);
    
    g.setColour (gradient.getColourAtPosition (getNormalizedValue (f)));
    
    g.fillRect (marker);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void VuPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    const juce::ColourGradient gradient (getGradient (r));
    
    g.setColour (vuBackgroundColour_.get());
    g.fillRect (r);
    
    // paintBar (r, gradient, g);
    paintPeak (r, gradient, g);
}

juce::Rectangle<int> VuPainter::getRequiredBoundsForObject()
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
