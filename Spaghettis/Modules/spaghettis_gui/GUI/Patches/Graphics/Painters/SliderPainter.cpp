
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

SliderPainter::SliderPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    sliderBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::SliderBackground), getOwner())),
    sliderBarColour_ (Painted (Spaghettis()->getCachedColour (Tag::SliderBar), getOwner())),
    value_ (Painted (getObject().getCached<double> (Tag::Parameters, Tag::Value), getOwner())),
    low_ (Painted (getObject().getCached<double> (Tag::Parameters, Tag::Low), getOwner())),
    high_ (Painted (getObject().getCached<double> (Tag::Parameters, Tag::High), getOwner())),
    interval_ (Painted (getObject().getCached<double> (Tag::Parameters, Tag::Interval), getOwner())),
    isVertical_ (Resized (getObject().getCached<bool> (Tag::Parameters, Tag::Vertical), getOwner())),
    isLogarithmic_ (Painted (getObject().getCached<bool> (Tag::Parameters, Tag::Logarithmic), getOwner())),
    width_ (Resized (getObject().getCached<int> (Tag::Parameters, Tag::Width), getOwner())),
    height_ (Resized (getObject().getCached<int> (Tag::Parameters, Tag::Height), getOwner())),
    painted_(),
    dragged_ (false)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SliderPainter::mouseDown (const juce::MouseEvent& e)
{
    const juce::Point<int> pt (e.getMouseDownPosition());
    
    if (painted_.contains (pt)) { dragged_ = true; mouseProceed (pt); }
}

void SliderPainter::mouseDrag (const juce::MouseEvent& e)
{
    if (dragged_) { mouseProceed (e.getMouseDownPosition() + e.getOffsetFromDragStart()); }
}

void SliderPainter::mouseUp (const juce::MouseEvent&)
{
    dragged_ = false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double getProportionalPosition (juce::Rectangle<float> r, juce::Point<float> pt, bool isVertical)
{
    const juce::Point<float> a = r.getBottomLeft();
    const juce::Point<float> b = isVertical ? r.getTopLeft() : r.getBottomRight();
    
    return juce::Line<float> (a, b).findNearestProportionalPositionTo (pt);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SliderPainter::mouseProceed (juce::Point<int> pt)
{
    const double f = getProportionalPosition (painted_.toFloat(), pt.toFloat(), isVertical_.get());
    const double v = Normalized (isLogarithmic_.get(), low_.get(), high_.get(), interval_.get()).map (f);
        
    Spaghettis()->handle (Inputs::sendObjectFloat (getIdentifier(), v));
}

float SliderPainter::getNormalizedValue() const
{
    return Normalized (isLogarithmic_.get(), low_.get(), high_.get(), interval_.get()).convert (value_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SliderPainter::paintBar (juce::Rectangle<int> r, juce::Graphics& g)
{
    const bool vertical = isVertical_.get();
    const float v = getNormalizedValue();
    const float f = vertical ? r.proportionOfHeight (v) : r.proportionOfWidth (v);
    const juce::Rectangle<int> t (vertical ? r.removeFromBottom (f) : r.removeFromLeft (f));
    
    g.fillRect (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void SliderPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (sliderBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (sliderBarColour_.get());
    
    painted_ = r.reduced (2);
    
    paintBar (painted_, g);
}

juce::Rectangle<int> SliderPainter::getRequiredBoundsForWidget()
{
    return PainterHelpers::getRequiredBoundsFromDimensions (*this);
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
    PainterHelpers::setDimensionsByParameters (*this, v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
