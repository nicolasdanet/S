
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace PresetsConstants
{
    constexpr static const char* const PositionTag = "RunWindowPosition";
    constexpr static const char* const StateTag    = "PresetsTabState";
    constexpr static const char* const WidthTag    = "PresetsTabWidth";
    constexpr static const char* const AutoloadTag = "AutoloadState";
    constexpr static const char* const DefaultTag  = "Default";
    constexpr static const char* const PresetTag   = "#";
    constexpr static const char* const FloatType   = "float";
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PresetsManager {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    PresetsManager (const juce::ValueTree&, const juce::File&);
    
    ~PresetsManager();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    bool isValid() const;
    bool isValidKey (juce::StringRef) const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::optional<juce::Rectangle<int>> getRunWindow() const;
    std::optional<bool> getTabState() const;
    std::optional<int> getTabWidth() const;
    bool hasAutoload() const;

public:
    void setRunWindow (const juce::Rectangle<int>&);
    void setTabState (bool);
    void setTabWidth (int);
    void setAutoload (bool);
    
public:
    bool load (const std::vector<PresetElement>&, const juce::String&);
    bool store (const std::vector<PresetElement>&, const juce::String&);

public:
    void loadbangBegin();
    void loadbangEnd();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void write();

private:
    void resolve();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    juce::ValueTree rootTree_;

private:
    juce::PropertiesFile presetsFile_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetsManager)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

