
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
    x_ (fetchAttribute<int> (Tags::X)),
    y_ (fetchAttribute<int> (Tags::Y)),
    patchBackgroundColour_ (Spaghettis()->getCachedColour (Tags::PatchBackground)),
    labelBackgroundColour_ (Spaghettis()->getCachedColour (Tags::LabelBackground)),
    labelTextColour_ (Spaghettis()->getCachedColour (Tags::LabelText))
{
    jassert (owner);
    
    auto f = []()
    {
        
    };
    
    x_.attach (f);
    y_.attach (f);
    
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
    g.setColour (labelTextColour_.get());
    g.setFont (getLabelFont());
    g.drawText (component_->getLabel(), r.translated (-1.0f, -1.0f), juce::Justification::bottomRight, true);
    
    return t;
}
    
juce::Rectangle<float> PainterPolicy::getRequiredBoundsWithLabel (juce::Rectangle<float> r)
{
    const juce::Font font (getLabelFont());
    
    objectWidth_ = r.getWidth();
    
    if (component_->hasLabel() && (r.getHeight() >= font.getHeight())) {
        const int w = objectWidth_ + font.getStringWidthFloat (component_->getLabel());
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

Patch& PainterPolicy::getPatch() const
{
    return component_->getPatch();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
