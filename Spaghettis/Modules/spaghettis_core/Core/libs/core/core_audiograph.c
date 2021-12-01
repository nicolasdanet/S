
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "core_audio.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void audio_vectorShrinkIn  (int);
PD_LOCAL void audio_vectorShrinkOut (int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void audiograph_buffersAllocate (t_audiograph *graph)
{
    PD_ASSERT (graph->g_data == NULL);

    int size = sizeof (UInt32) + (graph->g_channelsIn * sizeof (AudioBuffer));
    
    AudioBufferList *t = graph->g_data = (AudioBufferList *)PD_MEMORY_GET (size);
    
    UInt32 i, k = COREAUDIO_BLOCKSIZE_MAXIMUM * sizeof (Float32);

    t->mNumberBuffers = graph->g_channelsIn;

    for (i = 0; i < t->mNumberBuffers; i++) {
        t->mBuffers[i].mNumberChannels = 1;
        t->mBuffers[i].mDataByteSize   = k;
        t->mBuffers[i].mData           = PD_MEMORY_GET (k);
    }
}

static void audiograph_buffersFree (t_audiograph *graph)
{
    AudioBufferList *t = graph->g_data;
    
    if (t) {
    //
    UInt32 i;

    for (i = 0; i < t->mNumberBuffers; i++) { PD_MEMORY_FREE (t->mBuffers[i].mData); }
    
    PD_MEMORY_FREE (t);
    //
    }
    
    graph->g_data = NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void audiograph_reset (t_audiograph *graph)
{
    audiodevice_setInvalid (&graph->g_deviceIn);
    audiodevice_setInvalid (&graph->g_deviceOut);
    audiodevice_setInvalid (&graph->g_deviceDuplex);
    
    graph->g_sampleRate  = 0;
    graph->g_channelsIn  = 0;
    graph->g_channelsOut = 0;
    graph->g_hasInput    = 0;
    graph->g_hasOutput   = 0;
    graph->g_hasDuplex   = 0;
    
    PD_ASSERT (graph->g_data == NULL);

    PD_ATOMIC_INT32_WRITE (0, &graph->g_isRunning);
    PD_ATOMIC_INT32_WRITE (0, &graph->g_flag);
    PD_ATOMIC_INT32_WRITE (0, &graph->g_blank);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Should it be called asynchronously? */

PD_LOCAL t_error audiograph_check (t_audiograph *graph, int close)
{
    /* Avoid to check while setting devices. */
    
    if (close && !PD_ATOMIC_INT32_READ (&graph->g_isRunning)) { return PD_ERROR_NONE; }
    
    {
        t_error err = PD_ERROR_NONE;
        int t = graph->g_sampleRate;
    
        err |= (graph->g_hasInput  && audiodevice_getSampleRate (&graph->g_deviceIn)     != t);
        err |= (graph->g_hasOutput && audiodevice_getSampleRate (&graph->g_deviceOut)    != t);
        err |= (graph->g_hasDuplex && audiodevice_getSampleRate (&graph->g_deviceDuplex) != t);
    
        if (err) {
            if (close) { instance_audioCloseWithError(); }
            return PD_ERROR;
        }
    }
    {
        t_error err = PD_ERROR_NONE;
        int t = graph->g_vectorSize;
    
        err |= (graph->g_hasInput  && audiodevice_getBufferSize (&graph->g_deviceIn)     != t);
        err |= (graph->g_hasOutput && audiodevice_getBufferSize (&graph->g_deviceOut)    != t);
        err |= (graph->g_hasDuplex && audiodevice_getBufferSize (&graph->g_deviceDuplex) != t);
    
        if (err) {
            if (close) { instance_audioCloseWithError(); }
            return PD_ERROR;
        }
    }
    
    return PD_ERROR_NONE;
}

static void audiograph_invalid (int sampleRate, int i, int o)
{
    post_error (NULL, PD_TRANSLATE ("audio: invalid sampling rate"));
    
    if (sampleRate != i) { post_error (NULL, PD_TRANSLATE ("audio: invalid input / %d Hz"), i);  }
    if (sampleRate != o) { post_error (NULL, PD_TRANSLATE ("audio: invalid output / %d Hz"), o); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Input device is NOT equal to output device. */

static t_error audiograph_openSimple (t_audiograph *graph,
    AudioObjectID objectIn,
    AudioObjectID objectOut,
    int channelsIn,
    int channelsOut,
    int sampleRate,
    int vectorSize)
{
    t_error err = PD_ERROR_NONE;
    
    audiodevice_setAsInput (&graph->g_deviceIn,   objectIn);
    audiodevice_setAsOutput (&graph->g_deviceOut, objectOut);
    
    int hasInput  = (channelsIn > 0  && audiodevice_isValid (&graph->g_deviceIn));
    int hasOutput = (channelsOut > 0 && audiodevice_isValid (&graph->g_deviceOut));
    
    /* Note that the maximum number of channels should have been already tested. */
    
    PD_ASSERT (!hasInput  || audiodevice_getNumberOfChannelsIn (&graph->g_deviceIn)   >= channelsIn);
    PD_ASSERT (!hasOutput || audiodevice_getNumberOfChannelsOut (&graph->g_deviceOut) >= channelsOut);
    
    /* Input and output required to synchronize properly the callback. */
    
    if (!hasInput || !hasOutput) { return PD_ERROR; }
    else {
    //
    int t0 = audiodevice_getSampleRate (&graph->g_deviceIn);
    int t1 = audiodevice_getSampleRate (&graph->g_deviceOut);
    
    err |= (t0 != sampleRate);
    err |= (t1 != sampleRate);
    
    if (err) { audiograph_invalid (sampleRate, t0, t1); }
    else {
    //
    audio_vectorShrinkIn (channelsIn);
    audio_vectorShrinkOut (channelsOut);
    
    graph->g_sampleRate  = sampleRate;
    graph->g_vectorSize  = vectorSize;
    graph->g_channelsIn  = channelsIn;
    graph->g_channelsOut = channelsOut;
    graph->g_hasInput    = hasInput;
    graph->g_hasOutput   = hasOutput;
    
    audiograph_buffersAllocate (graph);
    
    err |= audiodevice_addListeners (&graph->g_deviceIn, graph);
    err |= audiodevice_addListeners (&graph->g_deviceOut, graph);
    err |= audiodevice_open (&graph->g_deviceIn, graph);
    err |= audiodevice_open (&graph->g_deviceOut, graph);
    
    if (!err) { err = audiograph_check (graph, 0); }
    if (!err) {
    //
    PD_ATOMIC_INT32_INCREMENT (&graph->g_blank);                /* Add one extra vector of latency. */
    
    err |= audiodevice_start (&graph->g_deviceIn);
    err |= audiodevice_start (&graph->g_deviceOut);
    
    if (!err) { PD_ATOMIC_INT32_WRITE (1, &graph->g_isRunning); }
    //
    }
    //
    }
    //
    }
    
    return err;
}

/* Input and outut device is the same. */

static t_error audiograph_openDuplex (t_audiograph *graph,
    AudioObjectID objectIn,
    AudioObjectID objectOut,
    int channelsIn,
    int channelsOut,
    int sampleRate,
    int vectorSize)
{
    t_error err = PD_ERROR_NONE;
    
    PD_ASSERT (objectIn == objectOut);
    
    audiodevice_setAsDuplex (&graph->g_deviceDuplex, objectIn);
    
    int hasIO = (channelsIn > 0 && channelsOut > 0 && audiodevice_isValid (&graph->g_deviceDuplex));

    /* Note that the maximum number of channels should have been already tested. */
    
    PD_ASSERT (!hasIO || audiodevice_getNumberOfChannelsIn (&graph->g_deviceDuplex)  >= channelsIn);
    PD_ASSERT (!hasIO || audiodevice_getNumberOfChannelsOut (&graph->g_deviceDuplex) >= channelsOut);
    
    /* Input and output required to synchronize properly the callback. */
    
    if (!hasIO) { return PD_ERROR; }
    else {
    //
    int t = audiodevice_getSampleRate (&graph->g_deviceDuplex);
    
    err |= (t != sampleRate);
    
    if (err) { audiograph_invalid (sampleRate, t, t); }
    else {
    //
    audio_vectorShrinkIn (channelsIn);
    audio_vectorShrinkOut (channelsOut);
    
    graph->g_sampleRate  = sampleRate;
    graph->g_vectorSize  = vectorSize;
    graph->g_channelsIn  = channelsIn;
    graph->g_channelsOut = channelsOut;
    graph->g_hasDuplex   = 1;
    
    audiograph_buffersAllocate (graph);
    
    err |= audiodevice_addListeners (&graph->g_deviceDuplex, graph);
    err |= audiodevice_open (&graph->g_deviceDuplex, graph);
    
    if (!err) { err = audiograph_check (graph, 0); }
    if (!err) {
    //
    err |= audiodevice_start (&graph->g_deviceDuplex);
    
    if (!err) { PD_ATOMIC_INT32_WRITE (1, &graph->g_isRunning); }
    //
    }
    //
    }
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error audiograph_open (t_audiograph *graph,
    t_symbol *deviceIn,
    t_symbol *deviceOut,
    int channelsIn,
    int channelsOut,
    int sampleRate,
    int vectorSize)
{
    t_error err = PD_ERROR;
    
    audiograph_reset (graph);
    
    if (vectorSize > 0) {
    if (vectorSize <= COREAUDIO_BLOCKSIZE_MAXIMUM) {
    if (PD_IS_POWER_2 (vectorSize)) {
    //
    AudioObjectID objectIn  = audiodevicelist_fetch (deviceIn);
    AudioObjectID objectOut = audiodevicelist_fetch (deviceOut);
    
    if (objectIn != kAudioObjectUnknown && objectIn == objectOut) {
    err = audiograph_openDuplex (graph, objectIn, objectOut, channelsIn, channelsOut, sampleRate, vectorSize);
    } else {
    err = audiograph_openSimple (graph, objectIn, objectOut, channelsIn, channelsOut, sampleRate, vectorSize);
    }
    //
    }
    }
    }
    
    return err;
}

PD_LOCAL void audiograph_close (t_audiograph *graph)
{
    PD_ATOMIC_INT32_WRITE (0, &graph->g_isRunning);
    
    audiodevice_stop (&graph->g_deviceIn);
    audiodevice_stop (&graph->g_deviceOut);
    audiodevice_stop (&graph->g_deviceDuplex);
    
    audiodevice_close (&graph->g_deviceIn,     graph);
    audiodevice_close (&graph->g_deviceOut,    graph);
    audiodevice_close (&graph->g_deviceDuplex, graph);
    
    audiodevice_removeListeners (&graph->g_deviceIn,     graph);
    audiodevice_removeListeners (&graph->g_deviceOut,    graph);
    audiodevice_removeListeners (&graph->g_deviceDuplex, graph);
    
    audiodevice_setInvalid (&graph->g_deviceIn);
    audiodevice_setInvalid (&graph->g_deviceOut);
    audiodevice_setInvalid (&graph->g_deviceDuplex);
    
    audiograph_buffersFree (graph);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error audiograph_initialize (t_audiograph *graph)
{
    audiograph_reset (graph);
    
    CFRunLoopRef runLoop = NULL;
    
    AudioObjectPropertyAddress property;
    
    property.mSelector = kAudioHardwarePropertyRunLoop;
    property.mScope    = kAudioObjectPropertyScopeGlobal;
    property.mElement  = kAudioObjectPropertyElementMaster;
    
    /* Required to force AudioUnit listeners to be called by a new run loop. */
    
    t_error err = coreaudio_error (AudioObjectSetPropertyData (kAudioObjectSystemObject,
                                        &property,
                                        0,
                                        NULL,
                                        sizeof (CFRunLoopRef),
                                        &runLoop));
    
    return err;
}

PD_LOCAL void audiograph_release (t_audiograph *graph)
{
    PD_ASSERT (!audiodevice_isValid (&graph->g_deviceIn));
    PD_ASSERT (!audiodevice_isValid (&graph->g_deviceOut));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
