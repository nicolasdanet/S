
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
    boxBackgroundColour_ (Spaghettis()->getCachedColour (Tags::BoxBackground)),
    boxTextColour_ (Spaghettis()->getCachedColour (Tags::BoxText)),
    x_ (fetchAttribute<int> (Tags::X)),
    y_ (fetchAttribute<int> (Tags::Y))
{
    jassert (owner);
    
    auto f = []()
    {
        
    };
    
    boxBackgroundColour_.attach (repaint (owner_));
    boxTextColour_.attach (repaint (owner_));
    x_.attach (f);
    y_.attach (f);
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

juce::Rectangle<float> PainterPolicy::paintLabel (juce::Rectangle<float> r, juce::Graphics& g)
{
    const juce::Rectangle<float> t (r.removeFromRight (labelWidth_));
    
    g.setColour (boxBackgroundColour_.get());
    g.fillRect (t);
    g.setColour (boxTextColour_.get());
    g.setFont (Spaghettis()->getLookAndFeel().getConsoleFont());
    g.drawText (owner_->getLabel(), t, juce::Justification::centredLeft, true);
    
    return r;
}
    
juce::Rectangle<float> PainterPolicy::getRequiredBoundsWithLabel (juce::Rectangle<float> r)
{
    const juce::Font font (Spaghettis()->getLookAndFeel().getConsoleFont());
    
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
