
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error audio_getListOfDevicesCached (t_audiodevices *l, int reload)
{
    static int cacheLoaded = 0;     /* Static. */
    static t_audiodevices cache;    /* Static. */
    
    t_error err = PD_ERROR_NONE;
    
    if (reload) { cacheLoaded = 0; }
    
    if (!cacheLoaded) {
    //
    audiodevices_initialize (&cache);
    err = audio_getListOfDevicesNative (&cache);
    if (!err) { cacheLoaded = 1; /* outputs_reportAvailableAudioDevices (&cache); */ }
    //
    }
    
    audiodevices_copy (l, &cache);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audio_rescanDevices (int isLogged)
{
    t_audiodevices l; t_error err = audio_getListOfDevicesCached (&l, 1);
    
    PD_ASSERT (!err);
    
    if (!err && isLogged) { audiodevices_report (&l); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
