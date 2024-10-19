
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_initialize (t_devices *d)
{
    int i;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    d->d_in[i]  = NULL;
    d->d_out[i] = NULL;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_copy (t_devices *d, t_devices *from)
{
    int i;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    d->d_in[i]  = from->d_in[i];
    d->d_out[i] = from->d_out[i];
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error devices_checkAudio (t_devices *d)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    t_devices t; devices_initialize (&t);
    
    int i, k;

    for (i = 0, k = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_in[i];
        if (audiodevices_hasAudioIn (&l, s)) {
            t.d_in[k++] = s;
        }
    }
    
    for (i = 0, k = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_out[i];
        if (audiodevices_hasAudioOut (&l, s)) {
            t.d_out[k++] = s;
        }
    }
    
    devices_copy (d, &t);
    
    if (d->d_in[0] != NULL && d->d_out[0] != NULL) { return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
}

int devices_getTotalOfChannelsIn (t_devices *d)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    int i, k = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    k += audiodevices_getNumberOfChannelsIn (&l, d->d_in[i]);
    //
    }
    
    return k;
}

int devices_getTotalOfChannelsOut (t_devices *d)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    int i, k = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    k += audiodevices_getNumberOfChannelsOut (&l, d->d_out[i]);
    //
    }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
