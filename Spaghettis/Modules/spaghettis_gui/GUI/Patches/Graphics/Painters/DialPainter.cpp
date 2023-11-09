
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<float> getCentredWithProportion (const juce::Rectangle<int>& r, float f)
{
    jassert (f > 0.0f && f < 1.0f);
    
    const float g = (1.0f - f) / 2.0f;
    
    return r.toFloat().getProportion (juce::Rectangle<float> (g, g, f, f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::paintDialBackground (juce::Rectangle<int> r,
    juce::Graphics& g,
    float offset,
    float thickness)
{
    constexpr float radiusProportion = 0.65f;
    
    const juce::Rectangle<float> t (getCentredWithProportion (r, radiusProportion));
    const float x = t.getCentreX();
    const float y = t.getCentreY();
    const float w = t.getWidth() / 2.0f;
    
    juce::Path p; p.addCentredArc (x, y - offset, w, w, 0.0f, startAngle_, endAngle_, true);
    
    g.strokePath (p, juce::PathStrokeType (thickness));
}

void DialPainter::paintDial (juce::Rectangle<int> r, juce::Graphics& g, float offset)
{
    const float thickness = juce::jmax (1.0f, r.getHeight() / 15.0f);

    g.setColour (dialNeedleColour_.get());
    
    paintDialBackground (r, g, offset, thickness);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    constexpr float digitsProportion = 0.25;
    constexpr float offsetProportion = digitsProportion / 5.0f;
    
    const int   h         = r.proportionOfWidth (digitsProportion);
    const float offset    = r.proportionOfWidth (offsetProportion);
    const bool  hasDigits = (digits_.get() > 0) && (h > 10);

    g.setColour (dialBackgroundColour_.get());
    g.fillRect (r);

    paintDial (r, g, offset);
    
    if (hasDigits) {
    //
    g.setColour (dialTextColour_.get());
        
    paintTextAsDigits (r.removeFromBottom (h), g, getText(), getFont (h));
    //
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
