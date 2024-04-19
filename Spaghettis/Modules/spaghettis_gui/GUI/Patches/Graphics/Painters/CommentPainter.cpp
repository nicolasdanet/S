
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

CommentPainter::CommentPainter (ObjectComponent* owner) :
    PainterStrategy (owner, Tag::CommentBackground),
    commentBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::CommentBackground), getOwner())),
    commentTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::CommentText), getOwner())),
    text_ (Resized (getObject().getCached<juce::String> (Tag::Parameters, Tag::Text), getOwner()))
{
    getOwner()->setBufferedToImage (true);
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
    
    PainterHelpers::paintText (*this, r, g, getText(), getFont(), juce::Justification::centredLeft);
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
