
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "core_audio.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error audiodevice_set (t_audiodevice *device, AudioObjectID deviceID)
{
    device->d_ID            = deviceID;
    device->d_unit          = NULL;
    device->d_hasListener   = 0;
    device->d_isInput       = 0;
    device->d_isOutput      = 0;
    device->d_isDuplex      = 0;
    device->d_isReady       = 0;
    device->d_isRunning     = 0;
    
    return (device->d_ID == kAudioObjectUnknown);
}

PD_LOCAL t_error audiodevice_setAsInput (t_audiodevice *device, AudioObjectID deviceID)
{
    t_error err = audiodevice_set (device, deviceID);
    
    if (!err) { device->d_isInput  = 1; }
    
    return err;
}

PD_LOCAL t_error audiodevice_setAsOutput (t_audiodevice *device, AudioObjectID deviceID)
{
    t_error err = audiodevice_set (device, deviceID);
    
    if (!err) { device->d_isOutput = 1; }
    
    return err;
}

PD_LOCAL t_error audiodevice_setAsDuplex (t_audiodevice *device, AudioObjectID deviceID)
{
    t_error err = audiodevice_set (device, deviceID);
    
    if (!err) { device->d_isDuplex = 1; }
    
    return err;
}

PD_LOCAL AudioObjectID audiodevice_getIdentifier (t_audiodevice *device)
{
    return device->d_ID;
}

PD_LOCAL void audiodevice_setInvalid (t_audiodevice *device)
{
    audiodevice_set (device, kAudioObjectUnknown);
}

PD_LOCAL int audiodevice_isValid (t_audiodevice *device)
{
    return (audiodevice_getIdentifier (device) != kAudioObjectUnknown);
}

PD_LOCAL int audiodevice_isInput (t_audiodevice *device)
{
    return (device->d_isInput  != 0);
}

PD_LOCAL int audiodevice_isOutput (t_audiodevice *device)
{
    return (device->d_isOutput != 0);
}

