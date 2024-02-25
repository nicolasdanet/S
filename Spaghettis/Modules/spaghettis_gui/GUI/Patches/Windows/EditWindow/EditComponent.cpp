
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

EditComponent::EditComponent (const PatchBase& base) :
    EditFactoryHelper (this),
    BaseComponent (getIconsFactory(), Spaghettis()->getMenu(), Spaghettis()->getCommandManager()),
    editView_ (base),
    editPort_ (editView_,
        editView_.getPatchRoot().getPlacements().get (editView_.getIdentifier()).getOffset(),
        editView_.getPatchRoot().getPlacements().get (editView_.getIdentifier()).getZoom()),
    editZoom_ (editPort_.getZoomAsValue()),
    editInspector_ (editView_)
{
    CommandsHandler::addCloseWindowCommand (this);
    
    addAndMakeVisible (editPort_);
    addChildComponent (editZoom_);
    addChildComponent (editInspector_);
    
    if (editView_.isAbstractionOrInside() == false) {
    //
    auto f = [this]() { return editView_.hasSelectedObject(); };
    
    addMenuCommand (MenuCommand (Commands::undo)
        .setInvoke ([this] (const auto&) { editView_.undo(); })
        .setCheck  ([this]() { return editView_.hasUndo(); })
        .setName   ([this]() { return editView_.getUndoAction(); }));
        
    addMenuCommand (MenuCommand (Commands::redo)
        .setInvoke ([this] (const auto&) { editView_.redo(); })
        .setCheck  ([this]() { return editView_.hasRedo(); })
        .setName   ([this]() { return editView_.getRedoAction(); }));
    
    addMenuCommand (MenuCommand (Commands::cut)
        .setInvoke ([this] (const auto&) { editView_.cut(); })
        .setCheck  (f));
        
    addMenuCommand (MenuCommand (Commands::copy)
        .setInvoke ([this] (const auto&) { editView_.copy(); })
        .setCheck  (f));
        
    addMenuCommand (MenuCommand (Commands::duplicate)
        .setInvoke ([this] (const auto&) { editView_.duplicate(); })
        .setCheck  (f));
        
    addMenuCommand (MenuCommand (Commands::remove)
        .setInvoke ([this] (const auto&) { editView_.remove(); })
        .setCheck  ([this]() { return editView_.hasSelected(); }));
        
    addMenuCommand (MenuCommand (Commands::paste)
        .setInvoke ([this] (const auto&) { editView_.paste(); })
        .setCheck  ([this]() { return editView_.hasPaste(); }));
    
    addMenuCommand (MenuCommand (Commands::addObject)
        .setInvoke ([this] (const auto& i) { editView_.makerRequire (MenuCommand::isFromMenu (i)); }));
    
    addMenuCommand (MenuCommand (Commands::encapsulate)
        .setInvoke ([this] (const auto&) { editView_.encapsulate(); })
        .setCheck  (f));
        
    addMenuCommand (MenuCommand (Commands::deencapsulate)
        .setInvoke ([this] (const auto&) { editView_.deencapsulate(); })
        .setCheck  ([this]() { return editView_.hasOnlyOnePatchSelected(); }));
    
    addMenuCommand (MenuCommand (Commands::moveBack)
        .setInvoke ([this] (const auto&) { editView_.moveBack(); })
        .setCheck  (f));
        
    addMenuCommand (MenuCommand (Commands::moveFront)
        .setInvoke ([this] (const auto&) { editView_.moveFront(); })
        .setCheck  (f));
        
    addMenuCommand (MenuCommand (Commands::snap)
        .setInvoke ([this] (const auto&) { editView_.snapToGrid(); })
        .setCheck  (f));
    //
    }

    addMenuCommand (MenuCommand (Commands::save)
        .setInvoke ([this] (const auto&) { editView_.getPatchRoot().save(); }));
        
    addMenuCommand (MenuCommand (Commands::selectAll)
        .setInvoke ([this] (const auto&) { editView_.selectAll(); }));
    
    addMenuCommand (MenuCommand (Commands::zoomIn)
        .setInvoke ([this] (const auto&) { zoomIn(); }));
        
    addMenuCommand (MenuCommand (Commands::zoomOut)
        .setInvoke ([this] (const auto&) { zoomOut(); }));
        
    addMenuCommand (MenuCommand (Commands::zoomReset)
        .setInvoke ([this] (const auto&) { zoomReset(); } ));
    
    addMenuCommand (MenuCommand (Commands::inspector)
        .setInvoke ([this] (const auto&) { toggleInspector(); })
        .setName   ([this]() { return getInspectorMenuText (editInspector_); }));
    
    addMenuCommand (MenuCommand (Commands::newRunView)
        .setInvoke ([this] (const auto&) { editView_.getPatchRoot().openMainRunWindow(); }));
    
    addMenuCommand (MenuCommand (Commands::newEditView)
        .setInvoke ([this] (const auto&) { editView_.getPatchRoot().openMainEditWindow(); }));
        
    setOpaque (true); setSize (600, 300);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PatchRoot& EditComponent::getPatchRoot()
{
    return editView_.getPatchRoot();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditComponent::paint (juce::Graphics& g)
{
    g.fillAll (Colours::fetchColour (Colours::windowsBackground));
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

bool EditComponent::locate (core::UniqueId u)
{
    return editPort_.locate (u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void EditComponent::showInspector()
{
    editInspector_.setActive (true);
    updateLayout();
    Spaghettis()->updateMenuBar();
    editPort_.updateInspector();
}

void EditComponent::hideInspector()
{
    editInspector_.setActive (false);
    updateLayout();
    Spaghettis()->updateMenuBar();
    editPort_.updateInspector();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void EditComponent::toggleInspector()
{
    if (toggleButtonState (Icons::inspector)) { showInspector(); } else { hideInspector(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool EditComponent::tryGrabFocus()
{
    return tryGrabFocusForComponent (this);
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
    
    updateZoom (editZoom_, getToolbarBounds());
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
