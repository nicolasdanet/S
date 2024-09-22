
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
static Perform setDirty (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static Perform setEditView (core::UniqueId, juce::Rectangle<int>);
static Perform setEditViewProperties (core::UniqueId, core::Point::Real, int, bool, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform undo (core::UniqueId);
static Perform redo (core::UniqueId);
static Perform cut (core::UniqueId);
static Perform copy (core::UniqueId);
static Perform paste (core::UniqueId, core::Point::Real);
static Perform duplicate (core::UniqueId);
static Perform remove (core::UniqueId);
static Perform encapsulate (core::UniqueId);
static Perform deencapsulate (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform selectObject (core::UniqueId);
static Perform deselectObject (core::UniqueId);
static Perform moveBackObject (core::UniqueId);
static Perform moveFrontObject (core::UniqueId);
static Perform snapObject (core::UniqueId);
static Perform positionObject (core::UniqueId, core::Point::Real);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform sendObjectBang (core::UniqueId);
static Perform sendObjectFloat (core::UniqueId, double);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform openHelp (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform createObject (core::UniqueId, core::Point::Real, juce::String);

static Perform parametersObject (core::UniqueId, data::Group);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform disconnectLine (core::UniqueId, int, core::UniqueId, int);
static Perform connectLine (core::UniqueId, int, core::UniqueId, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
