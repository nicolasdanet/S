
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
    digits_ (object_.getCached<int> (Tag::Parameters, Tag::Digits)),
    value_ (object_.getCached<double> (Tag::Parameters, Tag::Value))
{
    atomBackgroundColour_.attach (repaint (component_));
    atomTextColour_.attach (repaint (component_));
    digits_.attach (resized (component_));
    value_.attach (resized (component_));
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
    return static_cast<int> (16 * getScale());
}

juce::String AtomPainter::getText() const
{
    juce::String text (value_.get());
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void paintTriangle (juce::Rectangle<float> r, juce::Graphics& g)
{
    const juce::Rectangle<float> p (0.25f, 0.25f, 0.5f, 0.5f);
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
    
    g.setColour (atomTextColour_.get().withAlpha (0.5f));
    
    paintTriangle (r.removeFromLeft (getTriangleWidth()).toFloat(), g);
    
    g.setColour (atomTextColour_.get());
    
    paintText (r, g, getText());
}

juce::Rectangle<int> AtomPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromText (getText()).expanded (getTriangleWidth() / 2, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
