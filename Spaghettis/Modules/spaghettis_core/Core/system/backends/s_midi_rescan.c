
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error midi_getListOfDevicesCached (t_mididevices *l, int reload)
{
    static int cacheLoaded = 0;             /* Static. */
    static t_mididevices cache;             /* Static. */
    
    t_error err = PD_ERROR_NONE;
    
    if (reload) { cacheLoaded = 0; }
    
    if (!cacheLoaded) {
    //
    mididevices_initialize (&cache);
    err = midi_getListOfDevicesNative (&cache);
    if (!err) {
        cacheLoaded = 1;
        #if defined ( PD_BUILDING_APPLICATION )
            outputs_reportMidiDevices (&cache);
        #endif
    }
    //
    }
    
    mididevices_copy (l, &cache);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_getListOfDevices (t_mididevices *l)
{
    t_error err = midi_getListOfDevicesCached (l, 0);
    
    PD_ASSERT (!err); PD_UNUSED (err);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_rescanDevices (int isLogged)
{
    t_mididevices l; t_error err = midi_getListOfDevicesCached (&l, 1);
    
    PD_ASSERT (!err);
    
    if (!err && isLogged) { mididevices_report (&l); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
