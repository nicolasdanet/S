
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

std::shared_ptr<Patch> fetchPatch (std::vector<std::shared_ptr<Patch>>& v, const core::Unique& u)
{
    auto r = std::find_if (v.cbegin(), v.cend(), PatchHolder::hasEqualRoot (u));
    
    return std::shared_ptr<Patch> (r != v.cend() ? *r : nullptr);
}

void removePatch (std::vector<std::shared_ptr<Patch>>& v, const core::Unique& u)
{
    v.erase (std::remove_if (v.begin(), v.end(), PatchHolder::hasEqualRoot (u)), v.end());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

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
        Spaghettis()->getPatches().handleSaveRequest (u, result);
    };
    
    juce::NativeMessageBox::showAsync (getSaveRequestOptions (p, notify), f);
    
    requests_.push_back (p);
}

void PatchHolder::handleSaveRequest (const core::Unique& u, int result)
{
    // auto p = std::find_if (requests_.cbegin(), requests_.cend(), hasEqualRoot (u));
    
    // if (p != requests_.cend()) { p->get()->close (save); requests_.erase (p); }
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
    std::shared_ptr<Patch> p (fetchPatch (roots_, u));
            
    if (p) {
    //
    removePatch (roots_, u);
    
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
