
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditComponent::EditComponent (Patch& patch, const juce::ValueTree& tree) :
    EditFactoryHelper (this),
    BaseComponent (getIconsFactory()),
    patch_ (patch),
    editView_ (patch, tree),
    editPort_ (editView_),
    editZoom_ (editPort_.getZoomAsValue()),
    hasInspector_ (false)
{
    addAndMakeVisible (editPort_);
    addChildComponent (editZoom_);
    addChildComponent (editInspector_);
    
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
    updateLayout();
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

void EditComponent::showInspector()
{
    hasInspector_ = true;  updateLayout();
}

void EditComponent::hideInspector()
{
    hasInspector_ = false; updateLayout();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void updateZoom (EditZoom& c, juce::Rectangle<int> bounds)
{
    if (bounds.getWidth() < 300) { c.setVisible (false); }
    else {
    //
    c.setBounds (bounds.removeFromRight (80).reduced (15, 0));
    c.setVisible (true);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditComponent::updateLayout()
{
    juce::Rectangle<int> bounds (setBoundsForBarsAndGetRemaining());
    
    if (hasInspector_) {
    //
    const int w = bounds.getWidth() - editInspector_.getWidth();
    
    editInspector_.setBounds (bounds.withTrimmedLeft (w));
    //
    }
    
    editInspector_.setVisible (hasInspector_);

    editPort_.setBounds (bounds);
    
    updateZoom (editZoom_, getBoundsForToolbar());
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
