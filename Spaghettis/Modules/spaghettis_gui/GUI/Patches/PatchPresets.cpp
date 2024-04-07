
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::File getPresetFile (const juce::File& file)
{
    return file.withFileExtension ("xml");
}

juce::PropertiesFile::Options getPresetOptions()
{
    juce::PropertiesFile::Options options;
    
    options.millisecondsBeforeSaving = -1;
    
    return options;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PatchPresets::PatchPresets (const juce::ValueTree& root, const juce::File& file) :
    rootTree_ (root),
    presetsFile_ (getPresetFile (file), getPresetOptions())
{
    resolve();
}

PatchPresets::~PatchPresets()
{
    if (isValid()) { presetsFile_.setNeedsToBeSaved (false); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool PatchPresets::isValid() const
{
    return presetsFile_.isValidFile();
}

bool PatchPresets::isValid (juce::StringRef keyName) const
{
    return isValid() && presetsFile_.containsKey (keyName);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace PresetsConstants
{
    constexpr static const char* const PositionTag = "RunWindowPosition";
    constexpr static const char* const StateTag    = "PresetsTabState";
    constexpr static const char* const WidthTag    = "PresetsTabWidth";
    constexpr static const char* const PresetTag   = "#";
    constexpr static const char* const FloatType   = "float";
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<juce::Rectangle<int>> PatchPresets::getRunWindow() const
{
    if (isValid (PresetsConstants::PositionTag)) {
    //
    const std::unique_ptr<juce::XmlElement> e (presetsFile_.getXmlValue (PresetsConstants::PositionTag));
    
    if (e && e->hasTagName (Id::POSITION) && e->hasAttribute (Id::value)) {
        const juce::String s = e->getStringAttribute (Id::value);
        if (s.isNotEmpty()) {
            return juce::Rectangle<int>::fromString (s);
        }
    }
    //
    }
    
    return std::nullopt;
}

std::optional<bool> PatchPresets::getTabState() const
{
    if (isValid (PresetsConstants::StateTag)) {
        return presetsFile_.getBoolValue (PresetsConstants::StateTag);
    }
    
    return std::nullopt;
}

std::optional<int> PatchPresets::getTabWidth() const
{
    if (isValid (PresetsConstants::WidthTag)) {
        return presetsFile_.getIntValue (PresetsConstants::WidthTag);
    }
    
    return std::nullopt;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchPresets::setRunWindow (const juce::Rectangle<int>& bounds)
{
    if (isValid()) {
    //
    auto e = std::make_unique<juce::XmlElement> (Id::POSITION);
    
    e->setAttribute (Id::value, bounds.toString());
    
    presetsFile_.setValue (PresetsConstants::PositionTag, e.get());
    //
    }
}

void PatchPresets::setTabState (bool isActive)
{
    if (isValid()) { return presetsFile_.setValue (PresetsConstants::StateTag, isActive); }
}

void PatchPresets::setTabWidth (int w)
{
    if (isValid()) { return presetsFile_.setValue (PresetsConstants::WidthTag, w); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

bool containsElement (core::UniqueId u, const std::vector<PresetElement>& elements)
{
    for (const auto& p : elements) {
    //
    if (u == data::Cast::fromVar<core::UniqueId> (p.getTag())) { return true; }
    //
    }
    
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void storeSlot (juce::PropertiesFile& file,
    const juce::String& name,
    const std::vector<PresetElement>& elements)
{
    auto root = std::make_unique<juce::XmlElement> (Id::PRESETS);
    
    for (const auto& p : elements) {
    //
    juce::XmlElement* e = root->createNewChildElement (Id::PRESET);
    e->setAttribute (Id::item,  p.getTag().toString());
    e->setAttribute (Id::type,  PresetsConstants::FloatType);
    e->setAttribute (Id::value, p.getParameter().getValueTypedUnchecked<double>());
    //
    }
    
    file.setValue (PresetsConstants::PresetTag + name, root.get());
}

void loadSlot (juce::PropertiesFile& file,
    const juce::String& name,
    const std::vector<PresetElement>& elements)
{
    const std::unique_ptr<juce::XmlElement> root (file.getXmlValue (PresetsConstants::PresetTag + name));
        
    if (root && root->hasTagName (Id::PRESETS)) {
    //
    for (auto* e : root->getChildWithTagNameIterator (Id::PRESET)) {
        const core::UniqueId u = data::Cast::fromVar<core::UniqueId> (e->getStringAttribute (Id::item));
        if (containsElement (u, elements)) {
            Broadcast::sendFloat (u, e->getDoubleAttribute (Id::value));
        }
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

bool PatchPresets::load (const juce::String& name, const std::vector<PresetElement>& elements)
{
    if (isValid()) { loadSlot (presetsFile_, name, elements); return true; }
    
    return false;
}

bool PatchPresets::store (const juce::String& name, const std::vector<PresetElement>& elements)
{
    if (isValid()) { storeSlot (presetsFile_, name, elements); return true; }
    
    return false;
}

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
    for (auto* e : root->getChildWithTagNameIterator (Id::PRESET)) {
        e->setAttribute (Id::path, paths.getPathWithItem (e->getStringAttribute (Id::item)));
    }
    //
    }
    
    file.setValue (PresetsConstants::PresetTag + name, root.get());
}

void resolveSlot (juce::PropertiesFile& file, const juce::String& name, const AbsoluteToLocal& paths)
{
    const std::unique_ptr<juce::XmlElement> root (file.getXmlValue (PresetsConstants::PresetTag + name));
        
    if (root && root->hasTagName (Id::PRESETS)) {
    //
    for (auto* e : root->getChildWithTagNameIterator (Id::PRESET)) {
        e->setAttribute (Id::item, paths.getItemWithPath (e->getStringAttribute (Id::path)));
    }
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

void PatchPresets::write()
{
    if (isValid() && presetsFile_.needsToBeSaved()) {
        convertToAbsolute (presetsFile_, LocalToAbsolute (rootTree_));
        presetsFile_.save();
    }
}

void PatchPresets::resolve()
{
    if (isValid()) { resolveToLocal (presetsFile_, AbsoluteToLocal (rootTree_)); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
