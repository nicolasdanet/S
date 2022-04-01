
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

void BoxPainter::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    const juce::String text (core::Object::getAttribute<juce::String> (content_, Tags::Buffer));
    
    g.setColour (backgroundColour_.get());
    g.fillRect (r);
    g.setColour (textColour_.get());
    g.setFont (font_);
    g.drawText (text, r.reduced (4), juce::Justification::centredLeft, true);
}

juce::Rectangle<int> BoxPainter::getBounds()
{
    const juce::String text (core::Object::getAttribute<juce::String> (content_, Tags::Buffer));
    const int w = font_.getStringWidth (text);
    const int h = static_cast <int> (font_.getHeight());
    
    juce::Rectangle<int> bounds (core::Object::getAttribute<juce::Rectangle<int>> (content_, Tags::Bounds));
    
    bounds.setSize (w + 8, h + 8);
    
    return bounds;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
