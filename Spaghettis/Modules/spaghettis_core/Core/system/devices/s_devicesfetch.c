
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_symbol *devices_getInAtIndexAsSymbol (t_devices *p, int i)
{
    if (p->d_isMidi) {
        return midi_deviceAsSymbol (0,  devices_getInAtIndex (p, i));
    } else {
        return audio_deviceAsSymbol (0, devices_getInAtIndex (p, i));
    }
}

PD_LOCAL t_symbol *devices_getOutAtIndexAsSymbol (t_devices *p, int i)
{
    if (p->d_isMidi) {
        return midi_deviceAsSymbol (1,  devices_getOutAtIndex (p, i));
    } else {
        return audio_deviceAsSymbol (1, devices_getOutAtIndex (p, i));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error devices_getInAtIndexAsString (t_devices *p, int i, char *dest, size_t size)
{
    if (p->d_isMidi) {
        return midi_deviceAsString (0,  devices_getInAtIndex (p, i), dest, size);
    } else {
        return audio_deviceAsString (0, devices_getInAtIndex (p, i), dest, size);
    }
}

PD_LOCAL t_error devices_getOutAtIndexAsString (t_devices *p, int i, char *dest, size_t size)
{
    if (p->d_isMidi) {
        return midi_deviceAsString (1,  devices_getOutAtIndex (p, i), dest, size);
    } else {
        return audio_deviceAsString (1, devices_getOutAtIndex (p, i), dest, size);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error devices_appendMidiInWithSymbol (t_devices *p, t_symbol *device)
{
    if (device) { return devices_appendMidiIn (p, midi_deviceAsNumber (0, device)); }
    
    return PD_ERROR;
}

static t_error devices_appendMidiOutWithSymbol (t_devices *p, t_symbol *device)
{
    if (device) { return devices_appendMidiOut (p, midi_deviceAsNumber (1, device)); }
    
    return PD_ERROR;
}

static t_error devices_appendAudioInWithSymbol (t_devices *p, t_symbol *device, int channels)
{
    if (device) { return devices_appendAudioIn (p, audio_deviceAsNumber (0, device), channels); }
    
    return PD_ERROR;
}

static t_error devices_appendAudioOutWithSymbol (t_devices *p, t_symbol *device, int channels)
{
    if (device) { return devices_appendAudioOut (p, audio_deviceAsNumber (1, device), channels); }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error devices_appendMidiInWithString (t_devices *p, char *device)
{
    if (device) { return devices_appendMidiInWithSymbol (p, gensym (device)); }
    
    return PD_ERROR;
}

PD_LOCAL t_error devices_appendMidiOutWithString (t_devices *p, char *device)
{
    if (device) { return devices_appendMidiOutWithSymbol (p, gensym (device)); }
    
    return PD_ERROR;
}

PD_LOCAL t_error devices_appendAudioInWithString (t_devices *p, char *device, int channels)
{
    if (device) { return devices_appendAudioInWithSymbol (p, gensym (device), channels); }
    
    return PD_ERROR;
}

PD_LOCAL t_error devices_appendAudioOutWithString (t_devices *p, char *device, int channels)
{
    if (device) { return devices_appendAudioOutWithSymbol (p, gensym (device), channels); }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

PD_LOCAL t_error devices_appendAudioIn (t_devices *p, const AudioDevice& d)
{
    t_symbol *s = gensym (AudioDevices::getName (d).toRawUTF8());

    return devices_appendAudioInWithSymbol (p, s, AudioDevices::getChannels (d));
}

PD_LOCAL t_error devices_appendAudioOut (t_devices *p, const AudioDevice& d)
{
    t_symbol *s = gensym (AudioDevices::getName (d).toRawUTF8());

    return devices_appendAudioOutWithSymbol (p, s, AudioDevices::getChannels (d));
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error deviceslist_appendMidiInAsNumber (t_deviceslist *p, int n)
{
    t_symbol *t = midi_deviceAsSymbol (0, n);
    
    if (t) { return deviceslist_appendMidiIn (p, t); }
    
    return PD_ERROR;
}

static t_error deviceslist_appendMidiOutAsNumber (t_deviceslist *p, int n)
{
    t_symbol *t = midi_deviceAsSymbol (1, n);
    
    if (t) { return deviceslist_appendMidiOut (p, t); }
    
    return PD_ERROR;
}

static t_error deviceslist_appendAudioInAsNumber (t_deviceslist *p, int n, int channels)
{
    t_symbol *t = audio_deviceAsSymbol (0, n);
    
    if (t) { return deviceslist_appendAudioIn (p, t, channels); }
    
    return PD_ERROR;
}

static t_error deviceslist_appendAudioOutAsNumber (t_deviceslist *p, int n, int channels)
{
    t_symbol *t = audio_deviceAsSymbol (1, n);
    
    if (t) { return deviceslist_appendAudioOut (p, t, channels); }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void deviceslist_setDevices (t_deviceslist *l, t_devices *p, int setParameters)
{
    int i;
    
    if (setParameters) { deviceslist_init (l); } else { deviceslist_reset (l); }
    
    if (p->d_isMidi) {
    
        for (i = 0; i < devices_getInSize (p); i++) {
            deviceslist_appendMidiInAsNumber (l, devices_getInAtIndex (p, i));
        }
        
        for (i = 0; i < devices_getOutSize (p); i++) {
            deviceslist_appendMidiOutAsNumber (l, devices_getOutAtIndex (p, i));
        }
    
    } else {
        
        if (setParameters) { deviceslist_setSampleRate (l, devices_getSampleRate (p)); }
        if (setParameters) { deviceslist_setVectorSize (l, devices_getVectorSize (p)); }

        for (i = 0; i < devices_getInSize (p); i++) {
            deviceslist_appendAudioInAsNumber (l,
                devices_getInAtIndex (p, i),
                devices_getInChannelsAtIndex (p, i));
        }

        for (i = 0; i < devices_getOutSize (p); i++) {
            deviceslist_appendAudioOutAsNumber (l,
                devices_getOutAtIndex (p, i),
                devices_getOutChannelsAtIndex (p, i));
        }
    }
}

PD_LOCAL void deviceslist_getDevices (t_deviceslist *l, t_devices *p)
{
    int i;
    
    if (p->d_isMidi) {
        
        for (i = 0; i < deviceslist_getInSize (l); i++) {
            devices_appendMidiInWithSymbol (p, deviceslist_getInAtIndex (l, i));
        }
        
        for (i = 0; i < deviceslist_getOutSize (l); i++) {
            devices_appendMidiOutWithSymbol (p, deviceslist_getOutAtIndex (l, i));
        }
    
    } else {
    
        devices_setSampleRate (p, deviceslist_getSampleRate (l));
        devices_setVectorSize (p, deviceslist_getVectorSize (l));
        
        for (i = 0; i < deviceslist_getInSize (l); i++) {
            devices_appendAudioInWithSymbol (p,
                deviceslist_getInAtIndex (l, i),
                deviceslist_getInChannelsAtIndex (l, i));
        }
        
        for (i = 0; i < deviceslist_getOutSize (l); i++) {
            devices_appendAudioOutWithSymbol (p,
                deviceslist_getOutAtIndex (l, i),
                deviceslist_getOutChannelsAtIndex (l, i));
        }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
