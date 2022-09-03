
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Patch::getParent (const core::UniquePath& u) const
{
    juce::ValueTree t (rootTree_);
    
    if (u.hasPath()) { for (const auto& i : u.getPath()) { t = Tree::getChild (t, i); } }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::setOrder (const core::UniquePath& u, const std::vector<core::UniqueId>& v)
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

void Patch::add (const core::UniquePath& u, const core::Report& v)
{
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (u));
    
    if (v.isPatch()) {
        juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
        if (child.isValid()) {
            core::Patch (child).copyFrom (v); return;      /* Two-step creation for subpatches. */
        }
    }
    
    jassert (Tree::getChild (parent, u.getIdentifier()).isValid() == false);
    
    parent.appendChild (v.asValueTree(), nullptr);
}

void Patch::change (const core::UniquePath& u, const core::Report& v)
{
    juce::ValueTree parent (getParent (u));
    
    if (u.isRoot()) { core::Patch (parent).copyFrom (v); }
    else {
    //
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));

    if (child.isValid()) {
        if (Tree::isLine (child)) { core::Line (child).copyFrom (v); }
        else {
            core::Object (child).copyFrom (v);
        }
    }
    //
    }
}

void Patch::remove (const core::UniquePath& u)
{
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (u));
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
    
    if (child.isValid()) { parent.removeChild (child, nullptr); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::setDirty (bool isDirty)
{
    dirty_ = isDirty; setDirtyFlagIfRequired();
}

void Patch::updateDirty() const
{
    setDirtyFlagIfRequired();
}

bool Patch::isDirty() const
{
    return dirty_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::openWindow()
{
    if (Spaghettis()->getPreferences().getCached<bool> (Tags::General, Tags::DefaultIsRunView)) {
        openRunWindow();
    } else {
        openEditWindow();
    }
}

void Patch::openSubPatchWindow (core::UniqueId i)
{
    juce::ValueTree t (Tree::findChild (rootTree_, i));
    
    jassert (t.isValid());
    
    windows_.push_back (std::make_unique<EditWindow> (*this, t));
    
    updateDirty();
}

void Patch::openEditWindow()
{
    windows_.push_back (std::make_unique<EditWindow> (*this, rootTree_));
    
    updateDirty();
}

void Patch::openRunWindow()
{
    windows_.push_back (std::make_unique<RunWindow> (*this, rootTree_));
    
    updateDirty();
}

void Patch::closeWindowButtonPressed (PatchWindow* w)
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

void Patch::removeWindow (PatchWindow* w)
{
    auto f = [window = w](const std::unique_ptr<PatchWindow>& p)
    {
        return (p.get() == window);
    };
        
    windows_.erase (std::remove_if (windows_.begin(), windows_.end(), f), windows_.end());
}

void Patch::releaseAllWindows()
{
    windows_.clear();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Component* Patch::getMainWindow() const
{
    jassert (windows_.empty() == false);
    
    for (const auto& p : windows_) {
        if (!p->isSubPatch()) { return dynamic_cast<juce::Component*> (p.get()); }
    }
    
    return dynamic_cast<juce::Component*> (windows_.front().get());
}

void Patch::setDirtyFlagIfRequired() const
{
    for (const auto& p : windows_) { p->setDirtyFlag (dirty_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
