
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BoxPainter::BoxPainter (ObjectComponent* owner, const core::Object& object) : PainterPolicy (owner, object),
    boxBackgroundColour_ (Spaghettis()->getCachedColour (Tags::BoxBackground)),
    boxTextColour_ (Spaghettis()->getCachedColour (Tags::BoxText)),
    buffer_ (fetchAttribute<juce::String> (Tags::Buffer)),
    class_ (fetchAttribute<juce::String> (Tags::Class))
{
    boxBackgroundColour_.attach (repaint (owner_));
    boxTextColour_.attach (repaint (owner_));
    buffer_.attach (repaint (owner_));
    
    owner_->setBufferedToImage (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String BoxPainter::getText() const
{
    juce::String text (buffer_.get()); if (text.isEmpty()) { text = class_.get(); }
    
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

    g.setColour (boxTextColour_.get());
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
    
    const float x = scaled (x_.get(), f);
    const float y = scaled (y_.get(), f);
    const float w = font.getStringWidthFloat (text);
    const float h = font.getHeight();
    const float k = getTextMargins (f) * 2.0f;
    
    return juce::Rectangle<float> (x, y, w + k, h + k);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
