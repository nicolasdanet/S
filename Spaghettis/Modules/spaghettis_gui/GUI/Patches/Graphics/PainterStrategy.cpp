
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

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
    boxPinsBackgroundColour_ (Painted (Spaghettis()->getCachedColour (tag), owner)),
    patchLabelBackgroundColour_ (Painted (Spaghettis()->getCachedColour (Tag::PatchLabelBackground), owner)),
    patchLabelTextColour_ (Painted (Spaghettis()->getCachedColour (Tag::PatchLabelText), owner)),
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

juce::Font getLabelFont()
{
    return Fonts::getMonospacedFont();
}

int getLabelWidth (const juce::String& s)
{
    return getLabelFont().getStringWidth (s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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

juce::Rectangle<int> paintPinsBackground (juce::Graphics& g,
    juce::Rectangle<int> bounds,
    int w,
    juce::Colour c,
    float f)
{
    juce::Rectangle<int> widget (bounds.reduced (0, Painter::pinHeight (f)));
    
    g.setColour (c);
    g.fillRect (juce::Rectangle<int> (bounds.getTopLeft(), widget.getTopRight()).withWidth (w));
    g.fillRect (juce::Rectangle<int> (widget.getBottomLeft(), bounds.getBottomRight()).withWidth (w));
    
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
    
    juce::Rectangle<int> widget (paintPinsBackground (g, bounds, widgetWidth_, c, getScale()));
    
    if (isRunView) {        /* Paint label. */
    //
    const juce::Rectangle<int> t (widget.removeFromLeft (widgetWidth_));
    
    paintLabel (g,
        widget.withTrimmedLeft (4),
        patchLabelBackgroundColour_.get(),
        patchLabelTextColour_.get(),
        owner_->getLabel());
    
    widget = t;
    //
    }
        
    paintWidget (widget, g);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<int> PainterStrategy::getRequiredBounds()
{
    const bool isRunView = owner_->isInsideRunView();
    
    juce::Rectangle<int> t = withMinimumWidthForPins (owner_, getRequiredBoundsForWidget());
        
    widgetWidth_ = t.getWidth();
    
    if (isRunView) {    /* Add label bounds. */
    //
    t.setWidth (RunLayout::snapWidthToFitColumns (widgetWidth_ + getLabelWidth (owner_->getLabel())));
    //
    }
    
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
