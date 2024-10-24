
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

void    audiodevices_initialize                 (t_audiodevices *p);
void    audiodevices_copy                       (t_audiodevices *dest, t_audiodevices *src);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audiodevices_appendAudioIn              (t_audiodevices *p, t_symbol *device, int channels);
t_error audiodevices_appendAudioOut             (t_audiodevices *p, t_symbol *device, int channels);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int     audiodevices_hasAudioIn                 (t_audiodevices *p, t_symbol *device);
int     audiodevices_hasAudioOut                (t_audiodevices *p, t_symbol *device);

int     audiodevices_getNumberOfChannelsIn      (t_audiodevices *p, t_symbol *device);
int     audiodevices_getNumberOfChannelsOut     (t_audiodevices *p, t_symbol *device);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void    audiodevices_report                     (t_audiodevices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

juce::StringArray audiodevices_getListIn        (t_audiodevices *p);
juce::StringArray audiodevices_getListOut       (t_audiodevices *p);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_AUDIODEVICES_H_
