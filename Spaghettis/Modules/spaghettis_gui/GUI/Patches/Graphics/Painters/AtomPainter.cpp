
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

AtomPainter::AtomPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    atomBackgroundColour_ (Spaghettis()->getCachedColour (Tag::AtomBackground)),
    atomTextColour_ (Spaghettis()->getCachedColour (Tag::AtomText)),
    atomClickedColour_ (Spaghettis()->getCachedColour (Tag::AtomClicked)),
    digits_ (object_.getCached<int> (Tag::Parameters, Tag::Digits)),
    value_ (object_.getCached<double> (Tag::Parameters, Tag::Value)),
    low_ (object_.getCached<double> (Tag::Parameters, Tag::Low)),
    high_ (object_.getCached<double> (Tag::Parameters, Tag::High)),
    interval_ (object_.getCached<double> (Tag::Parameters, Tag::Interval)),
    v_ (0.0),
    dragged_ (false)
{
    atomBackgroundColour_.attach (repaint (component_));
    atomTextColour_.attach (repaint (component_));
    digits_.attach (resized (component_));
    value_.attach (repaint (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void AtomPainter::mouseDown (const juce::MouseEvent& e)
{
    dragged_ = true;
    v_       = value_.get();
        
    component_->repaint();
}

void AtomPainter::mouseDrag (const juce::MouseEvent& e)
{
    const int dY            = -e.getDistanceFromDragStartY();
    const double interval   = interval_.get();
    const double step       = (interval != 0.0) ? interval : 1.0;
    const double f          = v_ + (dY * step);
    const double v          = std::round (f / step) * step;
    const double low        = low_.get();
    const double high       = high_.get();
    
    const juce::Range<double> r (juce::jmin (low, high), juce::jmax (low, high));

    Spaghettis()->handle (Inputs::sendObjectFloat (getIdentifier(), r.isEmpty() ? v : r.clipValue (v)));
}

void AtomPainter::mouseUp (const juce::MouseEvent&)
{
    dragged_ = false;
    
    component_->repaint();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Colour AtomPainter::getPinsBackground()
{
    return atomBackgroundColour_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int AtomPainter::getDigits() const
{
    const int n = digits_.get(); return (n > 0) ? n : width_;
}

int AtomPainter::getTriangleWidth() const
{
    return static_cast<int> (12 * getScale());
}

juce::Font AtomPainter::getFont() const
{
    return Fonts::getFontRescaled (getScale());
}

juce::String AtomPainter::getPlaceholder() const
{
    /* < https://stackoverflow.com/a/9284821 > */
    
    return juce::String ("-.").paddedRight ('4', getDigits() + 2);
}

juce::String AtomPainter::getText() const
{
    return Strings::withNumberOfDigitsTruncated (value_.get(), getDigits());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void paintTriangle (juce::Rectangle<float> r, juce::Graphics& g)
{
    const juce::Rectangle<float> p (0.25f, 0.25f, 0.75f, 0.5f);
    const juce::Rectangle<float> t (r.getProportion (p));
    
    const juce::Point<float> pt (t.getRight(), t.getCentreY());
    
    juce::Path path;
    
    path.addTriangle (t.getTopLeft(), t.getBottomLeft(), pt);
    
    g.fillPath (path);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void AtomPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (getPinsBackground());
    
    g.fillRect (r);
    
    g.setColour (dragged_ ? atomClickedColour_.get() : atomTextColour_.get().withAlpha (0.5f));
    
    paintTriangle (r.removeFromLeft (getTriangleWidth()).toFloat(), g);
    
    g.setColour (dragged_ ? atomClickedColour_.get() : atomTextColour_.get());
    
    paintText (r, g, getText(), getFont(), juce::Justification::centredRight);
}

juce::Rectangle<int> AtomPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsFromText (getPlaceholder(), getFont(), getTriangleWidth());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
