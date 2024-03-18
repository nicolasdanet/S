
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_deviceslist midi_devices;          /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error midi_getDevicesList (t_deviceslist *l, int reload)
{
    static int cacheLoaded = 0;             /* Static. */
    static t_deviceslist cache;             /* Static. */
    
    t_error err = PD_ERROR_NONE;
    
    if (reload) { cacheLoaded = 0; }
    
    if (!cacheLoaded) {
    //
    deviceslist_init (&cache);
    err = midi_getListsNative (&cache);
    if (!err) { cacheLoaded = 1; outputs_reportAvailableMidiDevices (&cache); }
    //
    }
    
    deviceslist_copy (l, &cache);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_open (void)
{
    t_devices midi; devices_initAsMidi (&midi);
     
    midi_getDevices (&midi);
    midi_openNative (&midi);
}

void midi_close (void)
{
    midi_closeNative();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_getDevices (t_devices *p)
{
    deviceslist_getDevices (&midi_devices, p);
}

void midi_setDevices (t_devices *p, int setParameters)
{
    t_deviceslist old; deviceslist_copy (&old, &midi_devices);
    
    deviceslist_setDevices (&midi_devices, p, setParameters);
    
    if (!deviceslist_areEquals (&old, &midi_devices) && symbol_hasThingQuiet (sym__midiports)) {
    //
    pd_message (symbol_getThing (sym__midiports), sym__midiports, 0, NULL);
    //
    }
    
    outputs_reportCurrentMidiDevices (&midi_devices);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_rescanDevices (void)
{
    t_deviceslist l; t_error err = midi_getDevicesList (&l, 1);
    
    PD_ASSERT (!err); PD_UNUSED (err);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int midi_deviceAsNumber (int isOutput, t_symbol *name)
{
    t_deviceslist l;
    
    if (!midi_getDevicesList (&l, 0)) {
        if (isOutput) { return deviceslist_containsOut (&l, name); }
        else { 
            return deviceslist_containsIn (&l, name);
        }
    }
    
    return -1;
}

t_error midi_deviceAsString (int isOutput, int k, char *dest, size_t size)
{
    t_error err = PD_ERROR;
    t_symbol *t = midi_deviceAsSymbol (isOutput, k);
    
    if (t) { err = string_copy (dest, size, t->s_name); }
    
    if (err) { *dest = 0; }
    
    return err;
}

t_symbol *midi_deviceAsSymbol (int isOutput, int k)
{
    t_deviceslist l;
    
    if (k >= 0 && !midi_getDevicesList (&l, 0)) {
    //
    return isOutput ? deviceslist_getOutAtIndex (&l, k) : deviceslist_getInAtIndex (&l, k);
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
