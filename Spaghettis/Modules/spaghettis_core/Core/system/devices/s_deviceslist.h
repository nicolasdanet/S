
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_DEVICESLIST_H_
#define S_DEVICESLIST_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _deviceslist {
    int         d_sampleRate;
    int         d_vectorSize;
    int         d_inSize;
    int         d_outSize;
    int         d_inChannels  [DEVICES_MAXIMUM_IO];     // --
    int         d_outChannels [DEVICES_MAXIMUM_IO];     // --
    t_symbol    *d_inNames    [DEVICES_MAXIMUM_IO];     // --
    t_symbol    *d_outNames   [DEVICES_MAXIMUM_IO];     // --
    } t_deviceslist;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       deviceslist_init                    (t_deviceslist *p);
PD_LOCAL void       deviceslist_reset                   (t_deviceslist *p);
PD_LOCAL void       deviceslist_copy                    (t_deviceslist *dest, t_deviceslist *src);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int        deviceslist_areEquals               (t_deviceslist *p, t_deviceslist *q);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       deviceslist_setSampleRate           (t_deviceslist *p, int n);
PD_LOCAL int        deviceslist_getSampleRate           (t_deviceslist *p);
PD_LOCAL void       deviceslist_setVectorSize           (t_deviceslist *p, int n);
PD_LOCAL int        deviceslist_getVectorSize           (t_deviceslist *p);
PD_LOCAL int        deviceslist_getInSize               (t_deviceslist *p);
PD_LOCAL int        deviceslist_getOutSize              (t_deviceslist *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int        deviceslist_getInChannelsAtIndex    (t_deviceslist *p, int i);
PD_LOCAL int        deviceslist_getOutChannelsAtIndex   (t_deviceslist *p, int i);

PD_LOCAL int        deviceslist_getTotalOfChannelsIn    (t_deviceslist *p);
PD_LOCAL int        deviceslist_getTotalOfChannelsOut   (t_deviceslist *p);

PD_LOCAL t_symbol   *deviceslist_getInAtIndex           (t_deviceslist *p, int i);
PD_LOCAL t_symbol   *deviceslist_getOutAtIndex          (t_deviceslist *p, int i);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int        deviceslist_containsIn              (t_deviceslist *p, t_symbol *device);
PD_LOCAL int        deviceslist_containsOut             (t_deviceslist *p, t_symbol *device);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    deviceslist_appendMidiIn            (t_deviceslist *p, t_symbol *device);
PD_LOCAL t_error    deviceslist_appendMidiOut           (t_deviceslist *p, t_symbol *device);
PD_LOCAL t_error    deviceslist_appendAudioIn           (t_deviceslist *p, t_symbol *device, int channels);
PD_LOCAL t_error    deviceslist_appendAudioOut          (t_deviceslist *p, t_symbol *device, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void       deviceslist_setDevices              (t_deviceslist *l, t_devices *p, int setParameters);
PD_LOCAL void       deviceslist_getDevices              (t_deviceslist *l, t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_DEVICESLIST_H_
