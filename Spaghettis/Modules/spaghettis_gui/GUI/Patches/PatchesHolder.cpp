
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
    return [i = identifier] (const std::shared_ptr<Patch>& p)
    {
        return (p->getIdentifier() == i);
    };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::shared_ptr<Patch> fetchPatch (std::vector<std::shared_ptr<Patch>>& v, core::UniqueId i)
{
    auto r = std::find_if (v.cbegin(), v.cend(), isSamePatchAs (i));
    
    return std::shared_ptr<Patch> (r != v.cend() ? *r : nullptr);
}

void removePatch (std::vector<std::shared_ptr<Patch>>& v, core::UniqueId i)
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
// MARK: -

template <class T> void PatchesHolder::perform (const core::UniquePath& u, T f) const
{
    auto r = std::find_if (roots_.cbegin(), roots_.cend(), isSamePatchAs (u.getRoot()));
    
    if (r != roots_.cend()) { f (*r); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchesHolder::add (const core::UniquePath& u, const core::Report& v)
{
    if (u.isRoot()) { roots_.push_back (std::make_shared<Patch> (v)); }
    else {
        perform (u, [&] (const std::shared_ptr<Patch>& p) { p->add (u, v); });
    }
}

void PatchesHolder::change (const core::UniquePath& u, const core::Report& v)
{
    if (u.isRoot()) { }
    else {
        perform (u, [&] (const std::shared_ptr<Patch>& p) { p->change (u, v); });
    }
}

void PatchesHolder::remove (const core::UniquePath& u)
{
    if (u.isRoot()) { requestClosePatch (u.getRoot(), CloseType::none); }
    else {
        perform (u, [&] (const std::shared_ptr<Patch>& p) { p->remove (u); });
    }
}

void PatchesHolder::rename (const core::UniquePath& u, core::UniqueId i)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchesHolder::setDirty (const core::UniquePath& u, bool isDirty)
{
    perform (u, [&] (const std::shared_ptr<Patch>& p) { p->setDirty (isDirty); });
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchesHolder::showSaveRequest (const std::shared_ptr<Patch>& p, CloseType notify)
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
    std::shared_ptr<Patch> p (fetchPatch (requests_, i));
    
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
    std::shared_ptr<Patch> p (fetchPatch (roots_, i));
            
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
    
    auto f = [] (const std::shared_ptr<Patch>& p)
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
