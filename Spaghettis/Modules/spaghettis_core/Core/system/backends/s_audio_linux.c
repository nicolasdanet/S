
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

double audio_getNanosecondsToSleep (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

extern t_sample *audio_soundIn;
extern t_sample *audio_soundOut;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static jack_client_t        *jack_client;                                       /* Static. */

static jack_port_t          *jack_portsIn[DEVICES_MAXIMUM_CHANNELS];            /* Static. */
static jack_port_t          *jack_portsOut[DEVICES_MAXIMUM_CHANNELS];           /* Static. */

static t_fifo32             *jack_ringIn[DEVICES_MAXIMUM_CHANNELS];             /* Static. */
static t_fifo32             *jack_ringOut[DEVICES_MAXIMUM_CHANNELS];            /* Static. */

static int                  jack_numberOfPortsIn;                               /* Static. */
static int                  jack_numberOfPortsOut;                              /* Static. */

static t_uint32Atomic       jack_bufferSize;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define JACK_GRAIN          5

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audio_vectorShrinkIn   (int);
void audio_vectorShrinkOut  (int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void jack_buffersFree (void)
{
    int i;

    for (i = 0; i < DEVICES_MAXIMUM_CHANNELS; i++) {
        if (jack_ringIn[i])  { fifo32_free (jack_ringIn[i]);  jack_ringIn[i]  = NULL; }
        if (jack_ringOut[i]) { fifo32_free (jack_ringOut[i]); jack_ringOut[i] = NULL; }
    }
}

static void jack_buffersAllocate (int numberOfChannelsIn, int numberOfChannelsOut)
{
    int i;

    jack_buffersFree();

    for (i = 0; i < numberOfChannelsIn;  i++) { jack_ringIn[i]  = fifo32_new(); }
    for (i = 0; i < numberOfChannelsOut; i++) { jack_ringOut[i] = fifo32_new(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Take ALL channels or NONE. */

static int jack_pollCallback (jack_nframes_t framesCount, void *dummy)
{
    int i;

    if (jack_numberOfPortsOut) {
    //
    int readable = 1;

    for (i = 0; i < jack_numberOfPortsOut; i++) {
    //
    if (fifo32_getAvailableRead (jack_ringOut[i]) < (int)framesCount) { readable = 0; break; }
    //
    }

    for (i = 0; i < jack_numberOfPortsOut; i++) {
    //
    void *t = jack_port_get_buffer (jack_portsOut[i], framesCount);

    if (readable) {
        fifo32_read (jack_ringOut[i], t, framesCount);
        
    } else {
        PD_LOG ("*@*");
        memset (t, 0, framesCount * sizeof (t_sample));   /* Fill with zeros. */
    }
    //
    }
    //
    }
    
    if (jack_numberOfPortsIn) {
    //
    int writable = 1;

    for (i = 0; i < jack_numberOfPortsIn; i++) {
    //
    if (fifo32_getAvailableWrite (jack_ringIn[i]) < (int)framesCount) { writable = 0; break; }
    //
    }

    for (i = 0; i < jack_numberOfPortsIn; i++) {
    //
    void *t = jack_port_get_buffer (jack_portsIn[i], framesCount);

    if (writable) { fifo32_write (jack_ringIn[i], t, framesCount); }    /* Simply drop if full. */
    //
    }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int jack_bufferSizeCallback (jack_nframes_t n, void *dummy)
{
    atomic_uInt32Write (&jack_bufferSize, n);
    
    return 0;
}

static int jack_sampleRateCallback (jack_nframes_t n, void *dummy)
{
    return 0;
}

static void jack_shutdownCallback (void *dummy)
{
    jack_client = NULL; instance_audioCloseWithError();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char *audio_getBackendNameNative (void)
{
    static const char *name = "JACK"; return name;      /* Static. */
}

t_error audio_initializeNative (void)
{
    return PD_ERROR_NONE;
}

void audio_releaseNative (void)
{
    jack_buffersFree();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_openNative (t_devices *p)
{
    int numberOfChannelsIn  = devices_getInChannels (p, 0);
    int numberOfChannelsOut = devices_getOutChannels (p, 0);
    int sampleRate          = AUDIO_DEFAULT_SAMPLERATE;
        
    static_assert (sizeof (t_sample) == sizeof (jack_default_audio_sample_t));
    
    if (numberOfChannelsIn)  {  /* For now audio in is required to synchronize properly the callback. */
    if (numberOfChannelsOut) {
    //
    jack_status_t status;
    
    numberOfChannelsIn  = PD_MIN (numberOfChannelsIn, DEVICES_MAXIMUM_CHANNELS);
    numberOfChannelsOut = PD_MIN (numberOfChannelsOut, DEVICES_MAXIMUM_CHANNELS);
 
    PD_ASSERT (!jack_client);

    jack_client = jack_client_open (PD_NAME_LOWERCASE, JackNoStartServer, &status, NULL);
    
    if (jack_client) {
    //
    if (jack_get_sample_rate (jack_client) != (jack_nframes_t)sampleRate) {
        jack_client_close (jack_client);
        jack_client = NULL;
        error_invalid (NULL, sym_audio, sym_samplerate);
    }
    //
    }

    if (jack_client) {
    //
    int i;
    
    jack_buffersAllocate (numberOfChannelsIn, numberOfChannelsOut);

    jack_set_process_callback (jack_client, jack_pollCallback, NULL);
    jack_set_buffer_size_callback (jack_client, jack_bufferSizeCallback, NULL);
    jack_set_sample_rate_callback (jack_client, jack_sampleRateCallback, NULL);
    jack_on_shutdown (jack_client, jack_shutdownCallback, NULL);

    for (i = 0; i < numberOfChannelsIn; i++) {
    //
    char t[PD_STRING] = { 0 };
    string_sprintf (t, PD_STRING, "input_%d", i + 1);
    jack_portsIn[i] = jack_port_register (jack_client, t, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    if (jack_portsIn[i]) { }
    else {
        error_failed (NULL, sym_JACK);
        break;
    }
    //
    }

    audio_vectorShrinkIn (jack_numberOfPortsIn = numberOfChannelsIn = i);
    
    for (i = 0; i < numberOfChannelsOut; i++) {
    //
    char t[PD_STRING] = { 0 };
    string_sprintf (t, PD_STRING, "output_%d", i + 1);
    jack_portsOut[i] = jack_port_register (jack_client, t, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    if (jack_portsOut[i]) { }
    else {
        error_failed (NULL, sym_JACK);
        break;  
    }
    //
    }
    
    audio_vectorShrinkOut (jack_numberOfPortsOut = numberOfChannelsOut = i);
    
    if (!jack_activate (jack_client)) { return PD_ERROR_NONE; }
    //
    }
    //
    }
    }
        
    return PD_ERROR;
}

void audio_closeNative (void) 
{
    if (jack_client) {
    //
    int i;
    jack_deactivate (jack_client);
    for (i = 0; i < jack_numberOfPortsIn; i++)  {
        jack_port_unregister (jack_client, jack_portsIn[i]);
        jack_portsIn[i] = NULL;
    }
    for (i = 0; i < jack_numberOfPortsOut; i++) {
        jack_port_unregister (jack_client, jack_portsOut[i]);
        jack_portsOut[i] = NULL;
    }
    jack_client_close (jack_client);
    jack_client = NULL;
    //
    }

    jack_buffersFree();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int audio_pollNative (void)
{
    int i;
    int status = DACS_YES;
    t_sample *sound = NULL;

    if (!jack_client || (!jack_numberOfPortsIn && !jack_numberOfPortsOut)) { return DACS_NO; }
    else {
    //
    int needToWait = 0; double ns = audio_getNanosecondsToSleep() / (double)JACK_GRAIN;
    
    if (jack_numberOfPortsIn) {
    //
    for (i = 0; i < jack_numberOfPortsIn; i++) {
        while (fifo32_getAvailableRead (jack_ringIn[i]) < INTERNAL_BLOCKSIZE) {
            status = DACS_SLEPT;
            if (needToWait < JACK_GRAIN * 2) {
                PD_LOG (".");
                nano_sleep (ns);
            } else { return DACS_NO; }
            needToWait++;
        }
    }
    //
    }
    
    if (jack_numberOfPortsOut) {
    //
    for (i = 0; i < jack_numberOfPortsOut; i++) {
        while (fifo32_getAvailableWrite (jack_ringOut[i]) < INTERNAL_BLOCKSIZE) {
            status = DACS_SLEPT;
            if (needToWait < JACK_GRAIN * 2) {
                PD_LOG (":");
                nano_sleep (ns);
            } else { return DACS_NO; }
            needToWait++;
        }
    }
    //
    }

    if (jack_numberOfPortsIn) {
    //
    sound = audio_soundIn;
        
    for (i = 0; i < jack_numberOfPortsIn; i++) {
        fifo32_read (jack_ringIn[i], (void *)sound, INTERNAL_BLOCKSIZE);
        sound += INTERNAL_BLOCKSIZE;
    }
    //
    }

    if (jack_numberOfPortsOut) {
    //  
    sound = audio_soundOut;
        
    for (i = 0; i < jack_numberOfPortsOut; i++) {
        audio_safe (sound, INTERNAL_BLOCKSIZE);
        fifo32_write (jack_ringOut[i], (const void *)sound, INTERNAL_BLOCKSIZE);
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
    return atomic_uInt32Read (&jack_bufferSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_getListOfDevicesNative (t_audiodevices *p) 
{
    t_error err = PD_ERROR_NONE;
    
    err |= audiodevices_addAudioIn (p,  gensym ("JACK ports"), 2);
    err |= audiodevices_addAudioOut (p, gensym ("JACK ports"), 2);

    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
