
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

bool PatchRoot::hasChild (const core::UniquePath& u) const
{
    juce::ValueTree parent (getParent (u));
    
    if (parent.isValid()) { return Tree::getChild (parent, u.getIdentifier()).isValid(); }
    
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::add (const core::UniquePath& u, const core::Report& v)
{
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
    DBG (v.debug());
    
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
    jassert (!u.isRoot());
    
    juce::ValueTree parent (getParent (u));
    juce::ValueTree child (Tree::getChild (parent, u.getIdentifier()));
    
    if (child.isValid()) { parent.removeChild (child, nullptr); }
}

void PatchRoot::rename (const core::UniquePath& u, core::UniqueId i)
{
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

bool PatchRoot::locate (const core::UniquePath& u)
{
    if (contains (u)) {
    //
    const core::UniqueId parent = core::Patch (getParent (u)).getIdentifier();
    const core::UniqueId object = u.getIdentifier();
    
    showEditWindow (parent);
    
    if (auto p = fetchEditWindow (parent)) { return p->locate (object); }
    //
    }
    
    return false;
}

bool PatchRoot::contains (const core::UniquePath& u)
{
    jassert (!u.isRoot()); return hasChild (u);
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

void PatchRoot::registerWindowBounds (core::UniqueId u, core::Point::Real pt, int zoom)
{
    bounds_.set ({ u, pt, zoom });
}

core::Point::Real PatchRoot::getOffset (const EditView& view) const
{
    return std::get<WindowBounds::BOUNDS_POINT> (bounds_.get (view.getIdentifier()));
}

int PatchRoot::getZoom (const EditView& view) const
{
    return std::get<WindowBounds::BOUNDS_ZOOM> (bounds_.get (view.getIdentifier()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::openMainWindow()
{
    if (Spaghettis()->getPreferences().getCached<bool> (Tag::General, Tag::DefaultIsRunView)) {
        openMainRunWindow();
    } else {
        openMainEditWindow();
    }
}

void PatchRoot::openMainEditWindow()
{
    showEditWindow (getIdentifier());
}

void PatchRoot::openMainRunWindow()
{
    showRunWindow();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::showEditWindow (core::UniqueId i)
{
    EditWindow* w = fetchEditWindow (i);
    
    if (w) { w->toFront (true); }
    else {
        if (getIdentifier() == i) {
            windows_.push_back (std::make_unique<EditWindow> (*this, rootTree_));
        } else {
            juce::ValueTree t (Tree::findChild (rootTree_, i));
            jassert (t.isValid());
            windows_.push_back (std::make_unique<EditWindow> (*this, t));
        }
    }
    
    updateDirty();
}

void PatchRoot::showRunWindow()
{
    RunWindow* w = fetchRunWindow();
    
    if (w) { w->toFront (true); }
    else {
        windows_.push_back (std::make_unique<RunWindow> (*this, rootTree_));
    }
    
    updateDirty();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
    
    if (auto p = fetchEditWindow (getIdentifier())) { return dynamic_cast<juce::Component*> (p); }
    else if (auto q = fetchRunWindow())             { return dynamic_cast<juce::Component*> (q); }
    else {
        return dynamic_cast<juce::Component*> (windows_.front().get());
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

EditWindow* PatchRoot::fetchEditWindow (core::UniqueId i) const
{
    for (const auto& p : windows_) {
        if (p->getIdentifier() == i) {
            auto w = dynamic_cast<EditWindow*> (p.get());
            if (w) {
                return w;
            }
        }
    }
    
    return nullptr;
}

RunWindow* PatchRoot::fetchRunWindow() const
{
    for (const auto& p : windows_) {
        auto w = dynamic_cast<RunWindow*> (p.get());
        if (w) {
            return w;
        }
    }
    
    return nullptr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::setDirtyFlagIfRequired() const
{
    for (const auto& p : windows_) { p->setDirtyFlag (dirty_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
