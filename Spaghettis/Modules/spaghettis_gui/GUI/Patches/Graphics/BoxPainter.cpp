
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
    font_ (Spaghettis()->getLookAndFeel().getConsoleFont()),
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
    juce::String text (core::Object::getAttribute<juce::String> (content_, Tags::Buffer));
    
    g.setColour (backgroundColour_.get());
    g.fillRect (r);
    g.setColour (textColour_.get());
    g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
    g.drawText (text, r.reduced (2), juce::Justification::centredLeft, true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
