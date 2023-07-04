
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

//Tag::Name
//Tag::Size
//Tag::Start
//Tag::End
//Tag::Low
//Tag::High
    
ArrayPainter::ArrayPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width)),
    height_ (object_.getCached<int> (Tag::Parameters, Tag::Height))
{
    width_.attach (resized (component_));
    height_.attach (resized (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ArrayPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (juce::Colours::black);
    g.fillRect (r);
}

juce::Rectangle<int> ArrayPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> ArrayPainter::getDimensions()
{
    const int w = width_.get();
    const int h = height_.get();
    
    return core::Vector::Real (w, h);
}

void ArrayPainter::setDimensions (core::Vector::Real v)
{
    setDimensionsByParameters (v);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
