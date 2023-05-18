
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PainterPolicy::PainterPolicy (ObjectComponent* owner) :
    component_ (owner),
    object_ (owner->getObject()),
    boxPinsBackgroundColour_ (Spaghettis()->getCachedColour (Tag::BoxPinsBackground)),
    labelBackgroundColour_ (Spaghettis()->getCachedColour (Tag::LabelBackground)),
    labelTextColour_ (Spaghettis()->getCachedColour (Tag::LabelText)),
    objectWidth_ (0)
{
    jassert (owner);
    jassert (object_.isObject());
    
    boxPinsBackgroundColour_.attach (repaint (component_));
    labelBackgroundColour_.attach (repaint (component_));
    labelTextColour_.attach (repaint (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Font getLabelFont()
{
    return Spaghettis()->getLookAndFeel().getConsoleFont();
}

int getLabelWidth (const juce::String& s)
{
    return getLabelFont().getStringWidth (s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterPolicy::paint (juce::Rectangle<int> r, juce::Graphics& g)
{
    if (component_->isInsideRunView() && component_->hasLabel()) {              /* Paint label. */
    //
    const juce::Rectangle<int> t (r.removeFromLeft (objectWidth_));
    
    const juce::Font font (getLabelFont());
    
    if (r.getHeight() >= font.getHeight()) {
    //
    g.setColour (labelBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (labelTextColour_.get());
    g.setFont (font);
    g.drawText (component_->getLabel(), r.translated (-1, -1), juce::Justification::bottomRight, true);
    //
    }
    
    r = t;
    //
    }
    
    paintObject (r, g);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PainterPolicy::getRequiredBounds()
{
    juce::Rectangle<int> t = getRequiredBoundsForObject();
    
    objectWidth_ = t.getWidth();
    
    if (component_->isInsideRunView() && component_->hasLabel()) {              /* Add label bounds. */
    //
    t.setWidth (RunLayout::snapWidthToFitColumns (objectWidth_ + getLabelWidth (component_->getLabel())));
    //
    }
    
    return t;
}

float PainterPolicy::getScale() const
{
    return component_->getScale();
}

juce::Font PainterPolicy::getFont() const
{
    return Spaghettis()->getLookAndFeel().getObjectsFont (getScale());
}

core::Point::Scaled PainterPolicy::getPosition() const
{
    return core::Point::Scaled (component_->getPosition(), getScale());
}

core::UniqueId PainterPolicy::getIdentifier() const
{
    return object_.getIdentifier();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PainterPolicy::getRequiredBoundsForObjectFromVector (core::Vector::Scaled v)
{
    return juce::Rectangle<int> (v.getPoint().getX(), v.getPoint().getY()) + getPosition().getPoint();
}

juce::Rectangle<int> PainterPolicy::getRequiredBoundsForObjectFromDimensions()
{
    std::optional<core::Vector::Real> t = getDimensions();
    
    jassert (t.has_value());
    
    return getRequiredBoundsForObjectFromVector (core::Vector::Scaled (t.value(), getScale()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int getTextMargins (float f)
{
    return std::round (3 * f);
}

int getMinimumWidth (float f, int m, int n)
{
    const int pins = juce::jmax (m, n, 1);
    
    int w = pins * PainterPolicy::pinWidth (f);
    
    if (pins > 1) { w += (pins - 1) * PainterPolicy::pinGripX (f) * 2; }
    
    return w;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PainterPolicy::getRequiredBoundsForObjectFromText (const juce::String& text, int extra)
{
    const float f = getScale();
    
    const juce::Font font (getFont());
    
    const int w = font.getStringWidth (text);
    const int h = font.getHeight();
    const int m = getMinimumWidth (f, component_->getNumberOfInlets(), component_->getNumberOfOutlets());
    const int k = getTextMargins (f) * 2;
    
    core::Vector::Scaled v (juce::jmax (m, w + k), h + k, f);
    juce::Rectangle<int> t (getRequiredBoundsForObjectFromVector (v));
    
    if (extra > 0) { t.setWidth (t.getWidth() + extra); }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterPolicy::paintText (juce::Rectangle<int> r,
    juce::Graphics& g,
    const juce::String& text,
    juce::Justification justification)
{
    const float f = getScale();
        
    if (f > 0.5) {
    //
    const juce::Rectangle<int> t (r.reduced (getTextMargins (f)).translated (0, -1));
    
    g.setFont (getFont());
    g.drawText (text, t, justification, true);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterPolicy::setDimensionsByParameters (core::Vector::Real v)
{
    const int w = v.getPoint().getX();
    const int h = v.getPoint().getY();
    
    if (w > 0 && h > 0) {
    //
    core::Group group    = object_.getCopyOfParameters();
    const bool hasWidth  = group.hasParameter (Tag::Width);
    const bool hasHeight = group.hasParameter (Tag::Height);
    
    if (hasWidth) {
    //
    group.getParameter (Tag::Width).changeValue (hasHeight ? w : juce::jmax (w, h));
    
    if (hasHeight) {
        group.getParameter (Tag::Height).changeValue (h);
    }
    
    EditCommands::parameters (getIdentifier(), group);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
