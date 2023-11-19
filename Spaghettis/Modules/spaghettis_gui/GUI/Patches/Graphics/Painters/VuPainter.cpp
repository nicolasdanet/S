
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
    vuBackgroundColour_ (Spaghettis()->getCachedColour (Tag::SliderBackground)),
    vuBarColour_ (Spaghettis()->getCachedColour (Tag::SliderBar)),
    value_ (object_.getCached<double> (Tag::Parameters, Tag::Value)),
    peak_ (object_.getCached<double> (Tag::Parameters, Tag::Peak)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width)),
    height_ (object_.getCached<int> (Tag::Parameters, Tag::Height))
{
    vuBackgroundColour_.attach (repaint (component_));
    vuBarColour_.attach (repaint (component_));
    value_.attach (repaint (component_));
    peak_.attach (repaint (component_));
    width_.attach (resized (component_));
    height_.attach (resized (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void VuPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (vuBackgroundColour_.get());
    g.fillRect (r);
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