PD_LOCAL int audiodevice_isDuplex (t_audiodevice *device)
{
    return (device->d_isDuplex != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int audiodevice_getNumberOfChannelsProceed (t_audiodevice *device, int isInput)
{
    PD_ASSERT (device->d_ID != kAudioObjectUnknown);

    int result  = 0;
    UInt32 size = 0;
    
    AudioObjectPropertyScope   scope;
    AudioObjectPropertyAddress property;
    
    scope = isInput ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput;
    
    property.mSelector = kAudioDevicePropertyStreamConfiguration;
    property.mScope    = scope;
    property.mElement  = kAudioObjectPropertyElementMaster;

    if (!coreaudio_error (AudioObjectGetPropertyDataSize (device->d_ID, &property, 0, NULL, &size))) {
    //
    AudioBufferList *t = (AudioBufferList *)PD_MEMORY_GET (size);
    
    if (!coreaudio_error (AudioObjectGetPropertyData (device->d_ID, &property, 0, NULL, &size, t))) {
    //
    for (UInt32 i = 0; i < t->mNumberBuffers; i++) { result += t->mBuffers[i].mNumberChannels; }
    //
    }
    
    PD_MEMORY_FREE (t);
    //
    }

    return result;
}

PD_LOCAL int audiodevice_getNumberOfChannelsIn (t_audiodevice *device)
{
    return audiodevice_getNumberOfChannelsProceed (device, 1);
}

PD_LOCAL int audiodevice_getNumberOfChannelsOut (t_audiodevice *device)
{
    return audiodevice_getNumberOfChannelsProceed (device, 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if 0

static int audiodevice_getLatencyProceed (t_audiodevice *device, int isInput)
{
    PD_ASSERT (device->d_ID != kAudioObjectUnknown);
    
    UInt32 n = 0;
    
    AudioObjectPropertyScope   scope;
    AudioObjectPropertyAddress property;
    
    scope = isInput ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput;
    
    {
        UInt32 latency; UInt32 size = sizeof (latency);
    
        property.mSelector = kAudioDevicePropertyLatency;
        property.mScope    = scope;
        property.mElement  = kAudioObjectPropertyElementMaster;

        t_error err = coreaudio_error (AudioObjectGetPropertyData (device->d_ID,
                                            &property,
                                            0,
                                            NULL,
                                            &size,
                                            &latency));
        if (!err) { n += latency; }
    }
    {
        UInt32 safetyOffset; UInt32 size = sizeof (safetyOffset);
    
        property.mSelector = kAudioDevicePropertySafetyOffset;
        property.mScope    = scope;
        property.mElement  = kAudioObjectPropertyElementMaster;

        t_error err = coreaudio_error (AudioObjectGetPropertyData (device->d_ID,
                                            &property,
                                            0,
                                            NULL,
                                            &size,
                                            &safetyOffset));
        if (!err) { n += safetyOffset; }
    }
    
    return (int)n;
}

PD_LOCAL int audiodevice_getLatencyIn (t_audiodevice *device)
{
    return audiodevice_getLatencyProceed (device, 1);
}

PD_LOCAL int audiodevice_getLatencyOut (t_audiodevice *device)
{
    return audiodevice_getLatencyProceed (device, 0);
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int audiodevice_getSampleRate (t_audiodevice *device)
{
    PD_ASSERT (device->d_ID != kAudioObjectUnknown);
    
    t_float f = 0.0;
    
    AudioObjectPropertyAddress property;
    
    property.mSelector = kAudioDevicePropertyNominalSampleRate;
    property.mScope    = kAudioObjectPropertyScopeGlobal;
    property.mElement  = kAudioObjectPropertyElementMaster;

    Float64 sampleRate; UInt32 size = sizeof (sampleRate);

    t_error err = coreaudio_error (AudioObjectGetPropertyData (device->d_ID,
                                        &property,
                                        0,
                                        NULL,
                                        &size,
                                        &sampleRate));
    if (!err) { f = sampleRate; }
    
    return (int)f;
}

PD_LOCAL t_symbol *audiodevice_getName (t_audiodevice *device)
{
    PD_ASSERT (device->d_ID != kAudioObjectUnknown);

    t_symbol *name = NULL;
    
    AudioObjectPropertyAddress property;

    property.mSelector = kAudioObjectPropertyName;
    property.mScope    = kAudioObjectPropertyScopeGlobal;
    property.mElement  = kAudioObjectPropertyElementMaster;

    CFStringRef string = NULL; UInt32 size = sizeof (string);

    t_error err = coreaudio_error (AudioObjectGetPropertyData (device->d_ID,
                                        &property,
                                        0,
                                        NULL,
                                        &size,
                                        &string));

    if (!err) { name = core_CFStringToSymbol (string); }

    CFRelease (string);
    
    return name;
}

PD_LOCAL int audiodevice_getBufferSize (t_audiodevice *device)
{
    PD_ASSERT (device->d_ID != kAudioObjectUnknown);

    AudioObjectPropertyAddress property;
    
    property.mSelector  = kAudioDevicePropertyBufferFrameSize;
    property.mScope     = kAudioObjectPropertyScopeWildcard;
    property.mElement   = kAudioObjectPropertyElementMaster;

    UInt32 framesPerBuffer = 0;
    UInt32 size = sizeof (framesPerBuffer);
    
    t_error err = coreaudio_error (AudioObjectGetPropertyData (device->d_ID,
                                        &property,
                                        0,
                                        NULL,
                                        &size,
                                        &framesPerBuffer));
    
    PD_UNUSED (err);
    
    return (int)framesPerBuffer;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error audiodevice_setBufferSize (t_audiodevice *device, int size)
{
    PD_ASSERT (device->d_ID != kAudioObjectUnknown);
    
    AudioObjectPropertyAddress property;
    
    property.mSelector  = kAudioDevicePropertyBufferFrameSize;
    property.mScope     = kAudioObjectPropertyScopeGlobal;
    property.mElement   = kAudioObjectPropertyElementMaster;
    
    UInt32 framesPerBuffer = (UInt32)size;

    t_error err = coreaudio_error (AudioObjectSetPropertyData (device->d_ID,
                                        &property,
                                        0,
                                        NULL,
                                        sizeof (framesPerBuffer),
                                        &framesPerBuffer));
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
