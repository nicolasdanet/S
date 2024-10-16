
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_INMIDI_H_
#define S_INMIDI_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inmidi_noteOn              (int port, int channel, int pitch, int velocity);
void inmidi_polyPressure        (int port, int channel, int pitch, int value);
void inmidi_controlChange       (int port, int channel, int control, int value);
void inmidi_programChange       (int port, int channel, int value);
void inmidi_afterTouch          (int port, int channel, int value);
void inmidi_pitchBend           (int port, int channel, int value);

void inmidi_sysex               (int port, int argc, t_atom *argv);
void inmidi_system              (int port, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_INMIDI_H_
