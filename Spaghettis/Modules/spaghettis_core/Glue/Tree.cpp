
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

juce::var Tree::getValue (const juce::String& group, const juce::String& key) const
{
    return getGroup (group).getParameter (key).getValue();
}
    
void Tree::changeValue (const juce::String& group, const juce::String& key, const juce::var& v)
{
    if (getGroup (group).hasParameter (key)) { getGroup (group).getParameter (key).changeValue (v); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Group Tree::addGroup (const juce::String& name)
{
    jassert (!hasGroup (name));
    
    juce::ValueTree group (Ids::GROUP); group.setProperty (Ids::name, name, nullptr);
        
    tree_.appendChild (group, nullptr);
        
    return Group (group);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Tree::hasGroup (const juce::String& group) const
{
    return getGroup (group).isValid();
}

Group Tree::getGroup (const juce::String& name) const
{
    for (const auto& group : *this) { if (group.getName() == name) { return group; } }
    
    return Group();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Tree::setParametersFrom (const juce::ValueTree& other)
{
    const Tree tree (other);
    
    if (tree.isValid (tree_.getType())) {
    //
    for (const auto& group : tree) {
        const juce::String name (group.getName());
        for (const auto& parameter : group) {
            changeValue (name, parameter.getKey(), parameter.getValue());
        }
    }
    //
    }
}

void Tree::write (const juce::File& file) const
{
    std::unique_ptr<juce::XmlElement> xml (tree_.createXml());
    
    if (xml) { xml->writeTo (file); }
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
