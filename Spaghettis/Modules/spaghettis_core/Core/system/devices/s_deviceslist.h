
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_DEVICESLIST_H_
#define S_DEVICESLIST_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Audio/MIDI devices by full names. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _deviceslist {
    int         d_sampleRate;
    int         d_vectorSize;
    int         d_inSize;
    int         d_outSize;
    int         d_inChannels  [DEVICES_MAXIMUM_IO];
    int         d_outChannels [DEVICES_MAXIMUM_IO];
    t_symbol    *d_inNames    [DEVICES_MAXIMUM_IO];
    t_symbol    *d_outNames   [DEVICES_MAXIMUM_IO];
    } t_deviceslist;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       deviceslist_init                    (t_deviceslist *p);
void       deviceslist_reset                   (t_deviceslist *p);
void       deviceslist_copy                    (t_deviceslist *dest, t_deviceslist *src);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int        deviceslist_areEquals               (t_deviceslist *p, t_deviceslist *q);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       deviceslist_setSampleRate           (t_deviceslist *p, int n);
int        deviceslist_getSampleRate           (t_deviceslist *p);
void       deviceslist_setVectorSize           (t_deviceslist *p, int n);
int        deviceslist_getVectorSize           (t_deviceslist *p);
int        deviceslist_getInSize               (t_deviceslist *p);
int        deviceslist_getOutSize              (t_deviceslist *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int        deviceslist_getInChannelsAtIndex    (t_deviceslist *p, int i);
int        deviceslist_getOutChannelsAtIndex   (t_deviceslist *p, int i);

int        deviceslist_getTotalOfChannelsIn    (t_deviceslist *p);
int        deviceslist_getTotalOfChannelsOut   (t_deviceslist *p);

t_symbol   *deviceslist_getInAtIndex           (t_deviceslist *p, int i);
t_symbol   *deviceslist_getOutAtIndex          (t_deviceslist *p, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int        deviceslist_containsIn              (t_deviceslist *p, t_symbol *device);
int        deviceslist_containsOut             (t_deviceslist *p, t_symbol *device);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    deviceslist_appendMidiIn            (t_deviceslist *p, t_symbol *device);
t_error    deviceslist_appendMidiOut           (t_deviceslist *p, t_symbol *device);
t_error    deviceslist_appendAudioIn           (t_deviceslist *p, t_symbol *device, int channels);
t_error    deviceslist_appendAudioOut          (t_deviceslist *p, t_symbol *device, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void       deviceslist_setDevices              (t_deviceslist *l, t_devices *p, int setParameters);
void       deviceslist_getDevices              (t_deviceslist *l, t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_DEVICESLIST_H_
