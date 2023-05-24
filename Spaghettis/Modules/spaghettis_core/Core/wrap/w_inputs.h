
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

PD_LOCAL void inputs_ping               (void);
PD_LOCAL void inputs_newPatch           (const juce::File&);
PD_LOCAL void inputs_openPatch          (const juce::File&);
PD_LOCAL void inputs_switchDsp          (void);
PD_LOCAL void inputs_rescanSearchPaths  (int logged);
PD_LOCAL void inputs_setSearchPaths     (const juce::StringArray&);
PD_LOCAL void inputs_setSnap            (bool);
PD_LOCAL void inputs_setSnapSize        (int);
PD_LOCAL void inputs_rescanDevices      (void);
PD_LOCAL void inputs_setAudioDevices    (const std::vector<AudioDevice>&, const std::vector<AudioDevice>&);
PD_LOCAL void inputs_setMidiDevices     (const std::vector<MidiDevice>&, const std::vector<MidiDevice>&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void inputs_closePatch         (core::UniqueId);
PD_LOCAL void inputs_savePatch          (core::UniqueId);
PD_LOCAL void inputs_setEditView        (core::UniqueId, juce::Rectangle<int>);
PD_LOCAL void inputs_setRunView         (core::UniqueId, juce::Rectangle<int>);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void inputs_undo               (core::UniqueId);
PD_LOCAL void inputs_redo               (core::UniqueId);
PD_LOCAL void inputs_cut                (core::UniqueId);
PD_LOCAL void inputs_copy               (core::UniqueId);
PD_LOCAL void inputs_paste              (core::UniqueId, Point::Real pt);
PD_LOCAL void inputs_duplicate          (core::UniqueId);
PD_LOCAL void inputs_remove             (core::UniqueId);
PD_LOCAL void inputs_encapsulate        (core::UniqueId);
PD_LOCAL void inputs_deencapsulate      (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void inputs_selectObject       (core::UniqueId);
PD_LOCAL void inputs_deselectObject     (core::UniqueId);
PD_LOCAL void inputs_moveBackObject     (core::UniqueId);
PD_LOCAL void inputs_moveFrontObject    (core::UniqueId);
PD_LOCAL void inputs_snapObject         (core::UniqueId);
PD_LOCAL void inputs_positionObject     (core::UniqueId, Point::Real pt);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void inputs_sendObjectBang     (core::UniqueId);
PD_LOCAL void inputs_sendObjectFloat    (core::UniqueId, double);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void inputs_openHelp           (core::UniqueId);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void inputs_createObject       (core::UniqueId, Point::Real pt, juce::String);

PD_LOCAL void inputs_parametersObject   (core::UniqueId, const core::Group&);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void inputs_disconnectLine     (core::UniqueId u, int m, core::UniqueId v, int n);
PD_LOCAL void inputs_connectLine        (core::UniqueId u, int m, core::UniqueId v, int n);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_INPUTS_H_
