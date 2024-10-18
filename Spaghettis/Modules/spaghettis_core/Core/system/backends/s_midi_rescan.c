
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error midi_getListOfDevices (t_mididevices *l, int reload)
{
    static int cacheLoaded = 0;             /* Static. */
    static t_mididevices cache;             /* Static. */
    
    t_error err = PD_ERROR_NONE;
    
    if (reload) { cacheLoaded = 0; }
    
    if (!cacheLoaded) {
    //
    mididevices_initialize (&cache);
    err = midi_getListOfDevicesNative (&cache);
    if (!err) { cacheLoaded = 1; /* outputs_reportAvailableMidiDevices (&cache); */ }
    //
    }
    
    mididevices_copy (l, &cache);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_rescanDevices (int isLogged)
{
    t_mididevices l; t_error err = midi_getListOfDevices (&l, 1);
    
    PD_ASSERT (!err);
    
    if (!err && isLogged) { mididevices_report (&l); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
