
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "core_audio.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audiodevicelist_get (t_audiodevices *list)
{
    AudioObjectPropertyAddress property;
    
    property.mSelector = kAudioHardwarePropertyDevices;
    property.mScope    = kAudioObjectPropertyScopeGlobal;
    property.mElement  = kAudioObjectPropertyElementMaster;
    
    UInt32 size = 0;

    t_error err = coreaudio_error (AudioObjectGetPropertyDataSize (kAudioObjectSystemObject,
                                        &property,
                                        0,
                                        NULL,
                                        &size));

    if (!err) {
    //
    AudioObjectID *t = (AudioObjectID *)PD_MEMORY_GET (size);
    
    err = coreaudio_error (AudioObjectGetPropertyData (kAudioObjectSystemObject,
                                        &property,
                                        0,
                                        NULL,
                                        &size,
                                        t));
    
    if (!err) {
    //
    int i, numberOfDevices = size / sizeof (AudioObjectID);

    for (i = 0; i < numberOfDevices; i++) {
    //
    t_audiodevice device;
    
    if (!audiodevice_set (&device, t[i])) {
    //
    t_symbol *name = audiodevice_getName (&device);
    
    if (name) {
    //
    int m = audiodevice_getNumberOfChannelsIn (&device);
    int n = audiodevice_getNumberOfChannelsOut (&device);
    
    if (m > 0) { err |= audiodevices_appendAudioIn (list, name, m);  }
    if (n > 0) { err |= audiodevices_appendAudioOut (list, name, n); }
    //
    }
    //
    }
    //
    }
    //
    }
    
    PD_MEMORY_FREE (t);
    //
    }
    
    return err;
}

AudioObjectID audiodevicelist_fetch (t_symbol *name)
{
    AudioObjectID deviceId = kAudioObjectUnknown;
    
    if (name) {
    //
    AudioObjectPropertyAddress property;
    
    property.mSelector = kAudioHardwarePropertyDevices;
    property.mScope    = kAudioObjectPropertyScopeGlobal;
    property.mElement  = kAudioObjectPropertyElementMaster;
    
    UInt32 size = 0;

    t_error err = coreaudio_error (AudioObjectGetPropertyDataSize (kAudioObjectSystemObject,
                                        &property,
                                        0,
                                        NULL,
                                        &size));

    if (!err) {
    //
    AudioObjectID *t = (AudioObjectID *)PD_MEMORY_GET (size);
    
    err = coreaudio_error (AudioObjectGetPropertyData (kAudioObjectSystemObject,
                                        &property,
                                        0,
                                        NULL,
                                        &size,
                                        t));
    
    if (!err) {
    //
    int i, numberOfDevices = size / sizeof (AudioObjectID);

    for (i = 0; i < numberOfDevices; i++) {
    //
    t_audiodevice device;
    
    if (!audiodevice_set (&device, t[i])) {
    //
    if (name == audiodevice_getName (&device)) { deviceId = audiodevice_getIdentifier (&device); break; }
    //
    }
    //
    }
    //
    }
    
    PD_MEMORY_FREE (t);
    //
    }
    //
    }
    
    return deviceId;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
