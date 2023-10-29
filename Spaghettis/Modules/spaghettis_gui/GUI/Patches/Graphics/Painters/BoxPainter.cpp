
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BoxPainter::BoxPainter (ObjectComponent* owner) :
    PainterPolicy (owner),
    boxBackgroundColour_ (Spaghettis()->getCachedColour (Tag::BoxBackground)),
    boxTextColour_ (Spaghettis()->getCachedColour (Tag::BoxText)),
    boxWrongColour_ (Spaghettis()->getCachedColour (Tag::BoxWrong)),
    content_ (object_.getCached<juce::String> (Tag::Attributes, Tag::Content)),
    class_ (object_.getCached<juce::String> (Tag::Attributes, Tag::Class))
{
    boxBackgroundColour_.attach (repaint (component_));
    boxTextColour_.attach (repaint (component_));
    boxWrongColour_.attach (repaint (component_));
    content_.attach (resized (component_));
    
    component_->setBufferedToImage (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font BoxPainter::getFont() const
{
    return Spaghettis()->getLookAndFeel().getObjectsFont (getScale());
}

juce::String BoxPainter::getText() const
{
    juce::String text (content_.get()); if (text.isEmpty()) { text = class_.get(); }
    
    return text;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BoxPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    const bool wrong = class_.get().isEmpty();
    
    g.setColour (boxBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (wrong ? boxWrongColour_.get() : boxTextColour_.get());
    
    paintText (r, g, getText(), getFont(), juce::Justification::centredLeft);
}

juce::Rectangle<int> BoxPainter::getRequiredBoundsForObject()
{
    return getRequiredBoundsFromText (getText(), getFont());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
