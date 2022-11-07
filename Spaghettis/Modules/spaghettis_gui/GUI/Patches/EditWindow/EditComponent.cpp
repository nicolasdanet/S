
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
    editView_ (patch, tree),
    editPort_ (editView_),
    editZoom_ (editPort_.getZoomAsValue()),
    editInspector_ (editView_)
{
    addAndMakeVisible (editPort_);
    addChildComponent (editZoom_);
    addChildComponent (editInspector_);
    
    addMenuCommand (MenuCommand (Commands::save,        [this]() { editView_.getPatch().save(); }));
    
    addMenuCommand (MenuCommand (Commands::undo,        [this]() { editView_.undo(); },
                                                        [this]() { return editView_.hasUndo(); },
                                                        [this]() { return editView_.getUndoAction(); }));
                                                        
    addMenuCommand (MenuCommand (Commands::redo,        [this]() { editView_.redo(); },
                                                        [this]() { return editView_.hasRedo(); },
                                                        [this]() { return editView_.getRedoAction(); }));
    
    auto check = [this]() { return editView_.getNumberOfSelectedObjects() > 0; };
    
    addMenuCommand (MenuCommand (Commands::moveBack,    [this]() { editView_.moveBack();   }, check));
    addMenuCommand (MenuCommand (Commands::moveFront,   [this]() { editView_.moveFront();  }, check));
    addMenuCommand (MenuCommand (Commands::selectAll,   [this]() { editView_.selectAll();  }));
    addMenuCommand (MenuCommand (Commands::snap,        [this]() { editView_.snapToGrid(); }, check));
    
    addMenuCommand (MenuCommand (Commands::zoomIn,      [this]() { zoomIn();    } ));
    addMenuCommand (MenuCommand (Commands::zoomOut,     [this]() { zoomOut();   } ));
    addMenuCommand (MenuCommand (Commands::zoomReset,   [this]() { zoomReset(); } ));

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
    editInspector_.setActive (true);    updateLayout();
}

void EditComponent::hideInspector()
{
    editInspector_.setActive (false);   updateLayout();
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
    
    if (editInspector_.isActive()) {
    //
    const int w = bounds.getWidth() - editInspector_.getWidth();
    
    editInspector_.setBounds (bounds.withTrimmedLeft (w));
    //
    }
    
    editInspector_.setVisible (editInspector_.isActive());

    editPort_.setBounds (bounds);
    
    updateZoom (editZoom_, getBoundsForToolbar());
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
