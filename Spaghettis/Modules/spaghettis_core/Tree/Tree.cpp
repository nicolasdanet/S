
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

void substituteDelegates (juce::ValueTree& tree)
{
    if (tree.hasType (Ids::PARAMETER) && tree.hasProperty (Ids::DELEGATE)) {
    //
    auto p = dynamic_cast<Delegate::Shared*> (tree.getProperty (Ids::DELEGATE).getObject());
    
    if (p) { tree.setProperty (Ids::key, p->getTree().getProperty (Ids::key), nullptr); }
    
    tree.removeProperty (Ids::DELEGATE, nullptr);
    //
    }
    
    for (auto child : tree) { substituteDelegates (child); }
}

bool readValuesFrom (const juce::ValueTree& tree)
{
    if (tree.hasType (Ids::GROUP)) {
    //
    const juce::String name (tree.getProperty (Ids::name).toString());
    
    DBG (name);
    
    return true;
    //
    }
    
    return false;
}

void readFrom (Tree& tree, const juce::ValueTree& other)
{
    if (!readValuesFrom (other)) { for (auto child : other) { readValuesFrom (child); } }
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
    
    substituteDelegates (t);
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Tree::read (const juce::File& file)
{
    if (file.existsAsFile() && file.hasFileExtension (".xml")) {
        std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file));
        if (xml) {
            juce::ValueTree t (juce::ValueTree::fromXml (*xml));
            readFrom (*this, t);
            return true;
        }
    }
    
    return false;
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
