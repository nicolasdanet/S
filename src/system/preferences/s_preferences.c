
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error    properties_loadBegin    (void);
PD_LOCAL void       properties_loadClose    (void);
PD_LOCAL t_error    properties_saveBegin    (void);
PD_LOCAL void       properties_saveClose    (void);
PD_LOCAL int        properties_getKey       (const char *, char *, int size);
PD_LOCAL void       properties_setKey       (const char *, const char *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void preferences_load (void)
{
    t_devices midi;    
    t_devices audio;   

    devices_initAsMidi (&midi);
    devices_initAsAudio (&audio);
    
    if (properties_loadBegin() == PD_ERROR_NONE) {
    //
    int i, t;

    char k[PD_STRING] = { 0 };
    char v[PD_STRING] = { 0 };
    
    /* Audio settings. */
    
    if (properties_getKey ("SampleRate", v, PD_STRING)) { 
        if (sscanf (v, "%d", &t) == 1) { devices_setSampleRate (&audio, t); }
    }
    
    #if PD_APPLE
    
    if (properties_getKey ("VectorSize", v, PD_STRING)) {
        if (sscanf (v, "%d", &t) == 1) { devices_setVectorSize (&audio, t); }
    }
    
    #endif
    
    /* Search paths. */
    
    for (i = 0; 1; i++) {

        string_sprintf (k, PD_STRING, "Path%d", i + 1);
        if (!properties_getKey (k, v, PD_STRING)) { break; }
        else {
            searchpath_appendRoot (v);
        }
    }
    
    /* Recent files. */
    
    for (i = 0; 1; i++) {

        string_sprintf (k, PD_STRING, "RecentFile%d", i + 1);
        if (!properties_getKey (k, v, PD_STRING)) { break; }
        else {
            recentfiles_append (v);
        }
    }
    
    /* Audio devices. */
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {

        int channels;
        
        string_sprintf (k, PD_STRING, "AudioInDeviceChannels%d", i + 1);
        if (properties_getKey (k, v, PD_STRING)) {
            if (sscanf (v, "%d", &channels) == 1) {    
                string_sprintf (k, PD_STRING, "AudioInDeviceName%d", i + 1);
                if (properties_getKey (k, v, PD_STRING)) {
                    devices_appendAudioInWithString (&audio, v, channels);
                }
            }
        }
    }
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {

        int channels;
        
        string_sprintf (k, PD_STRING, "AudioOutDeviceChannels%d", i + 1);
        if (properties_getKey (k, v, PD_STRING)) {
            if (sscanf (v, "%d", &channels) == 1) {    
                string_sprintf (k, PD_STRING, "AudioOutDeviceName%d", i + 1);
                if (properties_getKey (k, v, PD_STRING)) {
                    devices_appendAudioOutWithString (&audio, v, channels);
                }
            }
        }
    }
        
    /* MIDI devices. */
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {

        string_sprintf (k, PD_STRING, "MidiInDeviceName%d", i + 1);
        if (!properties_getKey (k, v, PD_STRING)) { break; }
        else {
            devices_appendMidiInWithString (&midi, v);
        }
    }

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {

        string_sprintf (k, PD_STRING, "MidiOutDeviceName%d", i + 1);
        if (!properties_getKey (k, v, PD_STRING)) { break; }
        else { 
            devices_appendMidiOutWithString (&midi, v);
        }
    }
    //
    }
    
    devices_setDefaultsIfNone (&midi);
    devices_setDefaultsIfNone (&audio);
    
    midi_setDevices (&midi, 1);
    audio_setDevices (&audio, 1);
    
    properties_loadClose();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void preferences_save (void)
{
    t_devices midi;    
    t_devices audio;   

    devices_initAsMidi (&midi);
    devices_initAsAudio (&audio);
    
    if (properties_saveBegin() == PD_ERROR_NONE) {
    //
    int i;
    t_pathlist *l = NULL;
    
    char k[PD_STRING] = { 0 };
    char v[PD_STRING] = { 0 };
    
    midi_getDevices (&midi);
    audio_getDevices (&audio);
    
    /* Audio settings. */
    
    string_sprintf (v, PD_STRING, "%d", devices_getSampleRate (&audio));
    properties_setKey ("SampleRate", v);
    
    #if PD_APPLE
    
    string_sprintf (v, PD_STRING, "%d", devices_getVectorSize (&audio));
    properties_setKey ("VectorSize", v);
    
    #endif
    
    /* Search paths. */
    
    l = searchpath_getRoots();
    
    for (i = 0; 1; i++) {

        if (!pathlist_getPath (l)) { break; }
        else {
            string_sprintf (k, PD_STRING, "Path%d", i + 1);
            properties_setKey (k, pathlist_getPath (l));
        }
        
        l = pathlist_getNext (l);
    }
    
    /* Recent files. */
    
    l = recentfiles_get();
    
    for (i = 0; 1; i++) {

        if (!pathlist_getPath (l)) { break; }
        else {
            string_sprintf (k, PD_STRING, "RecentFile%d", i + 1);
            properties_setKey (k, pathlist_getPath (l));
        }
        
        l = pathlist_getNext (l);
    }
    
    /* Audio devices. */
    
    for (i = 0; i < devices_getInSize (&audio); i++) {

        string_sprintf (k, PD_STRING, "AudioInDeviceName%d", i + 1);
        if (devices_getInAtIndexAsString (&audio, i, v, PD_STRING)) { break; }
        else {
            properties_setKey (k, v);
            string_sprintf (k, PD_STRING, "AudioInDeviceChannels%d", i + 1);
            string_sprintf (v, PD_STRING, "%d", devices_getInChannelsAtIndex (&audio, i));
            properties_setKey (k, v);
        }
    }

    for (i = 0; i < devices_getOutSize (&audio); i++) {

        string_sprintf (k, PD_STRING, "AudioOutDeviceName%d", i + 1);
        if (devices_getOutAtIndexAsString (&audio, i, v, PD_STRING)) { break; }
        else {
            properties_setKey (k, v);
            string_sprintf (k, PD_STRING, "AudioOutDeviceChannels%d", i + 1);
            string_sprintf (v, PD_STRING, "%d", devices_getOutChannelsAtIndex (&audio, i));
            properties_setKey (k, v);
        }
    }

    /* MIDI devices. */
    
    for (i = 0; i < devices_getInSize (&midi); i++) {

        string_sprintf (k, PD_STRING, "MidiInDeviceName%d", i + 1);
        devices_getInAtIndexAsString (&midi, i, v, PD_STRING);
        properties_setKey (k, v);
    }

    for (i = 0; i < devices_getOutSize (&midi); i++) {

        string_sprintf (k, PD_STRING, "MidiOutDeviceName%d", i + 1);
        devices_getOutAtIndexAsString (&midi, i, v, PD_STRING);
        properties_setKey (k, v);
    }
    //
    }
    
    properties_saveClose();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
