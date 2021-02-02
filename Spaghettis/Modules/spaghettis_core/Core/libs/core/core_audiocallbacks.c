
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "core_audio.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void    core_setVectorSize  (UInt32);
PD_LOCAL t_error core_buffersPush    (UInt32, AudioBufferList *, t_audiograph *);
PD_LOCAL t_error core_buffersPull    (UInt32, AudioBufferList *, t_audiograph *);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline void audiodevice_bufferListZeroed (AudioBufferList *ioData)
{
    UInt32 i;
    
    for (i = 0; i < ioData->mNumberBuffers; i++) {
        memset (ioData->mBuffers[i].mData, 0, ioData->mBuffers[i].mDataByteSize);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL OSStatus audiodevice_listenerCallback (AudioObjectID inObjectID,
    UInt32 inNumberAddresses,
    const AudioObjectPropertyAddress *inAddresses,
    void *inClientData)
{
    t_audiograph *graph = (t_audiograph *)inClientData;
    
    UInt32 i;
    
    int check = 0;
    
    for (i = 0; i < inNumberAddresses; i++) {
    //
    const AudioObjectPropertyAddress *t = inAddresses + i;
    
    switch (t->mSelector) {
    //
    case kAudioDeviceProcessorOverload           : break;
    // case kAudioDevicePropertyBufferSizeRange  : break;
    // case kAudioDevicePropertyBufferSize       : break;
    case kAudioDevicePropertyBufferFrameSize     : check = 1; break;
    // case kAudioDevicePropertyStreamFormat     : break;
    case kAudioDevicePropertyStreamConfiguration : break;
    case kAudioDevicePropertyDeviceIsAlive       : break;
    case kAudioStreamPropertyPhysicalFormat      : break;
    case kAudioDevicePropertyDeviceHasChanged    : break;
    case kAudioObjectPropertyOwnedObjects        : break;
    case kAudioDevicePropertyVolumeScalar        : break;
    case kAudioDevicePropertyMute                : break;
    case kAudioDevicePropertyPlayThru            : break;
    case kAudioDevicePropertyDataSource          : break;
    case kAudioDevicePropertyDeviceIsRunning     : break;
    case kAudioDevicePropertyNominalSampleRate   : check = 1; break;
    default                                      : break;
    //
    }
    //
    }
    
    if (check) { audiograph_check (graph, 1); }
    
    return noErr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL OSStatus audiodevice_inputCallback (void *inRefCon,
    AudioUnitRenderActionFlags *ioActionFlags,
    const AudioTimeStamp *inTimeStamp,
    UInt32 inBusNumber,
    UInt32 inNumberFrames,
    AudioBufferList *ioData)
{
    t_audiograph *graph = (t_audiograph *)inRefCon;
    
    if (inNumberFrames <= COREAUDIO_BLOCKSIZE_MAXIMUM) {    /* Should always be true. */
    //
    core_setVectorSize (inNumberFrames);

    if (PD_ATOMIC_INT32_READ (&graph->g_flag)) {            /* Output stream must always be started first. */
    //
    t_error err = (AudioUnitRender (audiodevice_getAudioUnit (&graph->g_deviceIn),
                        ioActionFlags,
                        inTimeStamp,
                        COREAUDIO_INPUT_ELEMENT,
                        inNumberFrames,
                        graph->g_data) != noErr);
    
    if (err) { audiodevice_bufferListZeroed (graph->g_data); }
    
    core_buffersPush (inNumberFrames, graph->g_data, graph);
    //
    }
    //
    }
    
    return noErr;
}

PD_LOCAL OSStatus audiodevice_outputCallback (void *inRefCon,
    AudioUnitRenderActionFlags *ioActionFlags,
    const AudioTimeStamp *inTimeStamp,
    UInt32 inBusNumber,
    UInt32 inNumberFrames,
    AudioBufferList *ioData)
{
    t_audiograph *graph = (t_audiograph *)inRefCon;
    
    PD_ATOMIC_INT32_WRITE (1, &graph->g_flag);
    
    if (core_buffersPull (inNumberFrames, ioData, graph)) { audiodevice_bufferListZeroed (ioData); }
    
    return noErr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL OSStatus audiodevice_duplexCallback (void *inRefCon,
    AudioUnitRenderActionFlags *ioActionFlags,
    const AudioTimeStamp *inTimeStamp,
    UInt32 inBusNumber,
    UInt32 inNumberFrames,
    AudioBufferList *ioData)
{
    t_audiograph *graph = (t_audiograph *)inRefCon;
    
    if (core_buffersPull (inNumberFrames, ioData, graph)) { audiodevice_bufferListZeroed (ioData); }
    
    if (inNumberFrames <= COREAUDIO_BLOCKSIZE_MAXIMUM) {    /* Should always be true. */
    //
    core_setVectorSize (inNumberFrames);

    t_error err = (AudioUnitRender (audiodevice_getAudioUnit (&graph->g_deviceDuplex),
                        ioActionFlags,
                        inTimeStamp,
                        COREAUDIO_INPUT_ELEMENT,
                        inNumberFrames,
                        graph->g_data) != noErr);
    
    if (err) { audiodevice_bufferListZeroed (graph->g_data); }
    
    core_buffersPush (inNumberFrames, graph->g_data, graph);
    //
    }
    
    return noErr;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
