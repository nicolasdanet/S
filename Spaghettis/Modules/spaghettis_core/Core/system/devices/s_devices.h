
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
    int d_inSize;
    int d_outSize;
    int d_in          [DEVICES_MAXIMUM_IO];
    int d_out         [DEVICES_MAXIMUM_IO];
    int d_inChannels  [DEVICES_MAXIMUM_IO];
    int d_outChannels [DEVICES_MAXIMUM_IO];
    int d_isMidi;
    } t_devices;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int devices_getSampleRate (t_devices *p)
{
    return AUDIO_DEFAULT_SAMPLERATE;
}

static inline int devices_getVectorSize (t_devices *p)
{
    return INTERNAL_BLOCKSIZE;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       devices_initAsAudio                 (t_devices *p);
void       devices_initAsMidi                  (t_devices *p);
void       devices_setDefaultsIfNone           (t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int        devices_getInSize                   (t_devices *p);
int        devices_getOutSize                  (t_devices *p);
int        devices_getInAtIndex                (t_devices *p, int i);
int        devices_getOutAtIndex               (t_devices *p, int i);
int        devices_getInChannelsAtIndex        (t_devices *p, int i);
int        devices_getOutChannelsAtIndex       (t_devices *p, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       devices_check                       (t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    devices_appendMidiIn                (t_devices *p, int n);
t_error    devices_appendMidiOut               (t_devices *p, int n);
t_error    devices_appendAudioIn               (t_devices *p, int n, int channels);
t_error    devices_appendAudioOut              (t_devices *p, int n, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    devices_getInAtIndexAsString        (t_devices *p, int i, char *dest, size_t size);
t_error    devices_getOutAtIndexAsString       (t_devices *p, int i, char *dest, size_t size);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol   *devices_getInAtIndexAsSymbol       (t_devices *p, int i);
t_symbol   *devices_getOutAtIndexAsSymbol      (t_devices *p, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    devices_appendMidiInWithString      (t_devices *p, char *device);
t_error    devices_appendMidiOutWithString     (t_devices *p, char *device);
t_error    devices_appendAudioInWithString     (t_devices *p, char *device, int channels);
t_error    devices_appendAudioOutWithString    (t_devices *p, char *device, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_DEVICES_H_
