
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
    backgroundColour_ (getColour (Tags::BoxBackground)),
    textColour_ (getColour (Tags::BoxText)),
    text_ (object.getCachedAttribute<juce::String> (Tags::Buffer)),
    class_ (object.getCachedAttribute<juce::String> (Tags::Class))
{
    bind (backgroundColour_);
    bind (textColour_);
    bind (text_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String BoxPainter::getText() const
{
    juce::String text (text_.get()); if (text.isEmpty()) { text = class_.get(); }
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BoxPainter::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    const juce::String text (getText());
    
    g.setColour (backgroundColour_.get());
    g.fillRect (r);
    g.setColour (textColour_.get());
    g.setFont (font_);
    g.drawText (text, r.reduced (4), juce::Justification::centredLeft, true);
}

juce::Rectangle<int> BoxPainter::getBounds()
{
    const juce::String text (getText());
    
    const int x = x_.get();
    const int y = y_.get();
    const int w = font_.getStringWidth (text);
    const int h = static_cast <int> (font_.getHeight());

    return juce::Rectangle<int> (x, y, w + 8, h + 8);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
