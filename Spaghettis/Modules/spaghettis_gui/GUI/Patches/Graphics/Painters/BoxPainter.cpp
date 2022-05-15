
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BoxPainter::BoxPainter (ObjectComponent* owner, const core::Object& object) : PainterPolicy (owner, object),
    font_ (Spaghettis()->getLookAndFeel().getObjectsFont()),
    backgroundColour_ (Spaghettis()->getCachedColour (Tags::BoxBackground)),
    textColour_ (Spaghettis()->getCachedColour (Tags::BoxText)),
    buffer_ (fetchAttribute<juce::String> (Tags::Buffer)),
    class_ (fetchAttribute<juce::String> (Tags::Class))
{
    backgroundColour_.attach (repaint (owner_));
    textColour_.attach (repaint (owner_));
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BoxPainter::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    const float f = getScale();
    
    const juce::String text (getText());
    const juce::Rectangle<int> t (r.reduced (PainterPolicy::margins (f)).translated (0, -1));
    
    g.setColour (backgroundColour_.get());
    g.fillRect (r);
    g.setColour (textColour_.get());
    g.setFont (font_);
    g.drawText (text, t, juce::Justification::centredLeft, true);
}

juce::Rectangle<int> BoxPainter::getRequiredBounds()
{
    const float f = getScale();
    
    const juce::String text (getText());
    
    const int x = x_.get();
    const int y = y_.get();
    const int w = font_.getStringWidth (text);
    const int h = static_cast<int> (font_.getHeight());
    const int k = PainterPolicy::margins (f) * 2;
    
    return juce::Rectangle<int> (x, y, w + k, h + k);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
