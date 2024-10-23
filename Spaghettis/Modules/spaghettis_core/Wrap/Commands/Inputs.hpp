
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
static Perform devicesRescan (Logged type);

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
