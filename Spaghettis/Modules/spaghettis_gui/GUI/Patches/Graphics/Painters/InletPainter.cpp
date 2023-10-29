
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

core::Cached<juce::Colour> getContentColour (const core::Object& o)
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
    PainterPolicy (owner),
    boxBackgroundColour_ (Spaghettis()->getCachedColour (Tag::BoxBackground)),
    arrowColour_ (getContentColour (object_)),
    isOutlet_ (isOutlet (object_))
{
    boxBackgroundColour_.attach (repaint (component_));
    arrowColour_.attach (repaint (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void InletPainter::paintObject (juce::Rectangle<int> r, juce::Graphics& g)
{
    const float f = getScale();
    
    g.setColour (boxBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (arrowColour_.get().withAlpha (0.75f));
    
    if (f > 0.5) {
        if (isOutlet_) { LookAndFeel::drawArrowUp (g, r.reduced (2 * f)); }
        else {
            LookAndFeel::drawArrowDown (g, r.reduced (2 * f));
        }
    }
}

juce::Rectangle<int> InletPainter::getRequiredBoundsForObject()
{
    const float f = getScale();
    const int w   = Spaghettis()->getLookAndFeel().getObjectsFont (f).getHeight();
    
    return getRequiredBoundsFromVector (core::Vector::Scaled (w, w, f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
