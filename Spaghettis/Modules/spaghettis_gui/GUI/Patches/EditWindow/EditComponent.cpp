
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::String getInspectorMenuText (const EditInspector& i)
{
    return i.isActive() ? juce::String ("Hide Inspector") : juce::String ("Show Inspector");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditComponent::EditComponent (PatchRoot& patch, const juce::ValueTree& tree) :
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
    
    if (editView_.isAbstractionOrInside() == false) {
    //
    auto f = [this]() { return editView_.hasSelectedObject(); };
    
    addMenuCommand (MenuCommand (Commands::undo,            [this]() { editView_.undo(); },
                                                            [this]() { return editView_.hasUndo(); },
                                                            [this]() { return editView_.getUndoAction(); }));
    addMenuCommand (MenuCommand (Commands::redo,            [this]() { editView_.redo(); },
                                                            [this]() { return editView_.hasRedo(); },
                                                            [this]() { return editView_.getRedoAction(); }));
    
    addMenuCommand (MenuCommand (Commands::cut,             [this]() { editView_.cut(); }, f));
    addMenuCommand (MenuCommand (Commands::copy,            [this]() { editView_.copy(); }, f));
    addMenuCommand (MenuCommand (Commands::duplicate,       [this]() { editView_.duplicate(); }, f));
    addMenuCommand (MenuCommand (Commands::remove,          [this]() { editView_.remove(); },
                                                            [this]() { return editView_.hasSelected(); }));
    addMenuCommand (MenuCommand (Commands::paste,           [this]() { editView_.paste(); },
                                                            [this]() { return editView_.hasPaste(); }));
    
    addMenuCommand (MenuCommand (Commands::addObject,       [this]() { editView_.requestNewObject(); }));
    
    addMenuCommand (MenuCommand (Commands::encapsulate,     [this]() { editView_.encapsulate(); }, f));
    addMenuCommand (MenuCommand (Commands::deencapsulate,   [this]() { editView_.deencapsulate(); },
                                                            [this]()
                                                            {
                                                                return editView_.hasOnlyOnePatchSelected();
                                                            }
                                                            ));
    
    addMenuCommand (MenuCommand (Commands::moveBack,        [this]() { editView_.moveBack(); }, f));
    addMenuCommand (MenuCommand (Commands::moveFront,       [this]() { editView_.moveFront(); }, f));
    addMenuCommand (MenuCommand (Commands::snap,            [this]() { editView_.snapToGrid(); }, f));
    //
    }

    addMenuCommand (MenuCommand (Commands::save,            [this]() { editView_.getPatchRoot().save(); }));
    addMenuCommand (MenuCommand (Commands::selectAll,       [this]() { editView_.selectAll(); }));
    
    addMenuCommand (MenuCommand (Commands::zoomIn,          [this]() { zoomIn(); } ));
    addMenuCommand (MenuCommand (Commands::zoomOut,         [this]() { zoomOut(); } ));
    addMenuCommand (MenuCommand (Commands::zoomReset,       [this]() { zoomReset(); } ));
    
    addMenuCommand (MenuCommand (Commands::inspector,       [this]() { toggleInspector(); },
                                                            []() { return true; },
                                                            [this]()
                                                            {
                                                                return getInspectorMenuText (editInspector_);
                                                            }
                                                            ));
                                                            
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
    editInspector_.setActive (true);    updateLayout(); Spaghettis()->updateMenuBar();
}

void EditComponent::hideInspector()
{
    editInspector_.setActive (false);   updateLayout(); Spaghettis()->updateMenuBar();
}

void EditComponent::toggleInspector()
{
    if (toggleButtonState (Icons::inspector)) { showInspector(); } else { hideInspector(); }
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
