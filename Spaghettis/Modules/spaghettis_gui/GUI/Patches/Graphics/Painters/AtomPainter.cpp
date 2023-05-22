
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
    text_(),
    decimals_ (0),
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

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double getStep (int decimals, bool hasModifierKey)
{
    if (hasModifierKey && decimals) { return std::pow (10.0, -decimals); }
    else {
        return 1.0;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void AtomPainter::mouseDown (const juce::MouseEvent& e)
{
    dragged_    = true;
    decimals_   = Helpers::getNumberOfDigitsAfterDecimalSeparator (text_);
    v_          = value_.get();
    
    component_->repaint();
}

void AtomPainter::mouseDrag (const juce::MouseEvent& e)
{
    const int dY   = -e.getDistanceFromDragStartY();
    const double k = getStep (decimals_, Mouse::hasShiftKey (e));
    
    DBG (v_ + (dY * k));
    
    // Spaghettis()->handle (Inputs::sendObjectFloat (getIdentifier()));
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

juce::String AtomPainter::getPlaceholder() const
{
    /* < https://stackoverflow.com/a/9284821 > */
    
    return juce::String ("-.").paddedRight ('4', getDigits() + 2);
}

juce::String AtomPainter::getText() const
{
    return Helpers::withNumberOfDigits (value_.get(), getDigits());
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
    
    text_ = getText(); paintText (r, g, text_, juce::Justification::centredRight);
}

juce::Rectangle<int> AtomPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromText (getPlaceholder(), getTriangleWidth());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
