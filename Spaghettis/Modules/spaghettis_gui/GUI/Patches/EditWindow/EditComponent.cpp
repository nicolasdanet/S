
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditComponent::EditComponent (Patch& owner, const juce::ValueTree& tree) :
    EditFactoryHelper (this),
    BaseComponent (getIconsFactory()),
    owner_ (owner),
    editView_ (tree),
    editPort_ (editView_)
{
    addAndMakeVisible (editPort_);
    addChildComponent (zoomComponent_);
    
    addMenuBarCommand (Commands::zoomIn,    [this]() { zoomIn();    } );
    addMenuBarCommand (Commands::zoomOut,   [this]() { zoomOut();   } );
    addMenuBarCommand (Commands::zoomReset, [this]() { zoomReset(); } );
    
    setOpaque (true); setSize (600, 300);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditComponent::paint (juce::Graphics& g)
{
    g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
}

void EditComponent::resized()
{
    juce::Rectangle<int> bounds (setBoundsForBarsAndGetRemaining());
    
    editPort_.setBounds (bounds);
    
    updateZoomComponent();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditComponent::zoomIn()
{
    editPort_.zoomIn();
}

void EditComponent::zoomOut()
{
    editPort_.zoomOut();
}

void EditComponent::zoomReset()
{
    editPort_.zoomReset();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditComponent::updateZoomComponent()
{
    const int n = editPort_.getZoom();
    
    juce::Rectangle<int> b (getBoundsForToolbar());
    
    if (b.getWidth() < 300) { zoomComponent_.setVisible (false); }
    else {
    //
    zoomComponent_.setValue (n);
    zoomComponent_.setBounds (b.removeFromRight (80).reduced (10, 0));
    zoomComponent_.setVisible (true);
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
