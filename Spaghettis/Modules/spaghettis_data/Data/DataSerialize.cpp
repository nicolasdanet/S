
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::data {

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
    auto p = dynamic_cast<Delegate*> (tree.getProperty (Id::DELEGATE).getObject());
    
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
// MARK: -

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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Data::changeValues (const Data& data)
{
    changeValuesFrom (*this, getCopyPruned (data.tree_));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool Data::readValues (const juce::File& file)
{
    if (file.existsAsFile()) {
        std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file));
        if (xml) {
            changeValuesFrom (*this, juce::ValueTree::fromXml (*xml));
            return true;
        }
    }
    
    return false;
}

void Data::writeValues (const juce::File& file) const
{
    std::unique_ptr<juce::XmlElement> xml (getCopyPruned (tree_).createXml());
    
    if (xml) { xml->writeTo (file); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Data::setValuesFromDocumentation (const juce::String& s)
{

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
// MARK: -

juce::String Data::toDebugString (const juce::ValueTree& tree)
{
    return getCopyPruned (tree).toXmlString();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
