
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PainterStrategy::PainterStrategy (ObjectComponent* owner, juce::String tag) :
    owner_ (owner),
    object_ (owner->getObject()),
    boxSelectedColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxSelected), owner)),
    boxPinsBackgroundColour_ (Painted (Spaghettis()->getCachedColour (tag), owner))
{
    jassert (owner);
    jassert (object_.isObject());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int getMinimumWidthForPins (float f, int m, int n)
{
    const int pins = juce::jmax (m, n, 1);
    
    int w = pins * Painter::pinWidth (f);
    
    if (pins > 1) { w += (pins - 1) * Painter::pinGripX (f) * 2; }
    
    return w;
}

juce::Rectangle<int> withMinimumWidthForPins (ObjectComponent* c, juce::Rectangle<int> r)
{
    const int m = c->getNumberOfInlets();
    const int n = c->getNumberOfOutlets();
    const int w = getMinimumWidthForPins (c->getView()->getScale(), m, n);

    if (r.getWidth() < w) { r.setWidth (w); }
    
    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> paintPinsBackground (juce::Graphics& g,
    juce::Rectangle<int> bounds,
    juce::Colour c,
    float f)
{
    juce::Rectangle<int> widget (bounds.reduced (0, Painter::pinHeight (f)));
    
    g.setColour (c);
    g.fillRect (juce::Rectangle<int> (bounds.getTopLeft(), widget.getTopRight()));
    g.fillRect (juce::Rectangle<int> (widget.getBottomLeft(), bounds.getBottomRight()));
    
    return widget;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterStrategy::paint (juce::Rectangle<int> bounds, juce::Graphics& g)
{
    const bool isRunView  = owner_->isInsideRunView();
    const bool isSelected = owner_->isSelected();
    
    const juce::Colour c  = (!isRunView && isSelected)  ? boxSelectedColour_.get()
                                                        : boxPinsBackgroundColour_.get();
    
    juce::Rectangle<int> widget (paintPinsBackground (g, bounds, c, getScale()));
    
    paintWidget (widget, g);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<int> PainterStrategy::getRequiredBounds()
{
    juce::Rectangle<int> t = withMinimumWidthForPins (owner_, getRequiredBoundsForWidget());
    
    return t.expanded (0, Painter::pinHeight (getScale()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<core::Vector::Real> PainterStrategy::getDimensions()
{
    return std::nullopt;
}

void PainterStrategy::setDimensions (core::Vector::Real)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent* PainterStrategy::getOwner()
{
    return owner_;
}

core::Object& PainterStrategy::getObject()
{
    return object_;
}

float PainterStrategy::getScale() const
{
    return owner_->getView()->getScale();
}

core::Point::Scaled PainterStrategy::getPosition() const
{
    return core::Point::Scaled (owner_->getPosition(), getScale());
}

core::UniqueId PainterStrategy::getIdentifier() const
{
    return object_.getIdentifier();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
