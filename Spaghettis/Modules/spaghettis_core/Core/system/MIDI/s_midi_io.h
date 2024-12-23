
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_MIDI_IO_H_
#define S_MIDI_IO_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define MIDI_NOTEOFF            0x80    // 128
#define MIDI_NOTEON             0x90    // 144
#define MIDI_POLYPRESSURE       0xa0    // 160
#define MIDI_CONTROLCHANGE      0xb0    // 176
#define MIDI_PROGRAMCHANGE      0xc0    // 192
#define MIDI_AFTERTOUCH         0xd0    // 208
#define MIDI_PITCHBEND          0xe0    // 224

#define MIDI_STARTSYSEX         0xf0    // 240
#define MIDI_TIMECODE           0xf1    // 241
#define MIDI_SONGPOS            0xf2    // 242
#define MIDI_SONGSELECT         0xf3    // 243
#define MIDI_RESERVED1          0xf4    // 244
#define MIDI_RESERVED2          0xf5    // 245
#define MIDI_TUNEREQUEST        0xf6    // 246
#define MIDI_ENDSYSEX           0xf7    // 247

#define MIDI_CLOCK              0xf8    // 248
#define MIDI_TICK               0xf9    // 249
#define MIDI_START              0xfa    // 250
#define MIDI_CONTINUE           0xfb    // 251
#define MIDI_STOP               0xfc    // 252
#define MIDI_RESERVED3          0xfd    // 253
#define MIDI_ACTIVESENSE        0xfe    // 254
#define MIDI_RESET              0xff    // 255

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define MIDI_IS_STATUS(b)       (((b) & 0x80) != 0)

#define MIDI_IS_VOICE(b)        (MIDI_IS_STATUS (b) && ((b) <  MIDI_STARTSYSEX))
#define MIDI_IS_SYSEX(b)        (MIDI_IS_STATUS (b) && ((b) == MIDI_STARTSYSEX || (b) == MIDI_ENDSYSEX))
#define MIDI_IS_COMMON(b)       (MIDI_IS_STATUS (b) && ((b) >= MIDI_TIMECODE   && (b) <= MIDI_TUNEREQUEST))
#define MIDI_IS_REALTIME(b)     (MIDI_IS_STATUS (b) && ((b) >= MIDI_CLOCK))

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_receive               (int port, int status, int a, int b);
void midi_send                  (int port, int status, int a, int b);

void midi_receiveSysex          (int port, uint8_t byte);
void midi_sendSysex             (int port, int argc, t_atom *argv);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_MIDI_IO_H_
