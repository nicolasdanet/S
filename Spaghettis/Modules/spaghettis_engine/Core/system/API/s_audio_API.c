
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

t_deviceslist audio_devices;        /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audio_vectorInitialize (t_float, int, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error audio_getDevicesList (t_deviceslist *l, int reload)
{
    static int cacheLoaded = 0;     /* Static. */
    static t_deviceslist cache;     /* Static. */
    
    t_error err = PD_ERROR_NONE;
    
    if (reload) { cacheLoaded = 0; }
    
    if (!cacheLoaded) {
    //
    deviceslist_init (&cache);
    err = audio_getListsNative (&cache);
    if (!err) { cacheLoaded = 1; outputs_reportAvailableAudioDevices (&cache); }
    //
    }
    
    deviceslist_copy (l, &cache);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audio_getDevices (t_devices *p)
{
    deviceslist_getDevices (&audio_devices, p);
}

void audio_setDevices (t_devices *p, int setParameters)
{
    int m, n;
    
    deviceslist_setDevices (&audio_devices, p, setParameters);
    
    m = deviceslist_getTotalOfChannelsIn (&audio_devices);
    n = deviceslist_getTotalOfChannelsOut (&audio_devices);
    
    audio_vectorInitialize (devices_getSampleRate (p), m, n);
    
    outputs_reportCurrentAudioDevices (&audio_devices);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Check maximum device channels (if defined) before to open stream. */

t_error audio_check (t_devices *p)
{
    t_deviceslist l; t_error err = audio_getDevicesList (&l, 0);
    
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
    
    if (err) { error_mismatch (NULL, sym_audio, sym_channels); }
    
    return err;
}

void audio_rescanDevices (void)
{
    t_deviceslist l; t_error err = audio_getDevicesList (&l, 1);
    
    PD_ASSERT (!err); PD_UNUSED (err);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int audio_deviceAsNumber (int isOutput, t_symbol *name)
{
    t_deviceslist l;
    
    if (!audio_getDevicesList (&l, 0)) {
        if (isOutput) { return deviceslist_containsOut (&l, name); }
        else { 
            return deviceslist_containsIn (&l, name);
        }
    }
    
    return -1;
}

t_error audio_deviceAsString (int isOutput, int k, char *dest, size_t size)
{
    t_error err = PD_ERROR;
    t_symbol *t = audio_deviceAsSymbol (isOutput, k);
    
    if (t) { err = string_copy (dest, size, symbol_getName (t)); }
    if (err) { *dest = 0; }
    
    return err;
}

t_symbol *audio_deviceAsSymbol (int isOutput, int k)
{
    t_deviceslist l;
    
    if (k >= 0 && !audio_getDevicesList (&l, 0)) {
    //
    return isOutput ? deviceslist_getOutAtIndex (&l, k) : deviceslist_getInAtIndex (&l, k);
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
