
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Locator::Locator (const juce::Rectangle<int>& bounds) :
    locatorColour_ (Spaghettis()->getCachedColour (Tag::Locator))
{
    setAlwaysOnTop (true);
    setInterceptsMouseClicks (false, true);
    setBounds (bounds.expanded (10));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Path getDashedRectangle (const juce::Rectangle<int>& r)
{
    juce::Path p;
    
    p.addRectangle (r.reduced (2));
    
    const float dash[2] = { 2.0f, 4.0f };
        
    juce::PathStrokeType (1.0f).createDashedStroke (p, p, dash, juce::numElementsInArray (dash));
    
    return p;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Locator::paint (juce::Graphics& g)
{
    g.setColour (locatorColour_.get());
    
    g.fillPath (getDashedRectangle (getLocalBounds()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------