
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_error midi_getStatusWithSymbol (t_symbol *, int *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void outmidi_noteOn (int channel, int pitch, int velocity)
{
    int t    = PD_MAX (0, channel - 1);
    pitch    = PD_CLAMP (pitch, 0, 127);
    velocity = PD_CLAMP (velocity, 0, 127);

    midi_send ((t >> 4), MIDI_NOTEON + (t & 0xf), pitch, velocity);
}

void outmidi_polyPressure (int channel, int pitch, int value)
{
    int t = PD_MAX (0, channel - 1);
    pitch = PD_CLAMP (pitch, 0, 127);
    value = PD_CLAMP (value, 0, 127);
    
    midi_send ((t >> 4), MIDI_POLYPRESSURE + (t & 0xf), pitch, value);
}

void outmidi_controlChange (int channel, int control, int value)
{
    int t   = PD_MAX (0, channel - 1);
    control = PD_CLAMP (control, 0, 127);
    value   = PD_CLAMP (value, 0, 127);
    
    midi_send ((t >> 4), MIDI_CONTROLCHANGE + (t & 0xf), control, value);
}

void outmidi_programChange (int channel, int value)
{
    int t = PD_MAX (0, channel - 1);
    value = PD_CLAMP (value - 1, 0, 127);
    
    midi_send ((t >> 4), MIDI_PROGRAMCHANGE + (t & 0xf), value, 0);
}

void outmidi_afterTouch (int channel, int value)
{
    int t = PD_MAX (0, channel - 1);
    value = PD_CLAMP (value, 0, 127);
    
    midi_send ((t >> 4), MIDI_AFTERTOUCH + (t & 0xf), value, 0);
}

void outmidi_pitchBend (int channel, int value)
{
    int t = PD_MAX (0, channel - 1);
    value = PD_CLAMP (value, 0, 16383);     // 0x3fff
    
    midi_send ((t >> 4), MIDI_PITCHBEND + (t & 0xf), (value & 0x7f), ((value >> 7) & 0x7f));
}

void outmidi_sysex (int port, int argc, t_atom *argv)
{
    midi_sendSysex (port, argc, argv);
}

void outmidi_system (int port, int argc, t_atom *argv)
{
    int status;
    
    if (!midi_getStatusWithSymbol (atom_getSymbolAtIndex (0, argc, argv), &status)) {
    //
    int a = (int)atom_getFloatAtIndex (1, argc, argv);
    int b = (int)atom_getFloatAtIndex (2, argc, argv);
    
    midi_send (port, status, (a & 0x7f), (b & 0x7f));
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
