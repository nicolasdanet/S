
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

PatchPresets::PatchPresets (const juce::File& file) : presets_ (getPresetFile (file), getPresetOptions())
{
    if (isValid() == false) { }
}

PatchPresets::~PatchPresets()
{
    if (isValid()) { presets_.setNeedsToBeSaved (false); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool PatchPresets::isValid() const
{
    return presets_.isValidFile();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace PresetsConstants
{
    constexpr static const char* const PositionTag = "RunWindowPosition";
    constexpr static const char* const StateTag    = "TabState";
    constexpr static const char* const WidthTag    = "TabWidth";
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::optional<juce::Rectangle<int>> PatchPresets::getRunWindow() const
{
    if (isValid()) {
    //
    const std::unique_ptr<juce::XmlElement> e (presets_.getXmlValue (PresetsConstants::PositionTag));
    
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
    if (isValid() && presets_.containsKey (PresetsConstants::StateTag)) {
    //
    return presets_.getBoolValue (PresetsConstants::StateTag);
    //
    }
    
    return std::nullopt;
}

std::optional<int> PatchPresets::getTabWidth() const
{
    if (isValid() && presets_.containsKey (PresetsConstants::WidthTag)) {
    //
    return presets_.getIntValue (PresetsConstants::WidthTag);
    //
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
    
    presets_.setValue (PresetsConstants::PositionTag, e.get());
    //
    }
}

void PatchPresets::setTabState (bool)
{

}

void PatchPresets::setTabWidth (int)
{

}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void PatchPresets::save()
{
    if (isValid()) { presets_.saveIfNeeded(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
