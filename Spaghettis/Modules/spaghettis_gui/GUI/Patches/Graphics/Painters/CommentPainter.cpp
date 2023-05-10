
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

CommentPainter::CommentPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    commentBackgroundColour_ (Spaghettis()->getCachedColour (Tag::CommentBackground)),
    commentTextColour_ (Spaghettis()->getCachedColour (Tag::CommentText)),
    text_ (object_.getCached<juce::String> (Tag::Parameters, Tag::Text))
{
    commentBackgroundColour_.attach (repaint (component_));
    commentTextColour_.attach (repaint (component_));
    text_.attach (resized (component_));
    
    component_->setBufferedToImage (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Colour CommentPainter::getPinsBackground()
{
    return commentBackgroundColour_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String CommentPainter::getText() const
{
    juce::String text (text_.get()); if (text.isEmpty()) { text = Tag::comment; }
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void CommentPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (commentBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (commentTextColour_.get());
    
    paintText (r, g, getText());
}

juce::Rectangle<int> CommentPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsForObjectFromText (getText());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
