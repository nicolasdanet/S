
/* Copyright (c) 2023 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

data::Cached<juce::Colour> getContentColour (const core::Object& o)
{
    const bool isSignal = o.get<juce::String> (Tag::Attributes, Tag::Content).endsWith ("~");
    
    return Spaghettis()->getCachedColour (isSignal ? Tag::PinSignal : Tag::PinAnything);
}

bool isOutlet (const core::Object& o)
{
    return (o.get<juce::String> (Tag::Attributes, Tag::Class) == "outlet");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

InletPainter::InletPainter (ObjectComponent* owner) :
    PainterStrategy (owner),
    boxBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxBackground), getOwner())),
    contentColour_ (Painted (getContentColour (getObject()), getOwner())),
    number_ (Painted (getObject().getCached<int> (Tag::Attributes, Tag::Number), getOwner())),
    isOutlet_ (isOutlet (getObject()))
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Font InletPainter::getFont() const
{
    return Fonts::getFontRescaled (getScale());
}

juce::String InletPainter::getText() const
{
    return juce::String (number_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void InletPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    const int h = r.getHeight() / 5.0f;
    
    g.setColour (boxBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (contentColour_.get().withAlpha (0.75f));
    
    PainterHelpers::paintText (*this,
        (isOutlet_ ? r.withTrimmedTop (h).translated (0, -1) : r.withTrimmedBottom (h)),
        g,
        getText(),
        getFont(),
        juce::Justification::centred);
}

juce::Rectangle<int> InletPainter::getRequiredBoundsForWidget()
{
    const float f = getScale();
    const int w   = Fonts::getFontRescaled (f).getHeight() * 1.25;
    
    return PainterHelpers::getRequiredBoundsFromVector (*this, core::Vector::Scaled (w, w, f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
