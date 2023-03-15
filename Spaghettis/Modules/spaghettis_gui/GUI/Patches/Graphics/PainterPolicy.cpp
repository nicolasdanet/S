
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PainterPolicy::PainterPolicy (ObjectComponent* owner, const core::Object& object) :
    component_ (owner),
    object_ (object),
    patchBackgroundColour_ (Spaghettis()->getCachedColour (Tag::PatchBackground)),
    labelBackgroundColour_ (Spaghettis()->getCachedColour (Tag::LabelBackground)),
    labelTextColour_ (Spaghettis()->getCachedColour (Tag::LabelText))
{
    jassert (owner);
    
    patchBackgroundColour_.attach (repaint (component_));
    labelBackgroundColour_.attach (repaint (component_));
    labelTextColour_.attach (repaint (component_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterPolicy::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    juce::Rectangle<float> t = r.toFloat();
    
    if (component_->isInsideRunView()) { t = paintLabel (t, g); }
    
    paintObject (t, g);
}
    
juce::Rectangle<int> PainterPolicy::getRequiredBounds()
{
    juce::Rectangle<float> t = getRequiredBoundsForObject();
    
    if (component_->isInsideRunView()) { t = getRequiredBoundsWithLabel (t); }
    
    return t.toNearestInt();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterPolicy::setWidth (juce::Point<int> pt)
{
    const int w = pt.getX();
    const int h = pt.getY();
    
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
    
    EditCommands::parameters (object_.getIdentifier(), group);
    //
    }
    //
    }
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<float> PainterPolicy::paintLabel (juce::Rectangle<float> r, juce::Graphics& g)
{
    const juce::Rectangle<float> t (r.removeFromLeft (objectWidth_));
    
    g.setColour (patchBackgroundColour_.get());
    g.fillRect (r);
    g.setColour (labelBackgroundColour_.get());
    g.fillRect (r.withTrimmedLeft (2));
    
    const juce::Font font (getLabelFont());
    
    if (r.getHeight() >= font.getHeight()) {
    //
    g.setColour (labelTextColour_.get());
    g.setFont (font);
    g.drawText (component_->getLabel(), r.translated (-1.0f, -1.0f), juce::Justification::bottomRight, true);
    //
    }
    
    return t;
}
    
juce::Rectangle<float> PainterPolicy::getRequiredBoundsWithLabel (juce::Rectangle<float> r)
{
    objectWidth_ = r.getWidth();
    
    if (component_->hasLabel()) {
        const int w = objectWidth_ + getLabelFont().getStringWidthFloat (component_->getLabel());
        r.setWidth (RunLayout::snapWidthToFitColumns (w));
    }
    
    return r;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

float PainterPolicy::getScale() const
{
    return component_->getScale();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

float PainterPolicy::getScaled (float f) const
{
    return Distance::scaled (f, getScale());
}

juce::Point<float> PainterPolicy::getLocalPositionScaled() const
{
    return Coordinates::scaled (component_->getLocalPosition().toFloat(), getScale());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
