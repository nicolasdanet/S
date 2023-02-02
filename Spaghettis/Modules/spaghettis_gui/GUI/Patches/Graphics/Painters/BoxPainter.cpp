
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BoxPainter::BoxPainter (ObjectComponent* owner, const core::Object& object) : PainterPolicy (owner, object),
    boxBackgroundColour_ (Spaghettis()->getCachedColour (Tag::BoxBackground)),
    boxTextColour_ (Spaghettis()->getCachedColour (Tag::BoxText)),
    boxWrongColour_ (Spaghettis()->getCachedColour (Tag::BoxWrong)),
    content_ (object_.getCached<juce::String> (Tag::Attributes, Tag::Content)),
    class_ (object_.getCached<juce::String> (Tag::Attributes, Tag::Class))
{
    boxBackgroundColour_.attach (repaint (component_));
    boxTextColour_.attach (repaint (component_));
    boxWrongColour_.attach (repaint (component_));
    content_.attach (repaint (component_));
    
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

juce::Font BoxPainter::getFont (float scale) const
{
    return Spaghettis()->getLookAndFeel().getObjectsFont (scale);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

float getTextMargins (float f)
{
    return std::round (3 * f);
}

float getMinimumWidth (float f, int m, int n)
{
    const int pins = juce::jmax (m, n);
    float w = pins * PainterPolicy::pinWidth (f);
    
    if (pins > 1) { w += (pins - 1) * PainterPolicy::pinGripX (f) * 2; }
    
    return w;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BoxPainter::paintObject (juce::Rectangle<float> r, juce::Graphics& g)
{
    const float f = getScale();

    g.setColour (boxBackgroundColour_.get());
    g.fillRect (r);
    
    if (f > 0.5) {
    //
    const juce::Rectangle<float> t (r.reduced (getTextMargins (f)).translated (0.0f, -1.0f));

    const juce::Font   font (getFont (f));
    const juce::String text (getText());

    const bool wrong = class_.get().isEmpty();
    
    g.setColour (wrong ? boxWrongColour_.get() : boxTextColour_.get());
    g.setFont (getFont (f));
    g.drawText (text, t, juce::Justification::centredLeft, true);
    //
    }
}

juce::Rectangle<float> BoxPainter::getRequiredBoundsForObject()
{
    const float f = getScale();
    
    const juce::Font   font (getFont (f));
    const juce::String text (getText());
    
    const float x = getLocalPositionScaled().getX();
    const float y = getLocalPositionScaled().getY();
    const float w = font.getStringWidthFloat (text);
    const float h = font.getHeight();
    const float k = getTextMargins (f) * 2.0f;
    
    const float m = getMinimumWidth (f, component_->getNumberOfInlets(), component_->getNumberOfOutlets());
    
    return juce::Rectangle<float> (x, y, juce::jmax (m, w + k), h + k);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
