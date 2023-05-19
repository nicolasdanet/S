
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
    messageBackgroundColour_ (Spaghettis()->getCachedColour (Tag::MessageBackground)),
    messageTextColour_ (Spaghettis()->getCachedColour (Tag::MessageText)),
    messageClickedColour_ (Spaghettis()->getCachedColour (Tag::MessageClicked)),
    text_ (object_.getCached<juce::String> (Tag::Parameters, Tag::Text)),
    isClicked_ (false)
{
    messageBackgroundColour_.attach (repaint (component_));
    messageTextColour_.attach (repaint (component_));
    text_.attach (resized (component_));
    
    component_->setBufferedToImage (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MessagePainter::clicked (bool isClicked)
{
    if (isClicked != isClicked_) { isClicked_ = isClicked; component_->repaint(); }
}

void MessagePainter::timerCallback()
{
    clicked (false); stopTimer();
}

void MessagePainter::mouseDown (const juce::MouseEvent& e)
{
    Spaghettis()->handle (Inputs::sendObjectBang (getIdentifier()));
    
    clicked (true); startTimer (500);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Colour MessagePainter::getPinsBackground()
{
    return messageBackgroundColour_.get();
}

int MessagePainter::getExtra() const
{
    return static_cast<int> (4 * getScale());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void paintExtra (juce::Rectangle<float> r, juce::Graphics& g)
{
    const juce::Point<float> pt (r.getX(), r.getCentreY());
    
    juce::Path path;
    path.addTriangle (r.getTopLeft(), r.getTopRight(), pt);
    path.addTriangle (r.getBottomLeft(), r.getBottomRight(), pt);
    g.fillPath (path);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MessagePainter::paintBackground (juce::Rectangle<int>& r, juce::Graphics& g)
{
    paintExtra (r.removeFromRight (getExtra()).toFloat(), g);
        
    g.fillRect (r);
}

void MessagePainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (getPinsBackground());
    
    paintBackground (r, g);
    
    g.setColour (isClicked_ ? messageClickedColour_.get() : messageTextColour_.get());
    
    paintText (r, g, text_.get());
}

juce::Rectangle<int> MessagePainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromText (text_.get(), getExtra() * 2);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
