
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

CommentPainter::CommentPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    commentBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::CommentBackground), getOwner())),
    commentTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::CommentText), getOwner())),
    text_ (Resized (getObject().getCached<juce::String> (Tag::Parameters, Tag::Text), getOwner()))
{
    getOwner()->setBufferedToImage (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Colour CommentPainter::getPinsBackgroundColour()
{
    return commentBackgroundColour_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font CommentPainter::getFont() const
{
    return Fonts::getFontRescaled (getScale());
}

juce::String CommentPainter::getText() const
{
    juce::String text (text_.get()); if (text.isEmpty()) { text = Tag::comment; }
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void CommentPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    g.setColour (commentBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (commentTextColour_.get());
    
    paintText (r, g, getText(), getFont(), juce::Justification::centredLeft);
}

juce::Rectangle<int> CommentPainter::getRequiredBoundsForWidget()
{
    return PainterHelpers::getRequiredBoundsFromText (*this, getText(), getFont());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
