
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../libs/core/core_audio.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double audio_getNanosecondsToSleep (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define CORE_GRAIN      5

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_sample *audio_soundIn;
extern t_sample *audio_soundOut;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_audiograph     core_graph;                                         /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_fifo32         *core_ringIn[DEVICES_MAXIMUM_CHANNELS];             /* Static. */
static t_fifo32         *core_ringOut[DEVICES_MAXIMUM_CHANNELS];            /* Static. */

static int              core_channelsIn;                                    /* Static. */
static int              core_channelsOut;                                   /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_uint32Atomic   core_vectorSize;                                    /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void core_buffersFree (void)
{
    int i;

    for (i = 0; i < DEVICES_MAXIMUM_CHANNELS; i++) {
        if (core_ringIn[i])  { fifo32_free (core_ringIn[i]);  core_ringIn[i]  = NULL; }
        if (core_ringOut[i]) { fifo32_free (core_ringOut[i]); core_ringOut[i] = NULL; }
    }
}

static void core_buffersAllocate (int numberOfChannelsIn, int numberOfChannelsOut)
{
    int i;

    core_buffersFree();

    for (i = 0; i < numberOfChannelsIn;  i++) { core_ringIn[i]  = fifo32_new(); }
    for (i = 0; i < numberOfChannelsOut; i++) { core_ringOut[i] = fifo32_new(); }
    
    core_channelsIn  = numberOfChannelsIn;
    core_channelsOut = numberOfChannelsOut;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Take ALL channels or NONE. */

static int core_buffersWritable (int channels, UInt32 inNumberFrames)
{
    if (channels != core_channelsIn) { return 0; }
    else {
    //
    int i, writable = 1;
    
    for (i = 0; i < channels; i++) {
    //
    if (fifo32_getAvailableWrite (core_ringIn[i]) < (int)inNumberFrames) { writable = 0; break; }
    //
    }
    
    return writable;
    //
    }
}

t_error core_buffersPush (UInt32 inNumberFrames, AudioBufferList *ioData, t_audiograph *graph)
{
    int i, channels = ioData->mNumberBuffers;
    
    if (core_buffersWritable (channels, inNumberFrames)) {
    //
    for (i = 0; i < channels; i++) {
    //
    void *t = ioData->mBuffers[i].mData; fifo32_write (core_ringIn[i], t, inNumberFrames);
    //
    }
    
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

/* Take ALL channels or NONE. */

static int core_buffersReadable (int channels, UInt32 inNumberFrames)
{
    if (channels != core_channelsOut) { return 0; }
    else {
    //
    int i, readable = 1;
    
    for (i = 0; i < channels; i++) {
    //
    if (fifo32_getAvailableRead (core_ringOut[i]) < (int32_t)inNumberFrames) { readable = 0; break; }
    //
    }
    
    return readable;
    //
    }
}

t_error core_buffersPull (UInt32 inNumberFrames, AudioBufferList *ioData, t_audiograph *graph)
{
    int i, channels = ioData->mNumberBuffers;
    
    if (core_buffersReadable (channels, inNumberFrames)) {
    //
    if (atomic_int32Read (&graph->g_blank)) { atomic_int32Decrement (&graph->g_blank); }
    else {
    //
    for (i = 0; i < channels; i++) {
    //
    void *t = ioData->mBuffers[i].mData; fifo32_read (core_ringOut[i], t, inNumberFrames);
    //
    }
    
    return PD_ERROR_NONE;
    //
    }
    //
    }
    
    PD_LOG ("*@*"); return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void core_setVectorSize (UInt32 n)
{
    atomic_uInt32Write (&core_vectorSize, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char *audio_nameNative (void)
{
    static const char *name = "Core Audio"; return name;    /* Static. */
}

t_error audio_initializeNative (void)
{
    return audiograph_initialize (&core_graph);
}

void audio_releaseNative (void)
{
    audiograph_release (&core_graph); core_buffersFree();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_openNative (t_devices *p)
{
    t_error err = PD_ERROR;
    
    t_symbol *deviceIn  = devices_getInSize (p)  ? devices_getInAtIndexAsSymbol (p, 0)  : NULL;
    t_symbol *deviceOut = devices_getOutSize (p) ? devices_getOutAtIndexAsSymbol (p, 0) : NULL;
    int channelsIn      = devices_getInSize (p)  ? devices_getInChannelsAtIndex (p, 0)  : 0;
    int channelsOut     = devices_getOutSize (p) ? devices_getOutChannelsAtIndex (p, 0) : 0;
    int sampleRate      = devices_getSampleRate (p);
    int vectorSize      = devices_getVectorSize (p);
    
    static_assert (sizeof (t_sample) == sizeof (Float32), "");
    
    /* For now audio in is required to synchronize properly the callback. */
    
    if (deviceIn && channelsIn) {
    //
    core_buffersAllocate (channelsIn, channelsOut);
    
    err = audiograph_open (&core_graph, deviceIn, deviceOut, channelsIn, channelsOut, sampleRate, vectorSize);
    
    if (err) { audiograph_close (&core_graph); }
    //
    }
    
    return err;
}

void audio_closeNative() 
{
    audiograph_close (&core_graph); core_buffersFree();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int audio_pollNative() 
{
    int i;
    int status = DACS_YES;
    t_sample *sound = NULL;

    if (!core_channelsIn && !core_channelsOut) { return DACS_NO; }
    else {
    //
    int needToWait = 0; double ns = audio_getNanosecondsToSleep() / (double)CORE_GRAIN;
    
    if (core_channelsIn) {
    //
    for (i = 0; i < core_channelsIn; i++) {
        while (fifo32_getAvailableRead (core_ringIn[i]) < INTERNAL_BLOCKSIZE) {
            status = DACS_SLEPT;
            if (needToWait < CORE_GRAIN * 2) {
                PD_LOG (".");
                nano_sleep (ns);
            } else { return DACS_NO; }
            needToWait++;
        }
    }
    //
    }
    
    if (core_channelsOut) {
    //
    for (i = 0; i < core_channelsOut; i++) {
        while (fifo32_getAvailableWrite (core_ringOut[i]) < INTERNAL_BLOCKSIZE) {
            status = DACS_SLEPT;
            if (needToWait < CORE_GRAIN * 2) {
                PD_LOG (":");
                nano_sleep (ns);
            } else { return DACS_NO; }
            needToWait++;
        }
    }
    //
    }
    
    if (core_channelsIn) {
    //
    sound = audio_soundIn;
        
    for (i = 0; i < core_channelsIn; i++) {
        fifo32_read (core_ringIn[i], (void *)sound, INTERNAL_BLOCKSIZE);
        sound += INTERNAL_BLOCKSIZE;
    }
    //
    }

    if (core_channelsOut) {
    //
    sound = audio_soundOut;
        
    for (i = 0; i < core_channelsOut; i++) {
        audio_safe (sound, INTERNAL_BLOCKSIZE, 1);
        fifo32_write (core_ringOut[i], (const void *)sound, INTERNAL_BLOCKSIZE);
        memset ((void *)sound, 0, INTERNAL_BLOCKSIZE * sizeof (t_sample));                  /* Zeroed. */
        sound += INTERNAL_BLOCKSIZE;
    }
    //
    }
    //
    }
    
    return status;
}

int audio_getVectorSizeNative (void)
{
    return atomic_uInt32Read (&core_vectorSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_getListsNative (t_deviceslist *p) 
{
    return audiodevicelist_get (p);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

