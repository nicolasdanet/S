
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
    objectWidth_ (0)
{
    jassert (owner);
    jassert (object_.isObject());
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
    const juce::Rectangle<int> t (r.removeFromLeft (objectWidth_));
    
    paintLabel (r.withTrimmedLeft (4),
        g,
        patchLabelBackgroundColour_.get(),
        patchLabelTextColour_.get(),
        component_->getLabel());
    
    r = t;
    //
    }
        
    paintObject (r, g);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PainterStrategy::getRequiredBounds()
{
    juce::Rectangle<int> t = withMinimumWidthForPins (getRequiredBoundsForObject());
        
    objectWidth_ = t.getWidth();
    
    if (component_->isInsideRunView() && component_->hasLabel()) {              /* Add label bounds. */
    //
    t.setWidth (RunLayout::snapWidthToFitColumns (objectWidth_ + getLabelWidth (component_->getLabel())));
    //
    }
    
    return t;
}

int PainterStrategy::getWidthOfPinsBackground() const
{
    return objectWidth_;
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
    
    int w = pins * Painter::pinWidth (f);
    
    if (pins > 1) { w += (pins - 1) * Painter::pinGripX (f) * 2; }
    
    return w;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PainterStrategy::withMinimumWidthForPins (juce::Rectangle<int> r)
{
    const int m = component_->getNumberOfInlets();
    const int n = component_->getNumberOfOutlets();
    const int w = getMinimumWidth (getScale(), m, n);

    if (r.getWidth() < w) { r.setWidth (w); }
    
    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PainterStrategy::getRequiredBoundsFromVector (core::Vector::Scaled v)
{
    return juce::Rectangle<int> (v.getPoint().getX(), v.getPoint().getY()) + getPosition().getPoint();
}

juce::Rectangle<int> PainterStrategy::getRequiredBoundsFromDimensions()
{
    std::optional<core::Vector::Real> t = getDimensions();
    
    jassert (t.has_value());
    
    return getRequiredBoundsFromVector (core::Vector::Scaled (t.value(), getScale()));
}

juce::Rectangle<int> PainterStrategy::getRequiredBoundsFromText (const juce::String& text,
    const juce::Font& font,
    int extra)
{
    const float f = getScale();
    const int w   = font.getStringWidth (text);
    const int h   = font.getHeight();
    const int k   = getTextMargins (f) * 2;
    
    core::Vector::Scaled v (w + k, h + k, f);
    juce::Rectangle<int> t (getRequiredBoundsFromVector (v));
    
    if (extra > 0) { t.setWidth (t.getWidth() + extra); }
    
    return t;
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
    const juce::Rectangle<int> t (r.reduced (getTextMargins (f)));
    
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
