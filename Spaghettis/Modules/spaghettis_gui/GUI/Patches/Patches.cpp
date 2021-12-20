
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
    auto r = std::find_if (roots_.cbegin(), roots_.cend(), isEqual (u));
    
    return std::shared_ptr<Patch> (r != roots_.cend() ? *r : nullptr);
}

void Patches::removePatch (const core::Unique& u)
{
    roots_.erase (std::remove_if (roots_.begin(), roots_.end(), isEqual (u)), roots_.end());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patches::handleCloseRequest (const core::Unique& u, bool saveBeforeClosing)
{
    jassert (juce::MessageManager::getInstance()->isThisTheMessageThread());
        
    auto p = std::find_if (requests_.cbegin(), requests_.cend(), isEqual (u));
    
    if (p != requests_.cend()) {
    //
    if (saveBeforeClosing) { p->get()->save(); }
    
    p->get()->close();
    //
    }
    
    requests_.erase (std::remove_if (requests_.begin(), requests_.end(), isEqual (u)), requests_.end());
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
    std::shared_ptr<Patch> scoped (fetchPatch (u));
    
    if (scoped) {
        removePatch (u); if (notify) { requests_.push_back (scoped); scoped->requestClose(); }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patches::closeAllPatches()
{
    std::vector<core::Unique> t;
    
    std::transform (roots_.cbegin(), roots_.cend(), std::back_inserter (t), asUnique());
    
    for (const auto& u : t) { closePatch (u); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
