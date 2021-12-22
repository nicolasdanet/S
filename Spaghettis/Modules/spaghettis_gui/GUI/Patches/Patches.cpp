
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::shared_ptr<Patch> Patches::fetchPatch (const core::Unique& u) const
{
    auto r = std::find_if (roots_.cbegin(), roots_.cend(), hasEqualRoot (u));
    
    return std::shared_ptr<Patch> (r != roots_.cend() ? *r : nullptr);
}

void Patches::removePatch (const core::Unique& u)
{
    roots_.erase (std::remove_if (roots_.begin(), roots_.end(), hasEqualRoot (u)), roots_.end());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patches::showSaveRequest (const std::shared_ptr<Patch>& p)
{
    const juce::MessageBoxOptions options (juce::MessageBoxOptions().withTitle (p->getFile().getFileName())
            .withMessage (NEEDS_TRANS ("Save the patch before closing?"))
            .withButton (NEEDS_TRANS ("Yes"))
            .withButton (NEEDS_TRANS ("No")));
    
    auto f = [u = p->getUnique()] (int result)
    {
        const bool save = (result == 0); Spaghettis()->getPatches().handleSaveRequest (u, save);
    };
    
    juce::NativeMessageBox::showAsync (options, f);
    
    requests_.push_back (p);
}

void Patches::handleSaveRequest (const core::Unique& u, bool save)
{
    jassert (juce::MessageManager::getInstance()->isThisTheMessageThread());
        
    auto p = std::find_if (requests_.cbegin(), requests_.cend(), hasEqualRoot (u));
    
    if (p != requests_.cend()) { p->get()->close (save); requests_.erase (p); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patches::createPatch (const core::Unique& u, const core::Description& v)
{
    roots_.push_back (std::make_shared<Patch> (u, v));
}

/* To avoid bad loops remove first the patch from the roots before to release it. */

void Patches::closePatch (const core::Unique& u, bool notify)
{
    std::shared_ptr<Patch> p (fetchPatch (u));
            
    if (p) {
    //
    removePatch (u);
    
    if (notify) {
        if (p->isDirty()) { showSaveRequest (p); } else { p->close(); }
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patches::closeAllPatches()
{
    std::vector<core::Unique> t;
    
    std::transform (roots_.cbegin(), roots_.cend(), std::back_inserter (t), toUnique());
    
    for (const auto& u : t) { closePatch (u); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
