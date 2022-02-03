
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Group Tree::addGroup (const juce::String& name)
{
    juce::ValueTree group (Ids::GROUP); group.setProperty (Ids::name, name, nullptr);
        
    tree_.appendChild (group, nullptr);
        
    return Group (group);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/*
void setPropertyFrom (juce::ValueTree& tree, const juce::ValueTree& group, const juce::ValueTree& parameter)
{
    juce::ValueTree t = tree.getChildWithProperty (Ids::name, group.getProperty (Ids::name))
                            .getChildWithProperty (Ids::key, parameter.getProperty (Ids::key));
    
    if (t.isValid()) {
    //
    const juce::var& newValue (parameter.getProperty (Ids::value));
    const juce::var& oldValue (t.getProperty (Ids::value));
    
    if (!oldValue.equals (newValue)) {
        t.setProperty (Ids::value, Parameters::Base (t).constrained (newValue), nullptr);
    }
    //
    }
}
*/

void Tree::setPropertiesFrom (const juce::ValueTree& other)
{
    const Tree t (other);
    
    if (t.isValid (tree_.getType())) {
    //
    /*
    for (const auto& group : t) {
    for (const auto& parameter : group) {
        // setPropertyFrom (tree, group, parameter);
    }
    }
    */
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Tree::isValid (const juce::Identifier& identifier) const
{
    if (!tree_.isValid() || !tree_.hasType (identifier)) { return false; }
    
    for (const auto& group : tree_) {
        if (!Group (group).isValid()) { return false; }
    }

    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
