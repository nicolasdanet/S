
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

juce::Font DialPainter::getFont() const
{
    return Spaghettis()->getLookAndFeel().getDialsFont (getScale());
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

void DialPainter::paintDial (juce::Rectangle<int> r, juce::Graphics& g)
{
    const juce::Rectangle<float> t (getCentredWithProportion (r, 0.75f));
    const float x = t.getCentreX();
    const float y = t.getCentreY();
    const float w = t.getWidth() / 2.0f;

    juce::Path p; p.addCentredArc (x, y, w, w, 0.0f, startAngle_, endAngle_, true);
    
    g.setColour (dialNeedleColour_.get());
    g.strokePath (p, juce::PathStrokeType (thickness_));
}

void DialPainter::paintDigits (juce::Rectangle<int> r, juce::Graphics& g)
{
    if (digits_.get()) {
    //
    const juce::Font font (getFont());
    
    g.setColour (dialTextColour_.get());

    paintText (r.removeFromBottom (font.getHeight()), g, getText(), font, juce::Justification::centred);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void DialPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (dialBackgroundColour_.get());
    g.fillRect (r);

    paintDial (r, g);
    paintDigits (r, g);
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
