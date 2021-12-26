
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

void Patch::addObject (const core::Unique& u, const core::Description& v)
{
    DBG (juce::String ("Add: ") + u.debug()); DBG (v.debug());
    
    juce::ValueTree parent (getParent (u));
    juce::ValueTree object (getChildWithIdentifier (parent, u.getIdentifier()));
    
    if (object.isValid()) {
        object.copyPropertiesAndChildrenFrom (v, nullptr);
    } else {
        parent.appendChild (v, nullptr);
    }
}

void Patch::removeObject (const core::Unique& u)
{
    DBG (juce::String ("Remove: ") + u.debug());
    
    juce::ValueTree parent (getParent (u));
    juce::ValueTree object (getChildWithIdentifier (parent, u.getIdentifier()));
    
    if (object.isValid()) {
        parent.removeChild (object, nullptr);
    } else {
        jassertfalse;       /* Is this possible? */
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Patch::getParent (const core::Unique& u) const
{
    juce::ValueTree t (tree_);
    
    if (u.hasPath()) {
        for (const auto& i : u.getPath()) { t = getChildWithIdentifier (t, i); }
    }
    
    return t;
}

juce::ValueTree Patch::getObject (const core::Unique& u) const
{
    return getChildWithIdentifier (getParent (u), u.getIdentifier());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
