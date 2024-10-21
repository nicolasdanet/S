
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../libs/core/core_midi.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int core_numberOfPortIn;                             /* Static. */
static int core_numberOfPortOut;                            /* Static. */

static t_midiport core_portIn[DEVICES_MAXIMUM_IO];          /* Static. */
static t_midiport core_portOut[DEVICES_MAXIMUM_IO];         /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

const char *midi_getBackendNameNative (void)
{
    static const char *name = "Core MIDI"; return name;     /* Static. */
}

void midi_initializeNative (void)
{
    coreclient_initialize();
}

void midi_releaseNative (void)
{
    coreclient_release();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error midi_openNativeSource (t_symbol *name)
{
    t_error err = midiport_openInput (&core_portIn[core_numberOfPortIn], name);
    
    core_numberOfPortIn++;
    
    return err;
}

static t_error midi_openNativeDestination (t_symbol *name)
{
    t_error err = midiport_openOutput (&core_portOut[core_numberOfPortOut], name);
    
    core_numberOfPortOut++;
    
    return err;
}

t_error midi_openNative (t_devices *p)
{
    t_error err = PD_ERROR_NONE;
    
    int m = devices_getInSize (p);
    int n = devices_getOutSize (p);
    
    core_numberOfPortIn  = 0;
    core_numberOfPortOut = 0;

    PD_ASSERT (m < DEVICES_MAXIMUM_IO);
    PD_ASSERT (n < DEVICES_MAXIMUM_IO);
    
    if (m || n) {
    //
    int i;
    
    for (i = 0; i < m; i++)  {
        t_symbol *s = devices_getInName (p, i);
        t_error e   = midi_openNativeSource (s);
        if (e) {
            err |= PD_ERROR;
            error_canNotOpen (NULL, s);
        }
    }
    
    for (i = 0; i < n; i++) {
        t_symbol *s = devices_getOutName (p, i);
        t_error e   = midi_openNativeDestination (s);
        if (e) {
            err |= PD_ERROR;
            error_canNotOpen (NULL, s);
        }
    }
    //
    }
    
    return err;
}

void midi_closeNative (void)
{
    int i;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) { midiport_close (&core_portIn[i]);  }
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) { midiport_close (&core_portOut[i]); }
    
    core_numberOfPortIn  = 0;
    core_numberOfPortOut = 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_pushNative (int port, int status, int a, int b)
{
    if (port >= 0 && port < core_numberOfPortOut) { midiport_push (&core_portOut[port], status, a, b); }
}

void midi_pushSysexNative (int port, int argc, t_atom *argv)
{
    if (port >= 0 && port < core_numberOfPortOut) { midiport_pushSysex (&core_portOut[port], argc, argv); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midi_pollNative (void)
{
    int i; for (i = 0; i < core_numberOfPortIn; i++) { midiport_poll (&core_portIn[i], i); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error midi_getListOfDevicesNative (t_mididevices *p)
{
    return mididevicelist_get (p);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
