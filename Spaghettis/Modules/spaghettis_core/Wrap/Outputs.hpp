
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct Outputs {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform quit                         ();
static Perform clearConsole                 ();
static Perform reportDsp                    (bool);
static Perform patchOpened                  (juce::File);
static Perform reportAvailableAudioDevices  (std::vector<AudioDevice>&&, std::vector<AudioDevice>&&);
static Perform reportCurrentAudioDevices    (std::vector<AudioDevice>&&, std::vector<AudioDevice>&&);
static Perform reportAvailableMidiDevices   (std::vector<MidiDevice>&&, std::vector<MidiDevice>&&);
static Perform reportCurrentMidiDevices     (std::vector<MidiDevice>&&, std::vector<MidiDevice>&&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform objectAdded                  (core::Unique, core::Description);
static Perform objectChanged                (core::Unique, core::Description);
static Perform objectRemoved                (core::Unique);
static Perform objectRenamed                (core::Unique, core::Unique::Identifier);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
