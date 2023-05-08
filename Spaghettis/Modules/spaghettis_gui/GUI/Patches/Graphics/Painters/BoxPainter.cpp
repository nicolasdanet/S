
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BoxPainter::BoxPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    boxBackgroundColour_ (Spaghettis()->getCachedColour (Tag::BoxBackground)),
    boxTextColour_ (Spaghettis()->getCachedColour (Tag::BoxText)),
    boxWrongColour_ (Spaghettis()->getCachedColour (Tag::BoxWrong)),
    content_ (object_.getCached<juce::String> (Tag::Attributes, Tag::Content)),
    class_ (object_.getCached<juce::String> (Tag::Attributes, Tag::Class))
{
    boxBackgroundColour_.attach (repaint (component_));
    boxTextColour_.attach (repaint (component_));
    boxWrongColour_.attach (repaint (component_));
    content_.attach (resized (component_));
    
    component_->setBufferedToImage (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String BoxPainter::getText() const
{
    juce::String text (content_.get()); if (text.isEmpty()) { text = class_.get(); }
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int getTextMargins (float f)
{
    return std::round (3 * f);
}

int getMinimumWidth (float f, int m, int n)
{
    const int pins = juce::jmax (m, n);
    int w = pins * PainterPolicy::pinWidth (f);
    
    if (pins > 1) { w += (pins - 1) * PainterPolicy::pinGripX (f) * 2; }
    
    return w;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BoxPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    const float f = getScale();

    g.setColour (boxBackgroundColour_.get());
    g.fillRect (r);
    
    if (f > 0.5) {
    //
    const juce::Rectangle<int> t (r.reduced (getTextMargins (f)).translated (0, -1));

    const juce::Font   font (getFont());
    const juce::String text (getText());

    const bool wrong = class_.get().isEmpty();
    
    g.setColour (wrong ? boxWrongColour_.get() : boxTextColour_.get());
    g.setFont (getFont());
    g.drawText (text, t, juce::Justification::centredLeft, true);
    //
    }
}

juce::Rectangle<int> BoxPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromText (getText());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
