
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

DialPainter::DialPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    dialBackgroundColour_ (Spaghettis()->getCachedColour (Tag::DialBackground)),
    dialForegroundColour_ (Spaghettis()->getCachedColour (Tag::DialForeground)),
    dialNeedleColour_ (Spaghettis()->getCachedColour (Tag::DialNeedle)),
    dialTextColour_ (Spaghettis()->getCachedColour (Tag::DialText)),
    value_ (object_.getCached<double> (Tag::Parameters, Tag::Value)),
    low_ (object_.getCached<double> (Tag::Parameters, Tag::Low)),
    high_ (object_.getCached<double> (Tag::Parameters, Tag::High)),
    interval_ (object_.getCached<double> (Tag::Parameters, Tag::Interval)),
    isLogarithmic_ (object_.getCached<bool> (Tag::Parameters, Tag::Logarithmic)),
    width_ (object_.getCached<int> (Tag::Parameters, Tag::Width)),
    digits_ (object_.getCached<int> (Tag::Parameters, Tag::Digits)),
    dragged_ (false)
{
    dialBackgroundColour_.attach (repaint (component_));
    dialForegroundColour_.attach (repaint (component_));
    dialNeedleColour_.attach (repaint (component_));
    dialTextColour_.attach (repaint (component_));
    value_.attach (repaint (component_));
    low_.attach (repaint (component_));
    high_.attach (repaint (component_));
    interval_.attach (repaint (component_));
    isLogarithmic_.attach (repaint (component_));
    width_.attach (resized (component_));
    digits_.attach (repaint (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::mouseDown (const juce::MouseEvent& e)
{
    dragged_ = true;
}

void DialPainter::mouseDrag (const juce::MouseEvent& e)
{

}

void DialPainter::mouseUp (const juce::MouseEvent&)
{
    dragged_ = false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font DialPainter::getFont (int h) const
{
    return Spaghettis()->getLookAndFeel().getDialsFontWithHeight (h);
}

juce::String DialPainter::getText() const
{
    return Helpers::withNumberOfDigitsTruncated (value_.get(), digits_.get());
}

float DialPainter::getNormalizedValue() const
{
    return Normalized (isLogarithmic_.get(), low_.get(), high_.get(), interval_.get()).convert (value_.get());
}

float DialPainter::getAngle() const
{
    return juce::jmap (getNormalizedValue(), startAngle_, endAngle_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<float> getCentredWithProportion (const juce::Rectangle<float>& r, float f)
{
    jassert (f > 0.0f && f < 1.0f);
    
    const float g = (1.0f - f) / 2.0f;
    
    return r.getProportion (juce::Rectangle<float> (g, g, f, f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::paintDialMarker (juce::Rectangle<float> r,
    juce::Graphics& g,
    float angle,
    float thickness)
{
    const juce::Point<float> centre (r.getCentre());
    
    const float radius = r.getWidth() / 2.0f;
    const float inner  = radius * 0.70f;
    const float outer  = radius * 1.30f;
    
    const juce::Line<float> line (juce::Line<float>::fromStartAndAngle (centre, outer, angle));
    
    g.setColour (dialForegroundColour_.get());
    g.drawLine (line.withShortenedStart (inner), thickness);
}

juce::Rectangle<float> DialPainter::paintDialForeground (juce::Rectangle<float> r,
    juce::Graphics& g,
    float angle,
    float thickness)
{
    const float offset = r.proportionOfWidth (kOffset_);
    
    const juce::Rectangle<float> t (getCentredWithProportion (r, kDial_).translated (0.0f, - offset));
    
    const float x = t.getCentreX();
    const float y = t.getCentreY();
    const float w = t.getWidth() / 2.0f;
    
    juce::Path p; p.addCentredArc (x, y, w, w, 0.0f, startAngle_, endAngle_, true);
    
    g.setColour (dialForegroundColour_.get());
    g.strokePath (p, juce::PathStrokeType (thickness));
    
    return t;
}

void DialPainter::paintDial (juce::Rectangle<float> r, juce::Graphics& g)
{
    const float thickness = juce::jmax (1.0f, r.getHeight() / 15.0f);
    const float angle     = getAngle();
    
    const juce::Rectangle<float> t = paintDialForeground (r, g, angle, thickness);
    
    paintDialMarker (t, g, angle, thickness);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    const int heightDigits = r.proportionOfWidth (kDigits_);

    g.setColour (dialBackgroundColour_.get());
    g.fillRect (r);

    paintDial (r.toFloat(), g);
    
    if (heightDigits > 10 && digits_.get() > 0) {
        g.setColour (dialTextColour_.get());
        paintTextAsDigits (r.removeFromBottom (heightDigits), g, getText(), getFont (heightDigits));
    }
}

juce::Rectangle<int> DialPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsFromDimensions();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> DialPainter::getDimensions()
{
    const int w = width_.get(); return core::Vector::Real (w, w);
}

void DialPainter::setDimensions (core::Vector::Real v)
{
    setDimensionsByParameters (v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
