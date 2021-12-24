
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

bool hasIdentifier (const juce::ValueTree& t, core::Unique::Identifier i)
{
    return (i == core::Unique::Converter::fromVar (t.getProperty (Ids::identifier)));
}

juce::ValueTree getChildWithIdentifier (const juce::ValueTree& t, core::Unique::Identifier i)
{
    return t.getChildWithProperty (Ids::identifier, core::Unique::Converter::toVar (i));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Patch::Patch (const core::Unique& u, const core::Description& v) :
    unique_ (u),
    file_ (v.getFullPathName()),
    tree_ (v.fetchTree (u.getRoot())),
    dirty_ (false)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::addObject (const core::Unique& u, const core::Description& v)
{
    const core::Unique::Identifier i = u.getIdentifier();
    
    juce::ValueTree parent = getParent (u);
    juce::ValueTree object = getChildWithIdentifier (parent, i);
    
    if (object.isValid()) {
        object.copyPropertiesFrom (v.fetchTree (i), nullptr);
    } else {
        parent.appendChild (v.fetchTree (i), nullptr);
    }
}

void Patch::removeObject (const core::Unique& u)
{

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Patch::getParent (const core::Unique& u) const
{
    juce::ValueTree t (tree_);
        
    std::vector<core::Unique::Identifier> identifiers (u.getPath());
    
    jassert (!identifiers.empty() && hasIdentifier (t, identifiers.front()));
    
    identifiers.erase (identifiers.cbegin());
    
    for (const auto& i : identifiers) { t = getChildWithIdentifier (t, i); }
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
