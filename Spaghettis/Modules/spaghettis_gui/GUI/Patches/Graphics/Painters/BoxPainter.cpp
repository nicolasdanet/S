
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BoxPainter::BoxPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    boxBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxBackground), getOwner())),
    boxTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxText), getOwner())),
    boxWrongColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxWrong), getOwner())),
    content_ (Resized (getObject().getCached<juce::String> (Tag::Attributes, Tag::Content), getOwner())),
    class_ (getObject().getCached<juce::String> (Tag::Attributes, Tag::Class))
{
    getOwner()->setBufferedToImage (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font BoxPainter::getFont() const
{
    return Fonts::getFontRescaled (getScale());
}

juce::String BoxPainter::getText() const
{
    juce::String text (content_.get()); if (text.isEmpty()) { text = class_.get(); }
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BoxPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    const bool wrong = class_.get().isEmpty();
    
    g.setColour (boxBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (wrong ? boxWrongColour_.get() : boxTextColour_.get());
    
    PainterHelpers::paintText (*this, r, g, getText(), getFont(), juce::Justification::centredLeft);
}

juce::Rectangle<int> BoxPainter::getRequiredBoundsForWidget()
{
    return PainterHelpers::getRequiredBoundsFromText (*this, getText(), getFont());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
