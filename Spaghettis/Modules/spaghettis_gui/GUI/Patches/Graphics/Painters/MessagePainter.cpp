
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MessagePainter::MessagePainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    messageBackgroundColour_ (Spaghettis()->getCachedColour (Tag::CommentBackground)),
    messageTextColour_ (Spaghettis()->getCachedColour (Tag::CommentText)),
    text_ (object_.getCached<juce::String> (Tag::Parameters, Tag::Text))
{
    messageBackgroundColour_.attach (repaint (component_));
    messageTextColour_.attach (repaint (component_));
    text_.attach (resized (component_));
    
    component_->setBufferedToImage (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MessagePainter::mouseDown (const juce::MouseEvent& e)
{
    Spaghettis()->handle (Inputs::sendObjectBang (getIdentifier()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Colour MessagePainter::getPinsBackground()
{
    return messageBackgroundColour_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MessagePainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (messageBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (messageTextColour_.get());
    
    paintText (r, g, text_.get());
}

juce::Rectangle<int> MessagePainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromText (text_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
