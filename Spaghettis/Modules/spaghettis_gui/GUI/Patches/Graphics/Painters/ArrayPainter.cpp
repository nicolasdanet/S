
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ArrayPainter::ArrayPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    name_ (object_.getCached<juce::String> (Tag::Parameters, Tag::Name)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width)),
    height_ (object_.getCached<int> (Tag::Parameters, Tag::Height)),
    size_ (object_.getCached<int> (Tag::Parameters, Tag::Size)),
    start_ (object_.getCached<int> (Tag::Parameters, Tag::Start)),
    end_ (object_.getCached<int> (Tag::Parameters, Tag::End)),
    low_ (object_.getCached<int> (Tag::Parameters, Tag::Low)),
    high_ (object_.getCached<int> (Tag::Parameters, Tag::High))
{
    width_.attach (resized (component_));
    height_.attach (resized (component_));
    
    name_.attach (repaint (component_));
    size_.attach (repaint (component_));
    start_.attach (repaint (component_));
    end_.attach (repaint (component_));
    low_.attach (repaint (component_));
    high_.attach (repaint (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ArrayPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    DBG ("?");
    
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
