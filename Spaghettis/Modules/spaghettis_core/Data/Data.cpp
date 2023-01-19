
/* Copyright (c) 2022 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

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

void Data::changeValue (const juce::String& group, const juce::String& key, const juce::var& v)
{
    if (hasParameter (group, key)) { getParameter (group, key).changeValue (v); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Group Data::addGroup (const juce::String& name, bool isHidden)
{
    jassert (!hasGroup (name));
    
    juce::ValueTree group (Id::GROUP);
    
    group.setProperty (Id::name, name, nullptr);
    group.setProperty (Id::hidden, isHidden, nullptr);
    
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
// MARK: -

int Data::getGroupIndex (const juce::String& name) const
{
    int i = 0;
    
    for (const auto& group : *this) { if (group.getName() == name) { return i; } ++i; }
    
    return -1;
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
    if (tree.hasType (Id::PARAMETER) && tree.hasProperty (Id::DELEGATE)) {
    //
    auto p = dynamic_cast<DelegateShared*> (tree.getProperty (Id::DELEGATE).getObject());
    
    if (p) {
        tree.setProperty (Id::key, p->getValueTree().getProperty (Id::key), nullptr);
    }
    
    tree.removeProperty (Id::DELEGATE, nullptr);
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

juce::String Data::debug (const Data& data)
{
    return debug (data.asValueTree());
}

juce::String Data::debug (const juce::ValueTree& tree)
{
    return getCopyPruned (tree).toXmlString();
}

juce::String Data::debug (const Item& item)
{
    return debug (item.asValueTree());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void changeValuesFrom (Data& data, const juce::ValueTree& other)
{
    if (other.hasType (Id::PARAMETER)) {
    //
    const juce::String group (other.getParent().getProperty (Id::name).toString());
    const juce::String key (other.getProperty (Id::key).toString());
    const juce::var v (other.getProperty (Id::value));
    
    data.changeValue (group, key, v);
    //
    }
    
    for (auto child : other) { changeValuesFrom (data, child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Note that for now only XML from documentation is handled. */

void addParameterFrom (Group group, const juce::ValueTree& other)
{
    const juce::String key (other.getProperty (Id::key).toString());
    const juce::String value (other.getProperty (Id::value).toString());
    
    if (key.isNotEmpty() && value.isNotEmpty() && !group.hasParameter (key)) {
    //
    juce::String label (other.getProperty (Id::label).toString());
    juce::String info (other.getProperty (Id::info).toString());
    
    if (label.isEmpty()) { label = key;   }
    if (info.isEmpty())  { info  = label; }
    
    group.addParameter (key, label, info, value).setEditable (false);
    //
    }
}

void addFrom (Data& data, const juce::ValueTree& other)
{
    if (other.hasType (Id::PARAMETER)) {
    //
    const juce::String group (other.getParent().getProperty (Id::name).toString());
    
    if (group.isNotEmpty()) {
    //
    if (!data.hasGroup (group)) { data.addGroup (group); }
    
    addParameterFrom (data.getGroup (group), other);
    //
    }
    //
    }
    
    for (auto child : other) { addFrom (data, child); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Data::apply (const Item& item)
{
    changeValuesFrom (*this, getCopyPruned (item.asValueTree()));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Data::addParametersFromXmlDocumentation (const juce::String& s)
{
    std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (s));
    if (xml) {
        addFrom (*this, juce::ValueTree::fromXml (*xml));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Data::readValuesFromFile (const juce::File& file)
{
    if (file.existsAsFile() && file.hasFileExtension (".xml")) {
        std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file));
        if (xml) {
            changeValuesFrom (*this, juce::ValueTree::fromXml (*xml));
            return true;
        }
    }
    
    return false;
}

void Data::writeValuesToFile (const juce::File& file) const
{
    std::unique_ptr<juce::XmlElement> xml (getCopyPruned (tree_).createXml());
    
    if (xml) { xml->writeTo (file); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Data Data::makeCopy (const Data& data)
{
    return Data (data.tree_.createCopy());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
