
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "core_audio.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL OSStatus audiodevice_listenerCallback (AudioObjectID,
    UInt32,
    const AudioObjectPropertyAddress *,
    void *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL OSStatus audiodevice_inputCallback (void *,
    AudioUnitRenderActionFlags *,
    const AudioTimeStamp *,
    UInt32,
    UInt32,
    AudioBufferList *);

PD_LOCAL OSStatus audiodevice_outputCallback (void *,
    AudioUnitRenderActionFlags *,
    const AudioTimeStamp *,
    UInt32,
    UInt32,
    AudioBufferList *);

PD_LOCAL OSStatus audiodevice_duplexCallback (void *,
    AudioUnitRenderActionFlags *,
    const AudioTimeStamp *,
    UInt32,
    UInt32,
    AudioBufferList *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error audiodevice_addListeners (t_audiodevice *device, t_audiograph *graph)
{
    t_error err = PD_ERROR;
    
    if (device->d_ID != kAudioObjectUnknown) {
    if (!device->d_hasListener) {
    //
    AudioObjectPropertyAddress property;
    
    property.mSelector  = kAudioObjectPropertySelectorWildcard;
    property.mScope     = kAudioObjectPropertyScopeWildcard;
    property.mElement   = kAudioObjectPropertyElementWildcard;

    err = coreaudio_error (AudioObjectAddPropertyListener (device->d_ID,
                                &property,
                                audiodevice_listenerCallback,
                                graph));
    
    if (!err) { device->d_hasListener = 1; }
    //
    }
    }
    
    return err;
}

PD_LOCAL t_error audiodevice_removeListeners (t_audiodevice *device, t_audiograph *graph)
{
    t_error err = PD_ERROR_NONE;
    
    if (device->d_ID != kAudioObjectUnknown) {
    if (device->d_hasListener) {
    //
    AudioObjectPropertyAddress property;
    
    property.mSelector  = kAudioObjectPropertySelectorWildcard;
    property.mScope     = kAudioObjectPropertyScopeWildcard;
    property.mElement   = kAudioObjectPropertyElementWildcard;

    err = coreaudio_error (AudioObjectRemovePropertyListener (device->d_ID,
                                &property,
                                audiodevice_listenerCallback,
                                graph));
    //
    }
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static t_error audiodevice_openSetFormatProceed (t_audiodevice *device, t_audiograph *graph, int isInput)
{
    t_error err = PD_ERROR_NONE;
    
    AudioStreamBasicDescription deviceFormat;
   
    UInt32 size = sizeof (deviceFormat);

    err |= coreaudio_error (AudioUnitGetProperty (device->d_unit,
                                kAudioUnitProperty_StreamFormat,
                                isInput ? kAudioUnitScope_Input : kAudioUnitScope_Output,
                                isInput ? COREAUDIO_INPUT_ELEMENT : COREAUDIO_OUTPUT_ELEMENT,
                                &deviceFormat,
                                &size));

    /* Is that possible that the stream sample rate is not equal to device's nominal sample rate? */
    
    err |= (deviceFormat.mSampleRate != (Float64)graph->g_sampleRate);
    
    PD_ASSERT (!err);
    
    if (!err) {
    //
    AudioStreamBasicDescription graphFormat;
    
    graphFormat.mSampleRate       = deviceFormat.mSampleRate;
    graphFormat.mFormatID         = kAudioFormatLinearPCM;
    graphFormat.mFormatFlags      = kAudioFormatFlagsNativeFloatPacked | kLinearPCMFormatFlagIsNonInterleaved;
    graphFormat.mBytesPerPacket   = sizeof (t_sample);
    graphFormat.mFramesPerPacket  = 1;
    graphFormat.mBytesPerFrame    = sizeof (t_sample);
    graphFormat.mChannelsPerFrame = isInput ? graph->g_channelsIn : graph->g_channelsOut;
    graphFormat.mBitsPerChannel   = sizeof (t_sample) * 8;
    
    err |= coreaudio_error (AudioUnitSetProperty (device->d_unit,
                                kAudioUnitProperty_StreamFormat,
                                isInput ? kAudioUnitScope_Output : kAudioUnitScope_Input,
                                isInput ? COREAUDIO_INPUT_ELEMENT : COREAUDIO_OUTPUT_ELEMENT,
                                &graphFormat,
                                sizeof (graphFormat)));
    //
    }
    
    if (!err) {
    //
    UInt32 frames = graph->g_vectorSize;
    
    err = coreaudio_error (AudioUnitSetProperty (device->d_unit,
                                kAudioUnitProperty_MaximumFramesPerSlice,
                                isInput ? kAudioUnitScope_Output : kAudioUnitScope_Input,
                                isInput ? COREAUDIO_INPUT_ELEMENT : COREAUDIO_OUTPUT_ELEMENT,
                                &frames,
                                sizeof (frames)));
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error audiodevice_open (t_audiodevice *device, t_audiograph *graph)
{
    t_error err = (device->d_ID == kAudioObjectUnknown);

    if (!err) {
    //
    int isInput  = audiodevice_isInput (device);
    int isOutput = audiodevice_isOutput (device);
    int isDuplex = audiodevice_isDuplex (device);
    
    PD_ASSERT (isInput + isOutput + isDuplex == 1);
    
    AudioComponentDescription description;

    description.componentType         = kAudioUnitType_Output;
    description.componentSubType      = kAudioUnitSubType_HALOutput;
    description.componentManufacturer = kAudioUnitManufacturer_Apple;
    description.componentFlags        = 0;
    description.componentFlagsMask    = 0;

    AudioComponent component = AudioComponentFindNext (NULL, &description);

    err = (component == NULL);

    if (!err) { err = coreaudio_error (AudioComponentInstanceNew (component, &device->d_unit)); }
    if (!err) {
    //
    PD_ASSERT (device->d_unit != NULL);
    
    {
       UInt32 enableIO = isOutput ? 0 : 1;
       err |= coreaudio_error (AudioUnitSetProperty (device->d_unit,
                                    kAudioOutputUnitProperty_EnableIO,
                                    kAudioUnitScope_Input,
                                    COREAUDIO_INPUT_ELEMENT,
                                    &enableIO,
                                    sizeof (enableIO)));
    }
    {
        UInt32 enableIO = isInput ? 0 : 1;
        err |= coreaudio_error (AudioUnitSetProperty (device->d_unit,
                                    kAudioOutputUnitProperty_EnableIO,
                                    kAudioUnitScope_Output,
                                    COREAUDIO_OUTPUT_ELEMENT,
                                    &enableIO,
                                    sizeof (enableIO)));
    }
    {
        err |= coreaudio_error (AudioUnitSetProperty (device->d_unit,
                                    kAudioOutputUnitProperty_CurrentDevice,
                                    kAudioUnitScope_Global,
                                    0,
                                    &device->d_ID,
                                    sizeof (device->d_ID)));
    }
    {
        err |= audiodevice_setBufferSize (device, graph->g_vectorSize);
    }
    if (!isDuplex) { err |= audiodevice_openSetFormatProceed (device, graph, isInput); }
    else {
        err |= audiodevice_openSetFormatProceed (device, graph, 1);
        err |= audiodevice_openSetFormatProceed (device, graph, 0);
    }
    {
        AURenderCallbackStruct output; output.inputProcRefCon = graph;
    
        if (isInput) {
        //
        output.inputProc = audiodevice_inputCallback;

        err |= coreaudio_error (AudioUnitSetProperty (device->d_unit,
                                    kAudioOutputUnitProperty_SetInputCallback,
                                    kAudioUnitScope_Global,
                                    0,
                                    &output,
                                    sizeof (output)));
        //
        }
        if (isOutput) {
        //
        output.inputProc = audiodevice_outputCallback;

        err |= coreaudio_error (AudioUnitSetProperty (device->d_unit,
                                    kAudioUnitProperty_SetRenderCallback,
                                    kAudioUnitScope_Input,
                                    0,
                                    &output,
                                    sizeof (output)));
        //
        }
        if (isDuplex) {
        //
        output.inputProc = audiodevice_duplexCallback;

        err |= coreaudio_error (AudioUnitSetProperty (device->d_unit,
                                    kAudioUnitProperty_SetRenderCallback,
                                    kAudioUnitScope_Input,
                                    0,
                                    &output,
                                    sizeof (output)));
        //
        }
    }
    
    if (!err) { err |= coreaudio_error (AudioUnitInitialize (device->d_unit)); }
    if (!err) { device->d_isReady = 1; }
    //
    }
    //
    }
    
    return err;
}

PD_LOCAL t_error audiodevice_close (t_audiodevice *device, t_audiograph *graph)
{
    t_error err = PD_ERROR_NONE;
    
    if (device->d_unit) {
    //
    if (device->d_isReady) { err = coreaudio_error (AudioUnitUninitialize (device->d_unit)); }
    
    AudioComponentInstanceDispose (device->d_unit);
    //
    }
    
    device->d_unit    = NULL;
    device->d_isReady = 0;
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error audiodevice_start (t_audiodevice *device)
{
    t_error err = PD_ERROR;
    
    if (device->d_unit && device->d_isReady) {
    //
    err = coreaudio_error (AudioOutputUnitStart (device->d_unit));
    
    if (!err) { device->d_isRunning = 1; }
    //
    }
    
    return err;
}

PD_LOCAL t_error audiodevice_stop (t_audiodevice *device)
{
    t_error err = PD_ERROR;
    
    if (device->d_unit && device->d_isReady) {
    //
    if (device->d_isRunning) { err = coreaudio_error (AudioOutputUnitStop (device->d_unit)); }
    //
    }
    
    device->d_isRunning = 0;
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
