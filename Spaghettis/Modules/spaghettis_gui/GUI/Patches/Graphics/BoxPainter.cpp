
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

BoxPainter::BoxPainter (juce::Component& owner, const core::Object& object) :
    owner_ (owner),
    font_ (Spaghettis()->getLookAndFeel().getObjectsFont()),
    backgroundColour_ (Spaghettis()->getCachedColour (Tags::BoxBackground)),
    textColour_ (Spaghettis()->getCachedColour (Tags::BoxText)),
    text_ (object.getCachedAttribute<juce::String> (Tags::Buffer)),
    class_ (object.getCachedAttribute<juce::String> (Tags::Class)),
    x_ (object.getCachedAttribute<int> (Tags::X)),
    y_ (object.getCachedAttribute<int> (Tags::Y))
{
    backgroundColour_.attach (&owner_);
    textColour_.attach (&owner_);
    //text_.attach (&owner_);
    //class_.attach (&owner_);
    //x_.attach (&owner_);
    //y_.attach (&owner_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BoxPainter::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    juce::String text (text_.get());
    
    if (text.isEmpty()) { text = class_.get(); }
    
    g.setColour (backgroundColour_.get());
    g.fillRect (r);
    g.setColour (textColour_.get());
    g.setFont (font_);
    g.drawText (text, r.reduced (4), juce::Justification::centredLeft, true);
}

juce::Rectangle<int> BoxPainter::getBounds()
{
    juce::String text (text_.get());
    
    if (text.isEmpty()) { text = class_.get(); }
    
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
