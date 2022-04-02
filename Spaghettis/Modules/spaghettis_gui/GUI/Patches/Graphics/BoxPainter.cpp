
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

BoxPainter::BoxPainter (juce::Component& owner, const juce::ValueTree& content) :
    owner_ (owner),
    content_ (content),
    font_ (Spaghettis()->getLookAndFeel().getObjectsFont()),
    backgroundColour_ (Spaghettis()->getCachedColour (Tags::BoxBackground)),
    textColour_ (Spaghettis()->getCachedColour (Tags::BoxText))
{
    backgroundColour_.attach (owner_);
    textColour_.attach (owner_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String getTextToDraw (const juce::ValueTree& t)
{
    juce::String text (core::Object::getAttribute<juce::String> (t, Tags::Buffer));
    
    if (text.isEmpty()) {
        text = core::Object::getAttribute<juce::String> (t, Tags::Class);
    }
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BoxPainter::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    const juce::String text (getTextToDraw (content_));
    
    g.setColour (backgroundColour_.get());
    g.fillRect (r);
    g.setColour (textColour_.get());
    g.setFont (font_);
    g.drawText (text, r.reduced (4), juce::Justification::centredLeft, true);
}

juce::Rectangle<int> BoxPainter::getBounds()
{
    const juce::String text (getTextToDraw (content_));
    
    const int x = core::Object::getAttribute<int> (content_, Tags::X);
    const int y = core::Object::getAttribute<int> (content_, Tags::Y);
    const int w = font_.getStringWidth (text);
    const int h = static_cast <int> (font_.getHeight());

    return juce::Rectangle<int> (x, y, w + 8, h + 8);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
