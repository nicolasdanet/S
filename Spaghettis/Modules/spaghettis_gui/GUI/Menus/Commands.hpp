
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define SPAGHETTIS_COMMAND(c)   (c | 0x7f000000)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

struct Commands {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

enum CommandIds : juce::CommandID {
    preferences     = SPAGHETTIS_COMMAND (0),
    newPatch,
    openPatch,
    clearRecentFiles,
    save,
    closeWindow,
    paths,
    rescan,
    rescanLogged,
    selectAll,
    undo,
    redo,
    cut,
    copy,
    paste,
    duplicate,
    remove,
    addObject,
    encapsulate,
    deencapsulate,
    moveBack,
    moveFront,
    snap,
    zoomIn,
    zoomOut,
    zoomReset,
    inspector,
    newRunView,
    newEditView,
    clearConsole,
    devices,
    dspSwitch
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

enum BaseIds : int {
    recentFiles     = 100
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------