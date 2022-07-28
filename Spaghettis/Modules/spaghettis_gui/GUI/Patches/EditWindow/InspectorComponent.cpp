
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

InspectorComponent::InspectorComponent()
{
    // inspectorResizer_ (&inspectorComponent_, &inspectorConstrainer_, juce::ResizableEdgeComponent::leftEdge),
    
    // addChildComponent (inspectorResizer_);
    // inspectorConstrainer_.setMinimumWidth (50);
    // inspectorConstrainer_.setMaximumWidth (500);
    // inspectorResizer_.setAlwaysOnTop (true);
    
    setOpaque (true); setSize (200, 400);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void InspectorComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

void InspectorComponent::resized()
{
    // inspectorResizer_.setVisible (hasInspector_);
    // inspectorResizer_.setBounds (bounds.withTrimmedLeft (bounds.getWidth() - 4));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
