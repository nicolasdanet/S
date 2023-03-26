
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree PatchRoot::getParent (const core::UniquePath& u) const
{
    juce::ValueTree t (rootTree_);
    
    if (u.hasPath()) { for (const auto& i : u.getPath()) { t = Tree::getChild (t, i); } }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::add (const core::UniquePath& u, const core::Report& v)
{
    // DBG ("### ADD"); DBG (v.debug());
    
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (u));
    
    if (v.isPatch()) {
        juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
        if (child.isValid()) {
            core::Patch (child).apply (v); return;      /* Two-step creation for subpatches. */
        }
    }
    
    jassert (Tree::getChild (parent, u.getIdentifier()).isValid() == false);
    
    parent.appendChild (v.asValueTree(), nullptr);
}

void PatchRoot::change (const core::UniquePath& u, const core::Report& v)
{
    // DBG ("### CHANGE"); DBG (v.debug());
        
    juce::ValueTree parent (getParent (u));
    
    if (u.isRoot()) { core::Patch (parent).apply (v); }
    else {
    //
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));

    if (child.isValid()) {
        if (Tree::isLine (child)) { core::Line (child).apply (v); }
        else {
            core::Object (child).apply (v);
        }
    }
    //
    }
}

void PatchRoot::remove (const core::UniquePath& u)
{
    // DBG ("### REMOVE"); DBG (u.debug());
        
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (u));
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
    
    if (child.isValid()) { parent.removeChild (child, nullptr); }
}

void PatchRoot::rename (const core::UniquePath& u, core::UniqueId i)
{
    // DBG ("### RENAME"); DBG (u.debug());
        
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (u));
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));

    if (child.isValid()) {
        core::Object (child).changeIdentifier (i);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::localize (const core::UniquePath& u)
{
    DBG (u.debug());
        
    jassert (!u.isRoot());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::setOrder (const core::UniquePath& u, const std::vector<core::UniqueId>& v)
{
    juce::ValueTree parent (getParent (u));
    
    if (u.isRoot()) { core::Patch (parent).sortObjects (v); }
    else {
    //
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
    
    if (child.isValid()) {
        core::Patch (child).sortObjects (v);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::setDirty (bool isDirty)
{
    dirty_ = isDirty; setDirtyFlagIfRequired();
}

void PatchRoot::updateDirty() const
{
    setDirtyFlagIfRequired();
}

bool PatchRoot::isDirty() const
{
    return dirty_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::openWindow()
{
    if (Spaghettis()->getPreferences().getCached<bool> (Tag::General, Tag::DefaultIsRunView)) {
        openRunWindow();
    } else {
        openEditWindow();
    }
}

void PatchRoot::openEditWindow()
{
    windows_.push_back (std::make_unique<EditWindow> (*this, rootTree_));
    
    updateDirty();
}

void PatchRoot::openRunWindow()
{
    windows_.push_back (std::make_unique<RunWindow> (*this, rootTree_));
    
    updateDirty();
}

void PatchRoot::showEditWindow (core::UniqueId i)
{
    juce::ValueTree t (Tree::findChild (rootTree_, i));
    
    jassert (t.isValid());
    
    windows_.push_back (std::make_unique<EditWindow> (*this, t));
    
    updateDirty();
}

void PatchRoot::closeWindowButtonPressed (PatchWindow* w)
{
    if (windows_.size() > 1) { removeWindow (w); }
    else {
    //
    Spaghettis()->getPatches().requestClosePatch (getIdentifier(), CloseType::yesNoCancel);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::removeWindow (PatchWindow* window)
{
    auto f = [w = window](const std::unique_ptr<PatchWindow>& p)
    {
        return (p.get() == w);
    };
        
    windows_.erase (std::remove_if (windows_.begin(), windows_.end(), f), windows_.end());
}

void PatchRoot::releaseAllWindows()
{
    windows_.clear();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Component* PatchRoot::getMainWindow() const
{
    jassert (windows_.empty() == false);
    
    for (const auto& p : windows_) {
        if (p->isTopPatchWindow()) { return dynamic_cast<juce::Component*> (p.get()); }
    }
    
    return dynamic_cast<juce::Component*> (windows_.front().get());
}

void PatchRoot::setDirtyFlagIfRequired() const
{
    for (const auto& p : windows_) { p->setDirtyFlag (dirty_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
