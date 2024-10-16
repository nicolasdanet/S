
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_BACKENDS_H_
#define S_BACKENDS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char     *midi_nameNative        (void);

t_error    midi_getListsNative         (t_mididevices *);
void       midi_initializeNative       (void);
void       midi_releaseNative          (void);
void       midi_openNative             (t_devices *);
void       midi_closeNative            (void);
void       midi_pushNative             (int, int, int, int);
void       midi_pushSysexNative        (int, int, t_atom *);
void       midi_pollNative             (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char     *audio_nameNative       (void);

int        audio_getVectorSizeNative   (void);
t_error    audio_getListsNative        (t_audiodevices *);
t_error    audio_initializeNative      (void);
void       audio_releaseNative         (void);
void       audio_closeNative           (void);
t_error    audio_openNative            (t_devices *);
int        audio_pollNative            (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Interface for various audio / MIDI backends. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       midi_open                   (void);
void       midi_close                  (void);

void       midi_getDevices             (t_devices *p);
void       midi_setDevices             (t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    audio_open                  (void);
void       audio_close                 (void);
int        audio_isOpened              (void);

void       audio_getDevices            (t_devices *p);
void       audio_setDevices            (t_devices *p, int setAlsoParameters);

t_error    audio_check                 (t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       midi_rescanDevices          (void);
void       audio_rescanDevices         (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int audio_getVectorSize (void)
{
    return audio_getVectorSizeNative();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void audio_safe (t_sample *sound, int size, int clamp)
{
    t_sample *s = sound; int n = size;
    
    while (n--) {
    //
    t_sample f = *s;
    
    if (PD_FLOAT32_IS_INVALID_OR_ZERO (f)) { f = (t_sample)0.0; }

    *s++ = clamp ? PD_CLAMP (f, (t_sample)-1.0, (t_sample)1.0) : f;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_BACKENDS_H_
