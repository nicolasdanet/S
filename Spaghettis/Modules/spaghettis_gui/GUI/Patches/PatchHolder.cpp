
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

auto hasEqualRoot (const core::Unique& u)
{
    return [i = u.getRoot()] (const std::shared_ptr<Patch>& p)
    {
        return (p->getUnique().getRoot() == i);
    };
}
    
auto toUnique()
{
    return [] (const std::shared_ptr<Patch>& p)
    {
        return p->getUnique();
    };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::shared_ptr<Patch> fetchPatch (std::vector<std::shared_ptr<Patch>>& v, const core::Unique& u)
{
    auto r = std::find_if (v.cbegin(), v.cend(), hasEqualRoot (u));
    
    return std::shared_ptr<Patch> (r != v.cend() ? *r : nullptr);
}

void removePatch (std::vector<std::shared_ptr<Patch>>& v, const core::Unique& u)
{
    v.erase (std::remove_if (v.begin(), v.end(), hasEqualRoot (u)), v.end());
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
// MARK: -

template <class T> void PatchHolder::perform (const core::Unique& u, T f) const
{
    auto r = std::find_if (roots_.cbegin(), roots_.cend(), hasEqualRoot (u));
    
    if (r != roots_.cend()) { f (*r); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchHolder::addObject (const core::Unique& u, const core::Description& v)
{
    if (u.isRoot()) { roots_.push_back (std::make_shared<Patch> (u, v)); }
    else {
        perform (u, [&] (const std::shared_ptr<Patch>& p) { p->addObject (u, v); });
    }
}

void PatchHolder::changeObject (const core::Unique& u, const core::Description& v)
{
    if (u.isRoot()) { }
    else {
        perform (u, [&] (const std::shared_ptr<Patch>& p) { p->changeObject (u, v); });
    }
}

void PatchHolder::removeObject (const core::Unique& u)
{
    if (u.isRoot()) { requestClosePatch (u, CloseType::none); }
    else {
        perform (u, [&] (const std::shared_ptr<Patch>& p) { p->removeObject (u); });
    }
}

void PatchHolder::renameObject (const core::Unique& u, core::Unique::Identifier i)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchHolder::setDirty (const core::Unique& u, bool isDirty)
{
    perform (u, [&] (const std::shared_ptr<Patch>& p) { p->setDirty (isDirty); });
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchHolder::showSaveRequest (const std::shared_ptr<Patch>& p, CloseType notify)
{
    auto f = [notify, u = p->getUnique()] (int result)
    {
        Spaghettis()->getPatches().handleSaveRequest (u, getCloseResult (notify, result));
    };

    if (notify == CloseType::yesNoCancel) {
        juce::NativeMessageBox::showYesNoCancelBox (juce::MessageBoxIconType::QuestionIcon,
            p->getFile().getFileName(),
            NEEDS_TRANS ("Save the patch before closing?"),
            p->getMainWindow(),
            juce::ModalCallbackFunction::create (f));
    
    } else {
        juce::NativeMessageBox::showYesNoBox (juce::MessageBoxIconType::QuestionIcon,
            p->getFile().getFileName(),
            NEEDS_TRANS ("Save the patch before closing?"),
            p->getMainWindow(),
            juce::ModalCallbackFunction::create (f));
    }
    
    requests_.push_back (p);
}

void PatchHolder::handleSaveRequest (const core::Unique& u, CloseResult result)
{
    std::shared_ptr<Patch> p (fetchPatch (requests_, u));
    
    if (p) {
    //
    removePatch (requests_, u);
    
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

/* To avoid bad loops remove first the patch from the roots before to release it. */

void PatchHolder::requestClosePatch (const core::Unique& u, CloseType notify)
{
    std::shared_ptr<Patch> p (fetchPatch (roots_, u));
            
    if (p) {
    //
    removePatch (roots_, u);
    
    if (p->isDirty()) { showSaveRequest (p, notify); } else { p->close(); }
    //
    }
}

void PatchHolder::closeAllPatches()
{
    std::vector<core::Unique> t;
    
    std::transform (roots_.cbegin(), roots_.cend(), std::back_inserter (t), toUnique());
    
    for (const auto& u : t) { requestClosePatch (u, CloseType::yesNo); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
