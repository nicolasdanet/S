
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

ActionLassoComponent::ActionLassoComponent (const juce::Rectangle<int>& bounds) :
    patchLassoColour_ (Spaghettis()->getCachedColour (Tag::PatchLasso))
{
    setAlwaysOnTop (true);
    setInterceptsMouseClicks (false, true);
    setBounds (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void ActionLassoComponent::paint (juce::Graphics& g)
{
    g.setColour (patchLassoColour_.get()); g.drawRect (getLocalBounds());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
