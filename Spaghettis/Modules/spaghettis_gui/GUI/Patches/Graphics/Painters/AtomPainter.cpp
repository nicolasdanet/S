
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

AtomPainter::AtomPainter (ObjectComponent* owner) :
    PainterStrategy (owner, Tag::AtomBackground),
    atomBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::AtomBackground), getOwner())),
    atomTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::AtomText), getOwner())),
    atomClickedColour_ (Spaghettis()->getCachedColour (Tag::AtomClicked)),
    digits_ (Resized (getObject().getCached<int> (Tag::Parameters, Tag::Digits), getOwner())),
    value_ (Painted (getObject().getCached<double> (Tag::Parameters, Tag::Value), getOwner())),
    low_ (getObject().getCached<double> (Tag::Parameters, Tag::Low)),
    high_ (getObject().getCached<double> (Tag::Parameters, Tag::High)),
    interval_ (getObject().getCached<double> (Tag::Parameters, Tag::Interval)),
    v_ (0.0),
    dragged_ (false)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void AtomPainter::mouseDown (const juce::MouseEvent& e)
{
    dragged_ = true;
    v_       = value_.get();
        
    getOwner()->repaint();
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

    Broadcast::sendFloat (getIdentifier(), r.isEmpty() ? v : r.clipValue (v));
}

void AtomPainter::mouseUp (const juce::MouseEvent&)
{
    dragged_ = false;
    
    getOwner()->repaint();
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

void AtomPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (atomBackgroundColour_.get());
    
    g.fillRect (r);
    
    g.setColour (dragged_ ? atomClickedColour_.get() : atomTextColour_.get().withAlpha (0.5f));
    
    paintTriangle (r.removeFromLeft (getTriangleWidth()).toFloat(), g);
    
    g.setColour (dragged_ ? atomClickedColour_.get() : atomTextColour_.get());
    
    PainterHelpers::paintText (*this, r, g, getText(), getFont(), juce::Justification::centredRight);
}

juce::Rectangle<int> AtomPainter::getRequiredBoundsForWidget()
{
    return PainterHelpers::getRequiredBoundsFromText (*this, getPlaceholder(), getFont(), getTriangleWidth());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
