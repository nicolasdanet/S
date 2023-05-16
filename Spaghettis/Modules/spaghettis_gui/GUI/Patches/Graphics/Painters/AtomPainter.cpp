
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

juce::String AtomPainter::getText() const
{
    juce::String text (value_.get());
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void AtomPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (atomBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (atomTextColour_.get());
    
    paintText (r, g, getText());
}

juce::Rectangle<int> AtomPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromText (getText());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
