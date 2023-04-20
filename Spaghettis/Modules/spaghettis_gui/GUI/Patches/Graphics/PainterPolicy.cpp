
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
    labelBackgroundColour_ (Spaghettis()->getCachedColour (Tag::LabelBackground)),
    labelTextColour_ (Spaghettis()->getCachedColour (Tag::LabelText)),
    objectWidth_ (0)
{
    jassert (owner);
    jassert (object_.isObject());
    
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
    
        t.setWidth (RunLayout::snapWidthToFitColumns (objectWidth_ + getLabelWidth (component_->getLabel())));
    }
    
    return t;
}

float PainterPolicy::getScale() const
{
    return component_->getScale();
}

core::Point::Scaled PainterPolicy::getPosition() const
{
    return core::Point::Scaled (component_->getPosition(), getScale());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterPolicy::setParametersWidthAndHeight (core::Vector::Real pt)
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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
