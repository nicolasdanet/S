
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

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // W_INPUTS_H_
