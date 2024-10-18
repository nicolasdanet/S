
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_MIDI_BACKENDS_H_
#define S_MIDI_BACKENDS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        midi_getListOfDevices           (t_mididevices *);
void        midi_rescanDevices              (int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char  *midi_getBackendNameNative      (void);

t_error     midi_getListOfDevicesNative     (t_mididevices *);
void        midi_initializeNative           (void);
void        midi_releaseNative              (void);
void        midi_openNative                 (t_devices *);
void        midi_closeNative                (void);
void        midi_pushNative                 (int, int, int, int);
void        midi_pushSysexNative            (int, int, t_atom *);
void        midi_pollNative                 (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_MIDI_BACKENDS_H_
