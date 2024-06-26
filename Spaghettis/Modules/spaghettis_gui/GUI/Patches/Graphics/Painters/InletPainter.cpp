
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
    arrowColour_ (Painted (getContentColour (getObject()), getOwner())),
    isOutlet_ (isOutlet (getObject()))
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void InletPainter::paintWidget (juce::Rectangle<int> r, juce::Graphics& g)
{
    const float f = getScale();
    
    g.setColour (boxBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (arrowColour_.get().withAlpha (0.75f));
    
    if (f > 0.5) {
        if (isOutlet_) { LNF::drawArrowUp (g, r.reduced (2 * f)); }
        else {
            LNF::drawArrowDown (g, r.reduced (2 * f));
        }
    }
}

juce::Rectangle<int> InletPainter::getRequiredBoundsForWidget()
{
    const float f = getScale();
    const int w   = Fonts::getFontRescaled (f).getHeight();
    
    return PainterHelpers::getRequiredBoundsFromVector (*this, core::Vector::Scaled (w, w, f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
