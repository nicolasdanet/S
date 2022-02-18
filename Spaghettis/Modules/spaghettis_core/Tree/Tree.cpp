
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

bool Tree::hasValue (const juce::String& group, const juce::String& key) const
{
    if (getGroup (group).hasParameter (key)) { return true; }
    
    return false;
}

juce::var Tree::getValue (const juce::String& group, const juce::String& key) const
{
    jassert (hasValue (group, key)); return getGroup (group).getParameter (key).getValue();
}

void Tree::changeValue (const juce::String& group, const juce::String& key, const juce::var& v)
{
    jassert (hasValue (group, key)); getGroup (group).getParameter (key).changeValue (v);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Group Tree::addGroup (const juce::String& name, bool isHidden)
{
    jassert (!hasGroup (name));
    
    juce::ValueTree group (Ids::GROUP);
    
    group.setProperty (Ids::name, name, nullptr);
    group.setProperty (Ids::hidden, isHidden, nullptr);
    
    tree_.appendChild (group, nullptr);
        
    return Group (group);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Tree::hasGroup (const juce::String& name) const
{
    for (const auto& group : *this) { if (group.getName() == name) { return true; } }
    
    return false;
}

Group Tree::getGroup (const juce::String& name) const
{
    for (const auto& group : *this) { if (group.getName() == name) { return group; } }
    
    return Group();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that for serialization only the key/value pairs are kept. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void substitute (juce::ValueTree& tree)
{
    if (tree.hasType (Ids::PARAMETER) && tree.hasProperty (Ids::DELEGATE)) {
    //
    auto p = dynamic_cast<Delegate::Shared*> (tree.getProperty (Ids::DELEGATE).getObject());
    
    if (p) { tree.setProperty (Ids::key, p->getTree().getProperty (Ids::key), nullptr); }
    
    tree.removeProperty (Ids::DELEGATE, nullptr);
    //
    }
    
    for (auto child : tree) { substitute (child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ValueTree Tree::getCopyWithSubstitutedDelegates (const juce::ValueTree& tree)
{
    juce::ValueTree t (tree.createCopy());
    
    substitute (t);
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Tree::setValuesFrom (const juce::ValueTree& tree)
{
    // for (auto child : tree) { setValuesFrom (child); }
}

void Tree::write (const juce::File& file) const
{
    std::unique_ptr<juce::XmlElement> xml (getCopyWithSubstitutedDelegates (tree_).createXml());
    
    if (xml) { xml->writeTo (file); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace core

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
