
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

MessagePainter::MessagePainter (ObjectComponent* owner) :
    PainterStrategy (owner, Tag::MessageBackground),
    messageBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::MessageBackground), getOwner())),
    messageTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::MessageText), getOwner())),
    messageClickedColour_ (Painted (Spaghettis()->getCachedColour (Tag::MessageClicked), getOwner())),
    text_ (Resized (getObject().getCached<juce::String> (Tag::Parameters, Tag::Text), getOwner())),
    isClicked_ (false)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void MessagePainter::clicked (bool isClicked)
{
    if (isClicked != isClicked_) { isClicked_ = isClicked; getOwner()->repaint(); }
}

void MessagePainter::timerCallback()
{
    clicked (false); stopTimer();
}

void MessagePainter::mouseDown (const juce::MouseEvent& e)
{
    Broadcast::sendBang (getIdentifier());
    
    clicked (true); startTimer (500);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font MessagePainter::getFont() const
{
    return Fonts::getFontRescaled (getScale());
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

void MessagePainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (messageBackgroundColour_.get());
    
    paintBackground (r, g);
    
    g.setColour (isClicked_ ? messageClickedColour_.get() : messageTextColour_.get());
    
    PainterHelpers::paintText (*this, r, g, text_.get(), getFont(), juce::Justification::centredLeft);
}

juce::Rectangle<int> MessagePainter::getRequiredBoundsForWidget()
{
    return PainterHelpers::getRequiredBoundsFromText (*this, text_.get(), getFont(), getExtra() * 2);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
