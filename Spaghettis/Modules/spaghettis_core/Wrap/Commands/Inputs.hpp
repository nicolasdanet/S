
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

static inline int loggedAsInteger (Logged type)
{
    return (type == Logged::base) ? 0 : ((type == Logged::full) ? 1 : -1);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform dspSwitch();
static Perform searchPathsRescan (Logged type);
static Perform searchPathsSet (juce::StringArray);
static Perform snapToGridSet (bool);
static Perform snapToGridSetSize (int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform devicesRescanAudio (Logged type);
static Perform devicesRescanMidi (Logged type);
static Perform devicesSetAudio (juce::StringArray, juce::StringArray);
static Perform devicesSetMidi (juce::StringArray, juce::StringArray);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform patchNew (juce::File);
static Perform patchOpen (juce::File);
static Perform patchClose (core::UniqueId);
static Perform patchSave (core::UniqueId);
static Perform patchSetDirty (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static Perform patchSetEditView (core::UniqueId, juce::Rectangle<int>);
static Perform patchSetEditViewProperties (core::UniqueId, core::Point::Real, int, bool, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform patchUndo (core::UniqueId);
static Perform patchRedo (core::UniqueId);
static Perform patchCut (core::UniqueId);
static Perform patchCopy (core::UniqueId);
static Perform patchPaste (core::UniqueId, core::Point::Real);
static Perform patchDuplicate (core::UniqueId);
static Perform patchRemove (core::UniqueId);
static Perform patchEncapsulate (core::UniqueId);
static Perform patchDeencapsulate (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform patchCreateObject (core::UniqueId, core::Point::Real, juce::String);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform objectSelect (core::UniqueId);
static Perform objectDeselect (core::UniqueId);
static Perform objectMoveBack (core::UniqueId);
static Perform objectMoveFront (core::UniqueId);
static Perform objectSnap (core::UniqueId);
static Perform objectPosition (core::UniqueId, core::Point::Real);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform objectSendBang (core::UniqueId);
static Perform objectSendFloat (core::UniqueId, double);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform objectOpenHelp (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform objectSetParameters (core::UniqueId, data::Group);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static Perform lineConnect (core::UniqueId, int, core::UniqueId, int);
static Perform lineDisconnect (core::UniqueId, int, core::UniqueId, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
