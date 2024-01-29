
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PainterStrategy::PainterStrategy (ObjectComponent* owner) :
    component_ (owner),
    object_ (owner->getObject()),
    boxSelectedColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxSelected), component_)),
    boxPinsBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::BoxPinsBackground), component_)),
    patchLabelBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::PatchLabelBackground), component_)),
    patchLabelTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::PatchLabelText), component_)),
    widgetWidth_ (0)
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

juce::Font getLabelFont()
{
    return Fonts::getMonospacedFont();
}

int getLabelWidth (const juce::String& s)
{
    return getLabelFont().getStringWidth (s);
}

void paintLabel (juce::Graphics& g,
    juce::Rectangle<int> r,
    juce::Colour backgroundColour,
    juce::Colour textColour,
    const juce::String& text)
{
    const juce::Font font (getLabelFont());
    
    if (r.getHeight() >= font.getHeight()) {
    //
    g.setColour (backgroundColour);
    g.fillRect (r);
    g.setColour (textColour);
    g.setFont (font);
    g.drawText (text, r.translated (-1, -1), juce::Justification::bottomRight, true);
    //
    }
}

/*
void paintPinsBackground (juce::Graphics& g)
{
    const juce::Rectangle<int> bounds (getLocalBounds());
    const juce::Rectangle<int> painted (getWidgetAreaFromBounds (bounds, getScale()));
    const int w = painter_->getWidgetWidth();
    
    isSelected()
    
    if (!isInsideRunView() && selected_.get()) { g.setColour (boxSelectedColour_.get()); }
    else {
        g.setColour (painter_->getPinsBackgroundColour());
    }
    
    g.fillRect (juce::Rectangle<int> (bounds.getTopLeft(), painted.getTopRight()).withWidth (w));
    g.fillRect (juce::Rectangle<int> (painted.getBottomLeft(), bounds.getBottomRight()).withWidth (w));
}
*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> getWidgetAreaFromBounds (juce::Rectangle<int> r, float f)
{
    return r.reduced (0, Painter::pinHeight (f));
}

juce::Rectangle<int> getBoundsFromWidgetArea (juce::Rectangle<int> r, float f)
{
    return r.expanded (0, Painter::pinHeight (f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
    const int w = getMinimumWidthForPins (c->getScale(), m, n);

    if (r.getWidth() < w) { r.setWidth (w); }
    
    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
/*
void ObjectComponent::paint (juce::Graphics& g)
{
    const juce::Rectangle<int> bounds (getLocalBounds());
    const juce::Rectangle<int> painted (getWidgetAreaFromBounds (bounds, getScale()));
    const int w = painter_->getWidgetWidth();
    
    isSelected()
    
    if (!isInsideRunView() && selected_.get()) { g.setColour (boxSelectedColour_.get()); }
    else {
        g.setColour (painter_->getPinsBackgroundColour());
    }
    
    g.fillRect (juce::Rectangle<int> (bounds.getTopLeft(), painted.getTopRight()).withWidth (w));
    g.fillRect (juce::Rectangle<int> (painted.getBottomLeft(), bounds.getBottomRight()).withWidth (w));
    
    painter_->paint (painted, g);
}
*/

void PainterStrategy::paint (juce::Rectangle<int> bounds, juce::Graphics& g)
{
    const bool isInsideRunView = component_->isInsideRunView();
    const bool isSelected      = component_->isSelected();
    const bool hasLabel        = component_->hasLabel();
    
    juce::Rectangle<int> painted (getWidgetAreaFromBounds (bounds, getScale()));
        
    if (isInsideRunView && hasLabel) {          /* Paint label. */
    //
    const juce::Rectangle<int> t (painted.removeFromLeft (widgetWidth_));
    
    paintLabel (g,
        painted.withTrimmedLeft (4),
        patchLabelBackgroundColour_.get(),
        patchLabelTextColour_.get(),
        component_->getLabel());
    
    painted = t;
    //
    }
        
    paintWidget (painted, g);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<int> PainterStrategy::getRequiredBounds()
{
    const bool isInsideRunView = component_->isInsideRunView();
    const bool hasLabel        = component_->hasLabel();
    
    juce::Rectangle<int> t = withMinimumWidthForPins (component_, getRequiredBoundsForWidget());
        
    widgetWidth_ = t.getWidth();
    
    if (isInsideRunView && hasLabel) {          /* Add label bounds. */
    //
    t.setWidth (RunLayout::snapWidthToFitColumns (widgetWidth_ + getLabelWidth (component_->getLabel())));
    //
    }
    
    return getBoundsFromWidgetArea (t, getScale());
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

juce::Colour PainterStrategy::getPinsBackgroundColour()
{
    return boxPinsBackgroundColour_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ObjectComponent* PainterStrategy::getOwner()
{
    return component_;
}

core::Object& PainterStrategy::getObject()
{
    return object_;
}

float PainterStrategy::getScale() const
{
    return component_->getScale();
}

core::Point::Scaled PainterStrategy::getPosition() const
{
    return core::Point::Scaled (component_->getPosition(), getScale());
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
