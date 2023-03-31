
/* Copyright (c) 2021 Jojo and others. */

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

auto isSamePatchAs (core::UniqueId identifier)
{
    return [i = identifier] (const std::shared_ptr<PatchRoot>& p)
    {
        return (p->getIdentifier() == i);
    };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::shared_ptr<PatchRoot> fetchPatch (std::vector<std::shared_ptr<PatchRoot>>& v, core::UniqueId i)
{
    auto r = std::find_if (v.cbegin(), v.cend(), isSamePatchAs (i));
    
    return std::shared_ptr<PatchRoot> (r != v.cend() ? *r : nullptr);
}

void removePatch (std::vector<std::shared_ptr<PatchRoot>>& v, core::UniqueId i)
{
    v.erase (std::remove_if (v.begin(), v.end(), isSamePatchAs (i)), v.end());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

CloseResult getCloseResult (CloseType notify, int result)
{
    if (notify == CloseType::yesNo) {
        switch (result) {
            case 0  : return CloseResult::no;
            case 1  : return CloseResult::yes;
            default : return CloseResult::no;
        }
    } else {
        switch (result) {
            case 0  : return CloseResult::cancel;
            case 1  : return CloseResult::yes;
            case 2  : return CloseResult::no;
            default : return CloseResult::no;
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> bool perform (std::vector<std::shared_ptr<PatchRoot>>& v, const core::UniquePath& u, T f)
{
    auto r = std::find_if (v.cbegin(), v.cend(), isSamePatchAs (u.getRoot()));
    
    if (r != v.cend()) { f (*r); return true; }
    else {
        return false;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchesHolder::add (const core::UniquePath& u, const core::Report& v)
{
    if (u.isRoot()) { roots_.push_back (std::make_shared<PatchRoot> (v)); }
    else {
        perform (roots_, u, [&] (const std::shared_ptr<PatchRoot>& p) { p->add (u, v); });
    }
}

void PatchesHolder::change (const core::UniquePath& u, const core::Report& v)
{
    perform (roots_, u, [&] (const std::shared_ptr<PatchRoot>& p) { p->change (u, v); });
}

void PatchesHolder::remove (const core::UniquePath& u)
{
    if (u.isRoot()) { requestClosePatch (u.getRoot(), CloseType::none); }
    else {
        perform (roots_, u, [&] (const std::shared_ptr<PatchRoot>& p) { p->remove (u); });
    }
}

void PatchesHolder::rename (const core::UniquePath& u, core::UniqueId i)
{
    perform (roots_, u, [&] (const std::shared_ptr<PatchRoot>& p) { p->rename (u, i); });
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchesHolder::locate (const core::UniquePath& u)
{
    if (u.isValid() && !u.isRoot()) {
    //
    perform (roots_, u, [&] (const std::shared_ptr<PatchRoot>& p) { p->locate (u); });
    //
    }
}

bool PatchesHolder::contains (const core::UniquePath& u)
{
    if (u.isValid() && !u.isRoot()) {
    //
    std::shared_ptr<PatchRoot> p (fetchPatch (roots_, u.getRoot()));
    
    if (p) { return p->contains (u); }
    //
    }
    
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchesHolder::setOrder (const core::UniquePath& u, const std::vector<core::UniqueId>& v)
{
    perform (roots_, u, [&] (const std::shared_ptr<PatchRoot>& p) { p->setOrder (u, v); });
}

void PatchesHolder::setDirty (const core::UniquePath& u, bool isDirty)
{
    perform (roots_, u, [&] (const std::shared_ptr<PatchRoot>& p) { p->setDirty (isDirty); });
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchesHolder::showSaveRequest (const std::shared_ptr<PatchRoot>& p, CloseType notify)
{
    juce::Component* window = p->getMainWindow();
    
    jassert (window);
    
    window->toFront (false);
    
    auto f = [notify, i = p->getIdentifier()] (int result)
    {
        Spaghettis()->getPatches().handleSaveRequest (i, getCloseResult (notify, result));
    };

    if (notify == CloseType::yesNoCancel) {
        juce::NativeMessageBox::showYesNoCancelBox (juce::MessageBoxIconType::QuestionIcon,
            p->getFile().getFileName(),
            NEEDS_TRANS ("Save the patch before closing?"),
            window,
            juce::ModalCallbackFunction::create (f));
    
    } else {
        juce::NativeMessageBox::showYesNoBox (juce::MessageBoxIconType::QuestionIcon,
            p->getFile().getFileName(),
            NEEDS_TRANS ("Save the patch before closing?"),
            window,
            juce::ModalCallbackFunction::create (f));
    }
    
    requests_.push_back (p);
}

void PatchesHolder::handleSaveRequest (core::UniqueId i, CloseResult result)
{
    std::shared_ptr<PatchRoot> p (fetchPatch (requests_, i));
    
    if (p) {
    //
    removePatch (requests_, i);
    
    if (result == CloseResult::cancel) { roots_.push_back (p); }
    else {
        p->close (result == CloseResult::yes ? true : false);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool PatchesHolder::isEmpty() const
{
    return roots_.empty();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* To avoid bad loops remove first the patch from the roots before to release it. */

void PatchesHolder::requestClosePatch (core::UniqueId i, CloseType notify)
{
    std::shared_ptr<PatchRoot> p (fetchPatch (roots_, i));
            
    if (p) {
    //
    removePatch (roots_, i);
    
    if (p->isDirty()) { showSaveRequest (p, notify); } else { p->close(); }
    //
    }
}

void PatchesHolder::closeAllPatches()
{
    std::vector<core::UniqueId> t;
    
    auto f = [] (const std::shared_ptr<PatchRoot>& p)
    {
        return p->getIdentifier();
    };
    
    std::transform (roots_.cbegin(), roots_.cend(), std::back_inserter (t), f);
    
    for (const auto& i : t) { requestClosePatch (i, CloseType::yesNo); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
