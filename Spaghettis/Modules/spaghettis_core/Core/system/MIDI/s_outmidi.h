
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_OUTMIDI_H_
#define S_OUTMIDI_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void outmidi_noteOn             (int channel, int pitch, int velocity);
void outmidi_polyPressure       (int channel, int pitch, int value);
void outmidi_controlChange      (int channel, int control, int value);
void outmidi_programChange      (int channel, int value);
void outmidi_afterTouch         (int channel, int value);
void outmidi_pitchBend          (int channel, int value);

void outmidi_sysex              (int port, int argc, t_atom *argv);
void outmidi_system             (int port, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_OUTMIDI_H_
