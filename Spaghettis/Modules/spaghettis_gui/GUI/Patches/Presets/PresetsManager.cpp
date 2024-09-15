
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

juce::File getPresetFile (const juce::File& file)
{
    return file.withFileExtension ("pdpresets");
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

PresetsManager::PresetsManager (const juce::ValueTree& root, const juce::File& file) :
    rootTree_ (root),
    presetsFile_ (getPresetFile (file), getPresetOptions())
{
}

PresetsManager::~PresetsManager()
{
    if (isValid()) { presetsFile_.setNeedsToBeSaved (false); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool PresetsManager::isValid() const
{
    return presetsFile_.isValidFile();
}

bool PresetsManager::isValidKey (juce::StringRef keyName) const
{
    return isValid() && presetsFile_.containsKey (keyName);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<juce::Rectangle<int>> PresetsManager::getRunWindow() const
{
    if (isValidKey (PresetsConstants::PositionTag)) {
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

std::optional<bool> PresetsManager::getTabState() const
{
    if (isValidKey (PresetsConstants::StateTag)) {
        return presetsFile_.getBoolValue (PresetsConstants::StateTag);
    }
    
    return std::nullopt;
}

std::optional<int> PresetsManager::getTabWidth() const
{
    if (isValidKey (PresetsConstants::WidthTag)) {
        return presetsFile_.getIntValue (PresetsConstants::WidthTag);
    }
    
    return std::nullopt;
}

bool PresetsManager::hasAutoload() const
{
    if (isValidKey (PresetsConstants::AutoloadTag)) {
        return presetsFile_.getBoolValue (PresetsConstants::AutoloadTag);
    }
    
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PresetsManager::setRunWindow (const juce::Rectangle<int>& bounds)
{
    if (isValid()) {
    //
    auto e = std::make_unique<juce::XmlElement> (Id::POSITION);
    
    e->setAttribute (Id::value, bounds.toString());
    
    presetsFile_.setValue (PresetsConstants::PositionTag, e.get());
    //
    }
}

void PresetsManager::setTabState (bool isActive)
{
    if (isValid()) { return presetsFile_.setValue (PresetsConstants::StateTag, isActive); }
}

void PresetsManager::setTabWidth (int w)
{
    if (isValid()) { return presetsFile_.setValue (PresetsConstants::WidthTag, w); }
}

void PresetsManager::setAutoload (bool hasAutoload)
{
    if (isValid()) { return presetsFile_.setValue (PresetsConstants::AutoloadTag, hasAutoload); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void storeSlot (juce::PropertiesFile& file,
    juce::String name,
    const std::vector<PresetElement>* elements)
{
    if (name.isEmpty()) { name = PresetsConstants::DefaultTag; }
    
    auto root = std::make_unique<juce::XmlElement> (Id::PRESETS);
    
    for (const auto& p : *elements) {
    //
    juce::XmlElement* e = root->createNewChildElement (Id::PRESET);
    e->setAttribute (Id::item,  p.getTag().toString());
    e->setAttribute (Id::type,  PresetsConstants::FloatType);
    e->setAttribute (Id::value, p.getParameter().getValueTypedUnchecked<double>());
    //
    }
    
    file.setValue (PresetsConstants::PresetTag + name, root.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool containsElement (core::UniqueId u, const std::vector<PresetElement>* elements)
{
    if (elements == nullptr) { return true; }
    else {
        for (const auto& p : *elements) {
            if (u == data::Cast::fromVar<core::UniqueId> (p.getTag())) {
                return true;
            }
        }
    }
    
    return false;
}

void loadSlot (juce::PropertiesFile& file,
    juce::String name,
    const std::vector<PresetElement>* elements)
{
    if (name.isEmpty()) { name = PresetsConstants::DefaultTag; }    /* Must be the first. */
    
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

bool PresetsManager::load (const std::vector<PresetElement>& elements, const juce::String& name)
{
    if (isValid()) { loadSlot (presetsFile_, name, &elements); return true; }
    
    return false;
}

bool PresetsManager::store (const std::vector<PresetElement>& elements, const juce::String& name)
{
    if (isValid()) { storeSlot (presetsFile_, name, &elements); return true; }
    
    return false;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PresetsManager::loadbangBegin()
{
    resolve();
}

void PresetsManager::loadbangEnd()
{
    if (hasAutoload()) { loadSlot (presetsFile_, juce::String(), nullptr); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PresetsManager::write()
{
    if (isValid() && presetsFile_.needsToBeSaved()) {
    //
    PresetsResolver::convert (presetsFile_, rootTree_);
    presetsFile_.save();
    presetsFile_.setNeedsToBeSaved (false);
    //
    }
}

void PresetsManager::resolve()
{
    if (isValid()) { PresetsResolver::resolve (presetsFile_, rootTree_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
