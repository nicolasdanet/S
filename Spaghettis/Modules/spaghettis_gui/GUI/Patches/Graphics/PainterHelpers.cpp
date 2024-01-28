
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

int PainterHelpers::getTextMargins (float f)
{
    return std::round (3 * f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> PainterHelpers::getRequiredBoundsFromDimensions (PainterStrategy& p)
{
    std::optional<core::Vector::Real> t = p.getDimensions();
    
    jassert (t.has_value());
    
    return getRequiredBoundsFromVector (p, core::Vector::Scaled (t.value(), p.getScale()));
}

juce::Rectangle<int> PainterHelpers::getRequiredBoundsFromVector (PainterStrategy& p, core::Vector::Scaled v)
{
    return juce::Rectangle<int> (v.getPoint().getX(), v.getPoint().getY()) + p.getPosition().getPoint();
}

juce::Rectangle<int> PainterHelpers::getRequiredBoundsFromText (PainterStrategy& p,
    const juce::String& text,
    const juce::Font& font,
    int extra)
{
    const float f = p.getScale();
    const int w   = font.getStringWidth (text);
    const int h   = font.getHeight();
    const int k   = getTextMargins (f) * 2;
    
    core::Vector::Scaled v (w + k, h + k, f);
    juce::Rectangle<int> t (getRequiredBoundsFromVector (p, v));
    
    if (extra > 0) { t.setWidth (t.getWidth() + extra); }
    
    return t;
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

void PainterHelpers::paintText (PainterStrategy& p,
    juce::Rectangle<int> r,
    juce::Graphics& g,
    const juce::String& text,
    const juce::Font& font,
    juce::Justification justification)
{
    const float f = p.getScale();
        
    if (f > 0.5) {
    //
    const juce::Rectangle<int> t (r.reduced (PainterHelpers::getTextMargins (f)));
    
    g.setFont (font);
    
    g.drawText (text, t, justification, true);
    //
    }
}

/* Workaround to centre the digits without considering the minus sign. */

void PainterHelpers::paintTextAsDigits (PainterStrategy& p,
    juce::Rectangle<int> r,
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

void PainterHelpers::setDimensionsByParameters (PainterStrategy& p, core::Vector::Real v)
{
    const int w = v.getPoint().getX();
    const int h = v.getPoint().getY();
    
    if (w > 0 && h > 0) {
    //
    data::Group group    = p.getObject().getCopyOfParameters();
    const bool hasWidth  = group.hasParameter (Tag::Width);
    const bool hasHeight = group.hasParameter (Tag::Height);
    
    if (hasWidth) {
    //
    group.getParameter (Tag::Width).changeValue (hasHeight ? w : juce::jmax (w, h));
    
    if (hasHeight) {
        group.getParameter (Tag::Height).changeValue (h);
    }
    
    Broadcast::parameters (p.getIdentifier(), group);
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
