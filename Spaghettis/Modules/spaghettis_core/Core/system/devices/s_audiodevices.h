
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_AUDIODEVICES_H_
#define S_AUDIODEVICES_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _audiodevices {
    int         d_inSize;
    int         d_outSize;
    int         d_inChannels  [DEVICES_MAXIMUM_IO];
    int         d_outChannels [DEVICES_MAXIMUM_IO];
    t_symbol    *d_inNames    [DEVICES_MAXIMUM_IO];
    t_symbol    *d_outNames   [DEVICES_MAXIMUM_IO];
    } t_audiodevices;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void    audiodevices_init           (t_audiodevices *p);
void    audiodevices_copy           (t_audiodevices *dest, t_audiodevices *src);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audiodevices_addAudioIn     (t_audiodevices *p, t_symbol *device, int channels);
t_error audiodevices_addAudioOut    (t_audiodevices *p, t_symbol *device, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_AUDIODEVICES_H_