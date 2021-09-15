
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_DEVICES_H_
#define S_DEVICES_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Audio/MIDI devices by numbers. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define DEVICES_MAXIMUM_IO          8
#define DEVICES_MAXIMUM_CHANNELS    32

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* A negative number of channels is used for inactivated devices. */

typedef struct _devices {
    int d_sampleRate;
    int d_vectorSize;
    int d_inSize;
    int d_outSize;
    int d_in          [DEVICES_MAXIMUM_IO];             // --
    int d_out         [DEVICES_MAXIMUM_IO];             // --
    int d_inChannels  [DEVICES_MAXIMUM_IO];             // --
    int d_outChannels [DEVICES_MAXIMUM_IO];             // --
    int d_isMidi;
    } t_devices;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       devices_initAsAudio                 (t_devices *p);
PD_LOCAL void       devices_initAsMidi                  (t_devices *p);
PD_LOCAL void       devices_setDefaultsIfNone           (t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       devices_setSampleRate               (t_devices *p, int n);
PD_LOCAL int        devices_getSampleRate               (t_devices *p);
PD_LOCAL void       devices_setVectorSize               (t_devices *p, int n);
PD_LOCAL int        devices_getVectorSize               (t_devices *p);
PD_LOCAL int        devices_getInSize                   (t_devices *p);
PD_LOCAL int        devices_getOutSize                  (t_devices *p);
PD_LOCAL int        devices_getInAtIndex                (t_devices *p, int i);
PD_LOCAL int        devices_getOutAtIndex               (t_devices *p, int i);
PD_LOCAL int        devices_getInChannelsAtIndex        (t_devices *p, int i);
PD_LOCAL int        devices_getOutChannelsAtIndex       (t_devices *p, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       devices_check                       (t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    devices_appendMidiIn                (t_devices *p, int n);
PD_LOCAL t_error    devices_appendMidiOut               (t_devices *p, int n);
PD_LOCAL t_error    devices_appendAudioIn               (t_devices *p, int n, int channels);
PD_LOCAL t_error    devices_appendAudioOut              (t_devices *p, int n, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    devices_getInAtIndexAsString        (t_devices *p, int i, char *dest, size_t size);
PD_LOCAL t_error    devices_getOutAtIndexAsString       (t_devices *p, int i, char *dest, size_t size);

PD_LOCAL t_symbol   *devices_getInAtIndexAsSymbol       (t_devices *p, int i);
PD_LOCAL t_symbol   *devices_getOutAtIndexAsSymbol      (t_devices *p, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    devices_appendMidiInWithString      (t_devices *p, char *device);
PD_LOCAL t_error    devices_appendMidiOutWithString     (t_devices *p, char *device);
PD_LOCAL t_error    devices_appendAudioInWithString     (t_devices *p, char *device, int channels);
PD_LOCAL t_error    devices_appendAudioOutWithString    (t_devices *p, char *device, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_DEVICES_H_
