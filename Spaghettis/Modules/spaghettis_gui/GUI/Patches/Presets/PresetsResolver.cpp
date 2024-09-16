
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void convertSlot (juce::PropertiesFile& file, const juce::String& name, const LocalToAbsolute& paths)
{
    const std::unique_ptr<juce::XmlElement> root (file.getXmlValue (PresetsConstants::PresetTag + name));
        
    if (root && root->hasTagName (Id::PRESETS)) {
    //
    std::vector<juce::XmlElement*> pruned;
    
    for (auto* e : root->getChildWithTagNameIterator (Id::PRESET)) {
        const juce::String item (e->getStringAttribute (Id::item));
        const juce::String path (paths.getPathWithItem (item));
        if (path.isEmpty()) { pruned.push_back (e); }
        else {
            e->setAttribute (Id::path, path);
        }
    }
    
    for (auto p : pruned) { root->removeChildElement (p, true); }
    //
    }
    
    file.setValue (PresetsConstants::PresetTag + name, root.get());
}

void resolveSlot (juce::PropertiesFile& file, const juce::String& name, const AbsoluteToLocal& paths)
{
    const std::unique_ptr<juce::XmlElement> root (file.getXmlValue (PresetsConstants::PresetTag + name));
        
    if (root && root->hasTagName (Id::PRESETS)) {
    //
    std::vector<juce::XmlElement*> pruned;
    
    for (auto* e : root->getChildWithTagNameIterator (Id::PRESET)) {
        const juce::String path (e->getStringAttribute (Id::path));
        const juce::String item (paths.getItemWithPath (path));
        if (item.isEmpty()) { pruned.push_back (e); }
        else {
            e->setAttribute (Id::item, item);
        }
    }
    
    for (auto p : pruned) { root->removeChildElement (p, true); }
    //
    }
    
    file.setValue (PresetsConstants::PresetTag + name, root.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void convertToAbsolute (juce::PropertiesFile& file, const LocalToAbsolute& paths)
{
    juce::StringArray keys (file.getAllProperties().getAllKeys());
    
    for (const auto& k : keys) {
    //
    if (k.startsWith (PresetsConstants::PresetTag)) {
        convertSlot (file, k.trimCharactersAtStart (PresetsConstants::PresetTag), paths);
    }
    //
    }
}

void resolveToLocal (juce::PropertiesFile& file, const AbsoluteToLocal& paths)
{
    juce::StringArray keys (file.getAllProperties().getAllKeys());
    
    for (const auto& k : keys) {
    //
    if (k.startsWith (PresetsConstants::PresetTag)) {
        resolveSlot (file, k.trimCharactersAtStart (PresetsConstants::PresetTag), paths);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PresetsResolver::resolve (juce::PropertiesFile& file, const juce::ValueTree& root)
{
    resolveToLocal (file, AbsoluteToLocal (root));
}

void PresetsResolver::convert (juce::PropertiesFile& file, const juce::ValueTree& root)
{
    convertToAbsolute (file, LocalToAbsolute (root));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PresetsResolver::write (juce::PropertiesFile& file, const juce::ValueTree& root)
{
    if (file.needsToBeSaved()) {
    //
    convert (file, root);
    file.save();
    resolve (file, root);
    file.setNeedsToBeSaved (false);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
