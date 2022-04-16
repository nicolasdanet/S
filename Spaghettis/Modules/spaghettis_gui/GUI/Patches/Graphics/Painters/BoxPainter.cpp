
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BoxPainter::BoxPainter (juce::Component& owner, const core::Object& object) : PainterPolicy (owner, object),
    font_ (Spaghettis()->getLookAndFeel().getObjectsFont()),
    background_ (fetchColour (Tags::BoxBackground)),
    text_ (fetchColour (Tags::BoxText)),
    buffer_ (fetchAttribute<juce::String> (Tags::Buffer)),
    class_ (fetchAttribute<juce::String> (Tags::Class))
{
    bind (background_);
    bind (text_);
    bind (buffer_);
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
    const juce::String text (getText());
    
    g.setColour (background_.get());
    g.fillRect (r);
    g.setColour (text_.get());
    g.setFont (font_);
    g.drawText (text, r.reduced (margins_), juce::Justification::centredLeft, true);
}

juce::Rectangle<int> BoxPainter::getBounds()
{
    const juce::String text (getText());
    
    const int x = x_.get();
    const int y = y_.get();
    const int w = font_.getStringWidth (text);
    const int h = static_cast<int> (font_.getHeight());

    return juce::Rectangle<int> (x, y, w + margins_ + margins_, h + margins_ + margins_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
