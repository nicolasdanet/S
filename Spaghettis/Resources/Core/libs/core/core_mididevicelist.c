
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "core_midi.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* < https://stackoverflow.com/a/20368001 > */
/* < https://github.com/thestk/rtmidi/blob/master/RtMidi.cpp > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static void core_pump (void)
{
    while (CFRunLoopRunInMode (kCFRunLoopDefaultMode, 0, true) == kCFRunLoopRunHandledSource) { }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error mididevicelist_getSources (t_deviceslist *list)
{
    core_pump();
    
    {
        t_error err = PD_ERROR_NONE;
        
        ItemCount i, n = MIDIGetNumberOfSources();

        for (i = 0; i < n; i++) {
        //
        t_symbol *name = midiname_get (MIDIGetSource (i));
        
        if (name) { err |= deviceslist_appendMidiIn (list, name); }
        //
        }

        return err;
    }
}

static t_error mididevicelist_getDestinations (t_deviceslist *list)
{
    core_pump();
    
    {
        t_error err = PD_ERROR_NONE;
        
        ItemCount i, n = MIDIGetNumberOfDestinations();

        for (i = 0; i < n; i++) {
        //
        t_symbol *name = midiname_get (MIDIGetDestination (i));
        
        if (name) { err |= deviceslist_appendMidiOut (list, name); }
        //
        }

        return err;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error mididevicelist_get (t_deviceslist *list)
{
    t_error err = PD_ERROR_NONE;
    
    err |= mididevicelist_getSources (list);
    err |= mididevicelist_getDestinations (list);
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL MIDIEndpointRef mididevicelist_fetchSource (t_symbol *name)
{
    core_pump();
    
    {
        ItemCount i, n = MIDIGetNumberOfSources();

        for (i = 0; i < n; i++) {
        //
        MIDIEndpointRef t = MIDIGetSource (i);
        
        if (t && midiname_get (t) == name) { return t; }
        //
        }
        
        return (MIDIEndpointRef)NULL;
    }
}

PD_LOCAL MIDIEndpointRef mididevicelist_fetchDestination (t_symbol *name)
{
    core_pump();
    
    {
        ItemCount i, n = MIDIGetNumberOfDestinations();

        for (i = 0; i < n; i++) {
        //
        MIDIEndpointRef t = MIDIGetDestination (i);
        
        if (t && midiname_get (t) == name) { return t; }
        //
        }
        
        return (MIDIEndpointRef)NULL;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
