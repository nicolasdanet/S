
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Inputs {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum class Logged {
    none,
    base,
    full
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform ping();
static Perform newPatch (juce::File);
static Perform openPatch (juce::File);
static Perform rescanSearchPaths (Logged type);
static Perform switchDsp();
static Perform setSearchPaths (juce::StringArray);
static Perform setSnapToGrid (bool);
static Perform setSnapToGridSize (int);
static Perform rescanDevices (void);
static Perform setAudioDevices (std::vector<AudioDevice>&&, std::vector<AudioDevice>&&);
static Perform setMidiDevices (std::vector<MidiDevice>&&, std::vector<MidiDevice>&&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform closePatch (core::UniqueId);
static Perform savePatch (core::UniqueId);
static Perform setEditView (core::UniqueId, juce::Rectangle<int>);
static Perform setRunView (core::UniqueId, juce::Rectangle<int>);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform selectObject (core::UniqueId);
static Perform deselectObject (core::UniqueId);
static Perform moveBackObject (core::UniqueId);
static Perform moveFrontObject (core::UniqueId);
static Perform snapObject (core::UniqueId);
static Perform positionObject (core::UniqueId, juce::Point<int>);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
