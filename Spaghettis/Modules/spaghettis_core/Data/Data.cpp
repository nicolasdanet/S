
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

bool Data::hasParameter (const juce::String& group, const juce::String& key) const
{
    if (getGroup (group).hasParameter (key)) { return true; }
    
    return false;
}

Parameter Data::getParameter (const juce::String& group, const juce::String& key) const
{
    jassert (hasParameter (group, key)); return getGroup (group).getParameter (key);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Group Data::addGroup (const juce::String& name, bool isHidden)
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

bool Data::hasGroup (const juce::String& name) const
{
    for (const auto& group : *this) { if (group.getName() == name) { return true; } }
    
    return false;
}

Group Data::getGroup (const juce::String& name) const
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
    auto p = dynamic_cast<DelegateShared*> (tree.getProperty (Ids::DELEGATE).getObject());
    
    if (p) {
        tree.setProperty (Ids::key, p->getValueTree().getProperty (Ids::key), nullptr);
    }
    
    tree.removeProperty (Ids::DELEGATE, nullptr);
    //
    }
    
    for (auto child : tree) { substituteDelegates (child); }
}

juce::ValueTree getCopyPruned (const juce::ValueTree& tree)
{
    juce::ValueTree t (tree.createCopy());
    
    substituteDelegates (t);
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String Data::debug (const juce::ValueTree& tree)
{
    return getCopyPruned (tree).toXmlString();
}

juce::String Data::debug (const Report& v)
{
    return debug (v.asValueTree());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void readFrom (Data& data, const juce::ValueTree& other)
{
    if (other.hasType (Ids::PARAMETER)) {
    //
    const juce::String group (other.getParent().getProperty (Ids::name).toString());
    const juce::String key (other.getProperty (Ids::key).toString());
    const juce::var v (other.getProperty (Ids::value));
    
    if (data.hasParameter (group, key)) { data.getParameter (group, key).changeValue (v); }
    //
    }
    
    for (auto child : other) { readFrom (data, child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Data::copyFrom (const Report& t)
{
    readFrom (*this, getCopyPruned (t.asValueTree()));
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Data::read (const juce::File& file)
{
    if (file.existsAsFile() && file.hasFileExtension (".xml")) {
        std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file));
        if (xml) {
            readFrom (*this, juce::ValueTree::fromXml (*xml));
            return true;
        }
    }
    
    return false;
}

void Data::write (const juce::File& file) const
{
    std::unique_ptr<juce::XmlElement> xml (getCopyPruned (tree_).createXml());
    
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
