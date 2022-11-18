
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_API_H_
#define S_API_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL const char     *midi_nameNative        (void);

PD_LOCAL t_error    midi_getListsNative         (t_deviceslist *);
PD_LOCAL void       midi_initializeNative       (void);
PD_LOCAL void       midi_releaseNative          (void);
PD_LOCAL void       midi_openNative             (t_devices *);
PD_LOCAL void       midi_closeNative            (void);
PD_LOCAL void       midi_pushNative             (int, int, int, int);
PD_LOCAL void       midi_pushSysexNative        (int, int, t_atom *);
PD_LOCAL void       midi_pollNative             (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL const char     *audio_nameNative       (void);

PD_LOCAL int        audio_getVectorSizeNative   (void);
PD_LOCAL t_error    audio_getListsNative        (t_deviceslist *);
PD_LOCAL t_error    audio_initializeNative      (void);
PD_LOCAL void       audio_releaseNative         (void);
PD_LOCAL void       audio_closeNative           (void);
PD_LOCAL t_error    audio_openNative            (t_devices *);
PD_LOCAL int        audio_pollNative            (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Interface for various audio / MIDI backends. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       midi_open                   (void);
PD_LOCAL void       midi_close                  (void);

PD_LOCAL void       midi_getDevices             (t_devices *p);
PD_LOCAL void       midi_setDevices             (t_devices *p, int setAlsoParameters);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    audio_open                  (void);
PD_LOCAL void       audio_close                 (void);
PD_LOCAL int        audio_isOpened              (void);

PD_LOCAL void       audio_getDevices            (t_devices *p);
PD_LOCAL void       audio_setDevices            (t_devices *p, int setAlsoParameters);

PD_LOCAL t_error    audio_check                 (t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int        midi_deviceAsNumber         (int isOutput, t_symbol *name);
PD_LOCAL int        audio_deviceAsNumber        (int isOutput, t_symbol *name);

PD_LOCAL t_symbol   *midi_deviceAsSymbol        (int isOutput, int k);
PD_LOCAL t_symbol   *audio_deviceAsSymbol       (int isOutput, int k);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL t_error    midi_deviceAsString         (int isOutput, int k, char *dest, size_t size);
PD_LOCAL t_error    audio_deviceAsString        (int isOutput, int k, char *dest, size_t size);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       midi_rescanDevices          (void);
PD_LOCAL void       audio_rescanDevices         (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

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
#endif // S_API_H_
