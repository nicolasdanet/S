
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error audio_getListOfDevices (t_audiodevices *l, int reload)
{
    static int cacheLoaded = 0;     /* Static. */
    static t_audiodevices cache;    /* Static. */
    
    t_error err = PD_ERROR_NONE;
    
    if (reload) { cacheLoaded = 0; }
    
    if (!cacheLoaded) {
    //
    audiodevices_init (&cache);
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

void audio_rescanDevices (void)
{
    t_audiodevices l; t_error err = audio_getListOfDevices (&l, 1);
    
    PD_ASSERT (!err); PD_UNUSED (err);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
