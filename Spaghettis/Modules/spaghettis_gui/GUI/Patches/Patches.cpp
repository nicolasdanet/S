
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
    auto r = std::find_if (roots_.cbegin(), roots_.cend(), identifierIsEqual (u));
    
    return std::shared_ptr<Patch> (r != roots_.cend() ? *r : nullptr);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patches::addPatch (const core::Unique& u, const core::Description& v)
{
    roots_.push_back (std::make_shared<Patch> (u, v));
}

void Patches::removePatch (const core::Unique& u)
{
    roots_.erase (std::remove_if (roots_.begin(), roots_.end(), identifierIsEqual (u)), roots_.end());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* To avoid bad loops remove first the patch from the roots before to free it. */

void Patches::closePatch (const core::Unique& u)
{
    std::shared_ptr<Patch> scoped (fetchPatch (u));
    
    if (scoped) { removePatch (u); scoped->close(); }
}

void Patches::closeAllPatches()
{
    std::for_each (roots_.cbegin(), roots_.cend(), [] (const auto& p) { p->close(); });
    
    roots_.clear();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
