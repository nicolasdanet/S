
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

DialPainter::DialPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    dialBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::DialBackground), component_)),
    dialForegroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::DialForeground), component_)),
    dialNeedleColour_ (Painted (Spaghettis()->getCachedColour (Tag::DialNeedle), component_)),
    dialTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::DialText), component_)),
    value_ (Painted (object_.getCached<double> (Tag::Parameters, Tag::Value), component_)),
    low_ (Painted (object_.getCached<double> (Tag::Parameters, Tag::Low), component_)),
    high_ (Painted (object_.getCached<double> (Tag::Parameters, Tag::High), component_)),
    interval_ (Painted (object_.getCached<double> (Tag::Parameters, Tag::Interval), component_)),
    isLogarithmic_ (Painted (object_.getCached<bool> (Tag::Parameters, Tag::Logarithmic), component_)),
    width_ (Resized (object_.getCached<int> (Tag::Parameters, Tag::Width), component_)),
    digits_ (Painted (object_.getCached<int> (Tag::Parameters, Tag::Digits), component_)),
    painted_(),
    v_ (0.0f),
    dragged_ (false)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::mouseDown (const juce::MouseEvent& e)
{
    dragged_ = true;
    v_       = getNormalizedValue();
    
    component_->repaint();
}

void DialPainter::mouseDrag (const juce::MouseEvent& e)
{
    const float h  = painted_.getHeight();
    const float f  = v_ + (-e.getDistanceFromDragStartY() / h);
    const double v = Normalized (isLogarithmic_.get(), low_.get(), high_.get(), interval_.get()).map (f);
    
    Spaghettis()->handle (Inputs::sendObjectFloat (getIdentifier(), v));
}

void DialPainter::mouseUp (const juce::MouseEvent&)
{
    dragged_ = false;
    
    component_->repaint();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font DialPainter::getFont (int h) const
{
    return Fonts::getMonospacedFontWithHeight (h);
}

juce::String DialPainter::getText() const
{
    return Strings::withNumberOfDigitsTruncated (value_.get(), digits_.get());
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

void paintCentredArc (juce::Rectangle<float> r,
    juce::Graphics& g,
    float start,
    float end,
    float thickness)
{
    const float x = r.getCentreX();
    const float y = r.getCentreY();
    const float w = r.getWidth() / 2.0f;
    
    juce::Path p; p.addCentredArc (x, y, w, w, 0.0f, start, end, true);
        
    g.strokePath (p, juce::PathStrokeType (thickness));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::paintDialNeedle (juce::Rectangle<float> r,
    juce::Graphics& g,
    float angle,
    float thickness)
{
    const juce::Point<float> centre (r.getCentre());
    
    const float radius = r.getWidth() / 2.0f;
    const float inner  = radius * 0.70f;
    const float outer  = radius * 1.30f;
    
    const juce::Line<float> line (juce::Line<float>::fromStartAndAngle (centre, outer, angle));
    
    g.drawLine (line.withShortenedStart (inner), thickness);
}

juce::Rectangle<float> DialPainter::paintDialForeground (juce::Rectangle<float> r,
    juce::Graphics& g,
    float angle,
    float thickness)
{
    const float offset = r.proportionOfWidth (kOffset_);
    
    const juce::Rectangle<float> t (getCentredWithProportion (r, kDial_).translated (0.0f, -offset));
    
    g.setColour (dialForegroundColour_.get());
    paintCentredArc (t, g, angle, endAngle_, thickness);
    
    g.setColour (dragged_ ? dialNeedleColour_.get() : dialForegroundColour_.get());
    paintCentredArc (t, g, startAngle_, angle, thickness);
    
    return t;
}

juce::Rectangle<float> DialPainter::paintDial (juce::Rectangle<float> r, juce::Graphics& g)
{
    const float thickness = juce::jmax (1.0f, r.getHeight() / 15.0f);
    const float angle     = getAngle();
    
    const juce::Rectangle<float> t = paintDialForeground (r, g, angle, thickness);
    
    paintDialNeedle (t, g, angle, thickness);
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    const int heightDigits = r.proportionOfWidth (kDigits_);

    g.setColour (dialBackgroundColour_.get());
    
    g.fillRect (r);

    painted_ = paintDial (r.toFloat(), g);
    
    if (heightDigits > 10) {
    if (digits_.get() > 0) {
        g.setColour (dragged_ ? dialNeedleColour_.get() : dialTextColour_.get());
        paintTextAsDigits (r.removeFromBottom (heightDigits), g, getText(), getFont (heightDigits));
    }
    }
}

juce::Rectangle<int> DialPainter::getRequiredBoundsForWidget()
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
