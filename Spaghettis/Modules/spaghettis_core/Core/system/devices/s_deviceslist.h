
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
void       deviceslist_copy                    (t_deviceslist *dest, t_deviceslist *src);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    deviceslist_appendMidiIn            (t_deviceslist *p, t_symbol *device);
t_error    deviceslist_appendMidiOut           (t_deviceslist *p, t_symbol *device);
t_error    deviceslist_appendAudioIn           (t_deviceslist *p, t_symbol *device, int channels);
t_error    deviceslist_appendAudioOut          (t_deviceslist *p, t_symbol *device, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_DEVICESLIST_H_
