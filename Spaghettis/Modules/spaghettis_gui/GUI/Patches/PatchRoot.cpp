
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void releaseWindow (std::vector<std::unique_ptr<PatchWindow>>& v, PatchWindow* window)
{
    auto f = [w = window](const std::unique_ptr<PatchWindow>& p)
    {
        return (p.get() == w);
    };
        
    v.erase (std::remove_if (v.begin(), v.end(), f), v.end());
}

void releaseAllWindows (std::vector<std::unique_ptr<PatchWindow>>& v)
{
    v.clear();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PatchRoot::PatchRoot (const core::Report& v) :
    rootTree_ (v.asValueTree()),
    dirty_ (core::Patch (rootTree_).getCached<bool> (Tag::Attributes, Tag::Dirty)),
    path_ (core::Patch (rootTree_).getCached<juce::String> (Tag::Attributes, Tag::Path)),
    presets_ (rootTree_, getFile())
{
    dirty_.attach ([this]() { setDirtyFlagIfRequired(); });
    
    openMainWindow();
}

PatchRoot::~PatchRoot()
{
    Spaghettis()->appendRecentFile (getFile());
        
    releaseAllWindows (windows_);
        
    // DBG (data::Data::toDebugString (rootTree_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool PatchRoot::isDirty() const
{
    return dirty_.get();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

core::UniqueId PatchRoot::getIdentifier() const
{
    return core::Patch (rootTree_).getIdentifier();
}

juce::File PatchRoot::getFile() const
{
    return juce::File (path_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PresetsManager& PatchRoot::getPresetsManager()
{
    return presets_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::save()
{
    presets_.write();
    
    Broadcast::savePatch (getIdentifier());
}
    
void PatchRoot::close (bool saveFirst)
{
    if (saveFirst) { save(); }
    
    Broadcast::closePatch (getIdentifier());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchRoot::openMainWindow()
{
    if (Spaghettis()->getPreferences().get<bool> (Tag::General, Tag::DefaultIsRunView)) {
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
            windows_.push_back (std::make_unique<EditWindow> (PatchBase (*this, rootTree_)));
        } else {
            juce::ValueTree t (Tree::findChild (rootTree_, i));
            jassert (t.isValid());
            windows_.push_back (std::make_unique<EditWindow> (PatchBase (*this, t)));
        }
    }
    
    setDirtyFlagIfRequired();
}

void PatchRoot::showRunWindow()
{
    RunWindow* w = fetchRunWindow();
    
    if (w) { w->toFront (true); }
    else {
        windows_.push_back (std::make_unique<RunWindow> (PatchBase (*this, rootTree_)));
    }
    
    setDirtyFlagIfRequired();
}

void PatchRoot::closeWindowButtonPressed (PatchWindow* w)
{
    if (windows_.size() > 1) { releaseWindow (windows_, w); }
    else {
    //
    Spaghettis()->getPatches().requestClosePatch (getIdentifier(), CloseType::yesNoCancel);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Component* PatchRoot::fetchMainWindow() const
{
    jassert (windows_.empty() == false);
    
    if (auto p = fetchEditWindow (getIdentifier())) { return dynamic_cast<juce::Component*> (p); }
    else if (auto q = fetchRunWindow())             { return dynamic_cast<juce::Component*> (q); }
    else {
        return dynamic_cast<juce::Component*> (windows_.front().get());
    }
}

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
    for (const auto& p : windows_) { p->setDirtyFlag (isDirty()); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
