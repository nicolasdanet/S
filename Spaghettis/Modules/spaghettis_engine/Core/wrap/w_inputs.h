
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef W_INPUTS_H_
#define W_INPUTS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_ping               (void);
void inputs_newPatch           (const juce::File&);
void inputs_openPatch          (const juce::File&);
void inputs_switchDsp          (void);
void inputs_rescanSearchPaths  (int logged);
void inputs_setSearchPaths     (const juce::StringArray&);
void inputs_setSnap            (bool);
void inputs_setSnapSize        (int);
void inputs_rescanDevices      (void);
void inputs_setAudioDevices    (const std::vector<AudioDevice>&, const std::vector<AudioDevice>&);
void inputs_setMidiDevices     (const std::vector<MidiDevice>&, const std::vector<MidiDevice>&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_closePatch              (core::UniqueId);
void inputs_savePatch               (core::UniqueId);
void inputs_setDirty                (core::UniqueId);
void inputs_setEditView             (core::UniqueId, juce::Rectangle<int>);
void inputs_setEditViewProperties   (core::UniqueId, core::Point::Real, int, bool, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_undo               (core::UniqueId);
void inputs_redo               (core::UniqueId);
void inputs_cut                (core::UniqueId);
void inputs_copy               (core::UniqueId);
void inputs_paste              (core::UniqueId, Point::Real pt);
void inputs_duplicate          (core::UniqueId);
void inputs_remove             (core::UniqueId);
void inputs_encapsulate        (core::UniqueId);
void inputs_deencapsulate      (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_selectObject       (core::UniqueId);
void inputs_deselectObject     (core::UniqueId);
void inputs_moveBackObject     (core::UniqueId);
void inputs_moveFrontObject    (core::UniqueId);
void inputs_snapObject         (core::UniqueId);
void inputs_positionObject     (core::UniqueId, Point::Real pt);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_includeObject       (core::UniqueId);
void inputs_excludeObject       (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_sendObjectBang     (core::UniqueId);
void inputs_sendObjectFloat    (core::UniqueId, double);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_openHelp           (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_createObject       (core::UniqueId, Point::Real pt, juce::String);

void inputs_parametersObject   (core::UniqueId, const data::Group&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_disconnectLine     (core::UniqueId u, int m, core::UniqueId v, int n);
void inputs_connectLine        (core::UniqueId u, int m, core::UniqueId v, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_INPUTS_H_
