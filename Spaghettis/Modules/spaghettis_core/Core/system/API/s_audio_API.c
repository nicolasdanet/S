
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Note that a negative number of channels corresponds to a disabled device. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_audiodevices audio_devices;        /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audio_vectorInitialize (t_float, int, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error audio_getDevicesList (t_audiodevices *l, int reload)
{
    static int cacheLoaded = 0;     /* Static. */
    static t_audiodevices cache;    /* Static. */
    
    t_error err = PD_ERROR_NONE;
    
    if (reload) { cacheLoaded = 0; }
    
    if (!cacheLoaded) {
    //
    audiodevices_init (&cache);
    err = audio_getListsNative (&cache);
    if (!err) { cacheLoaded = 1; /* outputs_reportAvailableAudioDevices (&cache); */ }
    //
    }
    
    audiodevices_copy (l, &cache);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audio_getDevices (t_devices *p)
{
    // deviceslist_getDevices (&audio_devices, p);
}

void audio_setDevices (t_devices *p, int setParameters)
{
    /*
    int m, n;
    
    deviceslist_setDevices (&audio_devices, p, setParameters);
    
    m = deviceslist_getTotalOfChannelsIn (&audio_devices);
    n = deviceslist_getTotalOfChannelsOut (&audio_devices);
    
    audio_vectorInitialize (AUDIO_DEFAULT_SAMPLERATE, m, n);
    */
    
    // outputs_reportCurrentAudioDevices (&audio_devices);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Check maximum device channels (if defined) before to open stream. */

t_error audio_check (t_devices *p)
{
    t_audiodevices l; t_error err = audio_getDevicesList (&l, 0);
    
    /* ??? */
    
    /*
    devices_check (p);

    if (!err) {
    //
    int i;
    
    for (i = 0; i < devices_getInSize (p); i++) {
    //
    int m = devices_getInChannelsAtIndex (p, i);
    int n = deviceslist_getInChannelsAtIndex (&l, devices_getInAtIndex (p, i));
    if (n > 0 && m > n) {
        err = PD_ERROR; break;
    }
    //
    }
    //
    }
    
    if (!err) {
    //
    int i;
    
    for (i = 0; i < devices_getOutSize (p); i++) {
    //
    int m = devices_getOutChannelsAtIndex (p, i);
    int n = deviceslist_getOutChannelsAtIndex (&l, devices_getOutAtIndex (p, i));
    if (n > 0 && m > n) {
        err = PD_ERROR; break;
    }
    //
    }
    //
    }
    */
    
    if (err) { error_mismatch (NULL, sym_audio, sym_channels); }
    
    return err;
}

void audio_rescanDevices (void)
{
    t_audiodevices l; t_error err = audio_getDevicesList (&l, 1);
    
    PD_ASSERT (!err); PD_UNUSED (err);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
