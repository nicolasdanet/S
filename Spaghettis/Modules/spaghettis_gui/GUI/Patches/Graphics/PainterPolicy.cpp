
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PainterPolicy::PainterPolicy (ObjectComponent* owner, const core::Object& object) :
    owner_ (owner),
    object_ (object),
    x_ (fetchAttribute<int> (Tags::X)),
    y_ (fetchAttribute<int> (Tags::Y)),
    patchBackgroundColour_ (Spaghettis()->getCachedColour (Tags::PatchBackground))
{
    jassert (owner);
    
    auto f = []()
    {
        
    };
    
    x_.attach (f);
    y_.attach (f);
    
    patchBackgroundColour_.attach (repaint (owner_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PainterPolicy::paint (const juce::Rectangle<int>& r, juce::Graphics& g)
{
    juce::Rectangle<float> t = r.toFloat();
    
    if (owner_->isInsideRunView()) { t = paintLabel (t, g); }
    
    paintObject (t, g);
}
    
juce::Rectangle<int> PainterPolicy::getRequiredBounds()
{
    juce::Rectangle<float> t = getRequiredBoundsForObject();
    
    if (owner_->isInsideRunView()) { t = getRequiredBoundsWithLabel (t); }
    
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
    return Spaghettis()->getLookAndFeel().getTooltipsFont();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<float> PainterPolicy::paintLabel (juce::Rectangle<float> r, juce::Graphics& g)
{
    const juce::Rectangle<float> t (r.removeFromRight (labelWidth_));
    
    g.setColour (patchBackgroundColour_.get());
    g.fillRect (t);
    g.setColour (juce::Colours::orange);
    g.setFont (getLabelFont());
    g.drawText (owner_->getLabel(), t, juce::Justification::centredLeft, true);
    
    return r;
}
    
juce::Rectangle<float> PainterPolicy::getRequiredBoundsWithLabel (juce::Rectangle<float> r)
{
    const juce::Font font (getLabelFont());
    
    if (owner_->hasLabel() && (r.getHeight() >= font.getHeight())) {
        labelWidth_ = font.getStringWidthFloat (owner_->getLabel());
    } else {
        labelWidth_ = 0.0f;
    }
    
    return r.withWidth (r.getWidth() + labelWidth_);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

float PainterPolicy::getScale() const
{
    return owner_->getScale();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
