
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

void setIdentifier (juce::ValueTree& t, core::Unique::Identifier i)
{
    t.setProperty (Ids::identifier, core::Unique::Converter::toVar (i), nullptr);
}

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
    file_ (v.getProperty (Ids::path).toString()),
    tree_ (v.getTree()),
    dirty_ (false)
{
    setIdentifier (tree_, unique_.getRoot());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Patch::addObject (const core::Unique& u, const core::Description& v)
{
    DBG (v.debug());
    
    juce::ValueTree object = v.getTree();
    juce::ValueTree parent = getParent (u);
    
    setIdentifier (object, u.getIdentifier());
    
    jassert (parent.isValid());
    
    parent.appendChild (object, nullptr);
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
