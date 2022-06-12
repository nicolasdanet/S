
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PainterPolicy::PainterPolicy (ObjectComponent* owner, const core::Object& object) :
    owner_ (owner),
    object_ (object),
    x_ (fetchAttribute<int> (Tags::X)),
    y_ (fetchAttribute<int> (Tags::Y))
{
    jassert (owner);
    
    auto f = []()
    {
        
    };
    
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
// MARK: -

juce::Rectangle<float> PainterPolicy::paintLabel (juce::Rectangle<float> r, juce::Graphics& g)
{
    return r;
}
    
juce::Rectangle<float> PainterPolicy::getRequiredBoundsWithLabel (juce::Rectangle<float> r)
{
    return r;
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
