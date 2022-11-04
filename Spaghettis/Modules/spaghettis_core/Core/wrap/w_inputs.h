
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef W_INPUTS_H_
#define W_INPUTS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

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

PD_LOCAL void inputs_closePatch         (core::UniqueId);
PD_LOCAL void inputs_savePatch          (core::UniqueId);
PD_LOCAL void inputs_setEditView        (core::UniqueId, juce::Rectangle<int>);
PD_LOCAL void inputs_setRunView         (core::UniqueId, juce::Rectangle<int>);
PD_LOCAL void inputs_undoPatch          (core::UniqueId i);
PD_LOCAL void inputs_redoPatch          (core::UniqueId i);

PD_LOCAL void inputs_selectObject       (core::UniqueId);
PD_LOCAL void inputs_deselectObject     (core::UniqueId);
PD_LOCAL void inputs_moveBackObject     (core::UniqueId);
PD_LOCAL void inputs_moveFrontObject    (core::UniqueId);
PD_LOCAL void inputs_snapObject         (core::UniqueId);
PD_LOCAL void inputs_positionObject     (core::UniqueId, juce::Point<int> pt);

PD_LOCAL void inputs_parameterObject    (core::UniqueId, const core::Group&);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_INPUTS_H_
