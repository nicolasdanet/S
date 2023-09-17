
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

SliderPainter::SliderPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    sliderBackgroundColour_ (Spaghettis()->getCachedColour (Tag::RadioBackground)),
    sliderBarColour_ (Spaghettis()->getCachedColour (Tag::RadioButton)),
    value_ (object_.getCached<double> (Tag::Parameters, Tag::Value)),
    isVertical_ (object_.getCached<bool> (Tag::Parameters, Tag::Vertical)),
    isLogarithmic_ (object_.getCached<bool> (Tag::Parameters, Tag::Logarithmic)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width)),
    height_ (object_.getCached<int> (Tag::Parameters, Tag::Height))
{
    sliderBackgroundColour_.attach (repaint (component_));
    sliderBarColour_.attach (repaint (component_));
    value_.attach (repaint (component_));
    isLogarithmic_.attach (repaint (component_));
    isVertical_.attach (resized (component_));
    width_.attach (resized (component_));
    height_.attach (resized (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SliderPainter::mouseDown (const juce::MouseEvent& e)
{
    
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SliderPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (sliderBackgroundColour_.get());
    g.fillRect (r);
}

juce::Rectangle<int> SliderPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> SliderPainter::getDimensions()
{
    return core::Vector::Real (width_.get(), height_.get());
}

void SliderPainter::setDimensions (core::Vector::Real v)
{
    setDimensionsByParameters (v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
