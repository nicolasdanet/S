
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::shared_ptr<Patch> PatchHolder::fetchPatch (const core::Unique& u) const
{
    auto r = std::find_if (roots_.cbegin(), roots_.cend(), hasEqualRoot (u));
    
    return std::shared_ptr<Patch> (r != roots_.cend() ? *r : nullptr);
}

void PatchHolder::removePatch (const core::Unique& u)
{
    roots_.erase (std::remove_if (roots_.begin(), roots_.end(), hasEqualRoot (u)), roots_.end());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::MessageBoxOptions getSaveRequestOptions (const std::shared_ptr<Patch>& p, CloseType notify)
{
    if (notify == CloseType::yesNoCancel) {
        return juce::MessageBoxOptions().withTitle (p->getFile().getFileName())
            .withAssociatedComponent (p->getMainWindow())
            .withMessage (NEEDS_TRANS ("Save the patch before closing?"))
            .withButton (NEEDS_TRANS ("Yes"))
            .withButton (NEEDS_TRANS ("No"))
            .withButton (NEEDS_TRANS ("Cancel"));
    
    } else {
        return juce::MessageBoxOptions().withTitle (p->getFile().getFileName())
            .withAssociatedComponent (p->getMainWindow())
            .withMessage (NEEDS_TRANS ("Save the patch before closing?"))
            .withButton (NEEDS_TRANS ("Yes"))
            .withButton (NEEDS_TRANS ("No"));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchHolder::showSaveRequest (const std::shared_ptr<Patch>& p, CloseType notify)
{
    auto f = [u = p->getUnique()] (int result)
    {
        const bool save = (result == 0); Spaghettis()->getPatches().handleSaveRequest (u, save);
    };
    
    juce::NativeMessageBox::showAsync (getSaveRequestOptions (p, notify), f);
    
    requests_.push_back (p);
}

void PatchHolder::handleSaveRequest (const core::Unique& u, bool save)
{
    auto p = std::find_if (requests_.cbegin(), requests_.cend(), hasEqualRoot (u));
    
    if (p != requests_.cend()) { p->get()->close (save); requests_.erase (p); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchHolder::createPatch (const core::Unique& u, const core::Description& v)
{
    roots_.push_back (std::make_shared<Patch> (u, v));
}

void PatchHolder::changePatch (const core::Unique& u, const core::Description& v)
{

}

/* To avoid bad loops remove first the patch from the roots before to release it. */

void PatchHolder::requestClosePatch (const core::Unique& u, CloseType notify)
{
    std::shared_ptr<Patch> p (fetchPatch (u));
            
    if (p) {
    //
    removePatch (u);
    
    if (notify == CloseType::yesNoCancel) { showSaveRequest (p, notify); }
    else if (notify == CloseType::yesNo)  {
        if (p->isDirty()) { showSaveRequest (p, notify); } else { p->close(); }
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
