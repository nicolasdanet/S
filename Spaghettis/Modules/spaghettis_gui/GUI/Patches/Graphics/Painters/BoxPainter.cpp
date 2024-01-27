
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BoxPainter::BoxPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    boxBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxBackground), component_)),
    boxTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxText), component_)),
    boxWrongColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxWrong), component_)),
    content_ (Resized (object_.getCached<juce::String> (Tag::Attributes, Tag::Content), component_)),
    class_ (object_.getCached<juce::String> (Tag::Attributes, Tag::Class))
{
    component_->setBufferedToImage (true);
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
    
    paintText (r, g, getText(), getFont(), juce::Justification::centredLeft);
}

juce::Rectangle<int> BoxPainter::getRequiredBoundsForWidget()
{
    return getRequiredBoundsFromText (getText(), getFont());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
