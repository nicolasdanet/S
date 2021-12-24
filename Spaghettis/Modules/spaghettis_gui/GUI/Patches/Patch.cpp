
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Patch::getParentFor (const core::Unique& u) const
{
    juce::ValueTree t (tree_);
        
    std::vector<core::Unique::Identifier> identifiers (u.getPath());
    
    jassert (!identifiers.empty() && hasIdentifier (t, identifiers.front()));
    
    identifiers.erase (identifiers.cbegin());
    
    for (const auto& i : identifiers) { t = Patch::getChildWithIdentifier (t, i); }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::addObject (const core::Unique& u, const core::Description& v)
{
    DBG (u.debug()); DBG (v.debug());
    
    juce::ValueTree object = v.getTree();
    juce::ValueTree parent = getParentFor (u);
    
    setIdentifier (object, u.getIdentifier());
    
    jassert (parent.isValid());
    
    parent.appendChild (object, nullptr);
}

void Patch::removeObject (const core::Unique& u)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
