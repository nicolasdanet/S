
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
    bounds_.set (PatchBoundsElement (u, pt, zoom));
}

core::Point::Real PatchRoot::getOffset (const EditView& view) const
{
    return bounds_.get (view.getIdentifier()).getOffset();
}

int PatchRoot::getZoom (const EditView& view) const
{
    return bounds_.get (view.getIdentifier()).getZoom();
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
