
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

ObjectComponent* PainterStrategy::getOwner()
{
    return component_;
}

core::Object& PainterStrategy::getObject()
{
    return object_;
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

void paintLabel (juce::Rectangle<int> r,
    juce::Graphics& g,
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

int getMinimumWidth (float f, int m, int n)
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
    const int w = getMinimumWidth (c->getScale(), m, n);

    if (r.getWidth() < w) { r.setWidth (w); }
    
    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterStrategy::paint (juce::Rectangle<int> r, juce::Graphics& g)
{
    if (component_->isInsideRunView() && component_->hasLabel()) {              /* Paint label. */
    //
    const juce::Rectangle<int> t (r.removeFromLeft (getWidgetWidth()));
    
    paintLabel (r.withTrimmedLeft (4),
        g,
        patchLabelBackgroundColour_.get(),
        patchLabelTextColour_.get(),
        component_->getLabel());
    
    r = t;
    //
    }
        
    paintWidget (r, g);
}

juce::Rectangle<int> PainterStrategy::getRequiredBounds()
{
    juce::Rectangle<int> t = withMinimumWidthForPins (component_, getRequiredBoundsForWidget());
        
    widgetWidth_ = t.getWidth();
    
    if (component_->isInsideRunView() && component_->hasLabel()) {              /* Add label bounds. */
    //
    t.setWidth (RunLayout::snapWidthToFitColumns (getWidgetWidth() + getLabelWidth (component_->getLabel())));
    //
    }
    
    return t;
}

int PainterStrategy::getWidgetWidth() const
{
    return widgetWidth_;
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
// MARK: -

void PainterStrategy::paintText (juce::Rectangle<int> r,
    juce::Graphics& g,
    const juce::String& text,
    const juce::Font& font,
    juce::Justification justification)
{
    const float f = getScale();
        
    if (f > 0.5) {
    //
    const juce::Rectangle<int> t (r.reduced (PainterHelpers::getTextMargins (f)));
    
    g.setFont (font);
    
    g.drawText (text, t, justification, true);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void paintGlyphs (juce::Rectangle<float> r, juce::Graphics& g, const juce::GlyphArrangement& glyphs)
{
    const juce::Rectangle<float> box (glyphs.getBoundingBox (0, -1, true));
    
    const float spaceX = r.getWidth() - box.getWidth();
    
    if (spaceX > 0.0f) {
    //
    const float deltaX = spaceX / 2.0f;
    const float deltaY = r.getY() - (r.getHeight() / 4.0f);
    
    glyphs.draw (g, juce::AffineTransform::translation (deltaX, deltaY));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Workaround to centre the digits without considering the minus sign. */

void PainterStrategy::paintTextAsDigits (juce::Rectangle<int> r,
    juce::Graphics& g,
    const juce::String& text,
    const juce::Font& font)
{
    const juce::String s (text.startsWithChar ('-') ? text + " " : text);
    
    juce::GlyphArrangement glyphs; glyphs.addLineOfText (font, s, r.getX(), r.getHeight());
    
    g.setFont (font);
    
    paintGlyphs (r.toFloat(), g, glyphs);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterStrategy::setDimensionsByParameters (core::Vector::Real v)
{
    const int w = v.getPoint().getX();
    const int h = v.getPoint().getY();
    
    if (w > 0 && h > 0) {
    //
    data::Group group    = object_.getCopyOfParameters();
    const bool hasWidth  = group.hasParameter (Tag::Width);
    const bool hasHeight = group.hasParameter (Tag::Height);
    
    if (hasWidth) {
    //
    group.getParameter (Tag::Width).changeValue (hasHeight ? w : juce::jmax (w, h));
    
    if (hasHeight) {
        group.getParameter (Tag::Height).changeValue (h);
    }
    
    Broadcast::parameters (getIdentifier(), group);
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
