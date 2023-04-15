
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

void PainterPolicy::paint (juce::Rectangle<int> r, juce::Graphics& g)
{
    if (component_->isInsideRunView()) { r = paintLabel (r, g); }
    
    paintObject (r, g);
}
    
juce::Rectangle<int> PainterPolicy::getRequiredBounds()
{
    juce::Rectangle<int> t = getRequiredBoundsForObject();
    
    if (component_->isInsideRunView()) { t = getRequiredBoundsWithLabel (t); }
    
    return t.toNearestInt();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterPolicy::setDimensions (core::Vector::Real pt)
{
    const int w = pt.getPoint().getX();
    const int h = pt.getPoint().getY();
    
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

juce::Rectangle<int> PainterPolicy::paintLabel (juce::Rectangle<int> r, juce::Graphics& g)
{
    const juce::Rectangle<int> t (r.removeFromLeft (objectWidth_));
    
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
    
juce::Rectangle<int> PainterPolicy::getRequiredBoundsWithLabel (juce::Rectangle<int> r)
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

core::Point::Scaled PainterPolicy::getLocalPositionScaled() const
{
    return core::Geometry::realToScaled (component_->getRealPosition(), getScale());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
