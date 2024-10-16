
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_sample *audio_soundIn;                            /* Static. */
t_sample *audio_soundOut;                           /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int              audio_totalOfChannelsIn;    /* Static. */
static int              audio_totalOfChannelsOut;   /* Static. */
static t_float64Atomic  audio_sampleRate;           /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int              audio_state;                /* Static. */
pthread_mutex_t         audio_mutex;                /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void audio_vectorInitialize (t_float, int, int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int audio_isOpened (void)
{
    return (audio_state != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Check maximum device channels (if defined) before to open stream. */

static t_error audio_check (t_devices *p)
{
    t_error err = PD_ERROR_NONE;
    
    // t_audiodevices l; t_error err = audio_getDevicesList (&l, 0);
    
    /* ??? */
    
    /*
    devices_check (p);

    if (!err) {
    //
    int i;
    
    for (i = 0; i < devices_getInSize (p); i++) {
    //
    int m = devices_getInChannelsAtIndex (p, i);
    int n = deviceslist_getInChannelsAtIndex (&l, devices_getInAtIndex (p, i));
    if (n > 0 && m > n) {
        err = PD_ERROR; break;
    }
    //
    }
    //
    }
    
    if (!err) {
    //
    int i;
    
    for (i = 0; i < devices_getOutSize (p); i++) {
    //
    int m = devices_getOutChannelsAtIndex (p, i);
    int n = deviceslist_getOutChannelsAtIndex (&l, devices_getOutAtIndex (p, i));
    if (n > 0 && m > n) {
        err = PD_ERROR; break;
    }
    //
    }
    //
    }
    */
    
    if (err) { error_mismatch (NULL, sym_audio, sym_channels); }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_open (void)
{
    t_error err = PD_ERROR;
    
    t_devices audio;
    
    // audio_getDevices (&audio);
    
    /* ??? */
    
    if (audio_check (&audio) == PD_ERROR_NONE) {
    //
    /*
    pthread_mutex_lock (&audio_mutex);
    
        if (devices_getInSize (&audio) || devices_getOutSize (&audio)) {
            int m = audio_getTotalOfChannelsIn();
            int n = audio_getTotalOfChannelsOut();
            audio_vectorInitialize (AUDIO_DEFAULT_SAMPLERATE, m, n);
            err = audio_openNative (&audio);
        }

        audio_state = err ? 0 : 1;
    
    pthread_mutex_unlock (&audio_mutex);
    */
    //
    }
    
    return err;
}

void audio_close (void)
{
    pthread_mutex_lock (&audio_mutex);
    
    if (audio_isOpened()) { audio_closeNative(); } audio_state = 0;
    
    pthread_mutex_unlock (&audio_mutex);
}

int audio_poll (void)
{
    int k = DACS_NO;
    
    if (pthread_mutex_trylock (&audio_mutex) == 0) {
    //
    if (audio_isOpened()) { k = audio_pollNative(); }
    
    pthread_mutex_unlock (&audio_mutex);
    //
    }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_stop (void)
{
    if (audio_isOpened()) { audio_close(); } return PD_ERROR_NONE;
}

t_error audio_start (void)
{
    if (!audio_isOpened()) { return audio_open(); } else { return PD_ERROR_NONE; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

double audio_getNanosecondsToSleep (void)
{
    double t = INTERNAL_BLOCKSIZE / audio_getSampleRate();
    
    return PD_SECONDS_TO_NANOSECONDS (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_float audio_getSampleRate (void)
{
    t_float f = atomic_float64Read (&audio_sampleRate); return (f <= 0.0 ? AUDIO_DEFAULT_SAMPLERATE : f);
}

int audio_getVectorSize (void)
{
    return audio_getVectorSizeNative();
}

int audio_getTotalOfChannelsIn (void) 
{
    return audio_totalOfChannelsIn;
}

int audio_getTotalOfChannelsOut (void)
{
    return audio_totalOfChannelsOut; 
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void audio_vectorInitialize (t_float sampleRate, int totalOfChannelsIn, int totalOfChannelsOut)
{
    int m = (int)((INTERNAL_BLOCKSIZE * sizeof (t_sample)) * (totalOfChannelsIn ? totalOfChannelsIn : 2));
    int n = (int)((INTERNAL_BLOCKSIZE * sizeof (t_sample)) * (totalOfChannelsOut ? totalOfChannelsOut : 2));

    PD_ASSERT (totalOfChannelsIn >= 0);
    PD_ASSERT (totalOfChannelsOut >= 0);
    
    if (audio_soundIn)  { garbage_newRaw ((void *)audio_soundIn);  audio_soundIn  = NULL; }
    if (audio_soundOut) { garbage_newRaw ((void *)audio_soundOut); audio_soundOut = NULL; }
    
    audio_soundIn  = (t_sample *)PD_MEMORY_GET (m);
    audio_soundOut = (t_sample *)PD_MEMORY_GET (n);
    
    audio_totalOfChannelsIn  = totalOfChannelsIn;
    audio_totalOfChannelsOut = totalOfChannelsOut;
    
    atomic_float64Write (&audio_sampleRate, sampleRate);
}

void audio_vectorShrinkIn (int totalOfChannelsIn)
{
    PD_ASSERT (totalOfChannelsIn <= audio_totalOfChannelsIn);
    
    audio_totalOfChannelsIn = totalOfChannelsIn;
}

void audio_vectorShrinkOut (int totalOfChannelsOut)
{
    PD_ASSERT (totalOfChannelsOut <= audio_totalOfChannelsOut);
    
    audio_totalOfChannelsOut = totalOfChannelsOut;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audio_setCurrentDevices (void)
{
    /*
    int m, n;
    
    deviceslist_setDevices (&audio_devices, p, setParameters);
    
    m = deviceslist_getTotalOfChannelsIn (&audio_devices);
    n = deviceslist_getTotalOfChannelsOut (&audio_devices);
    
    audio_vectorInitialize (AUDIO_DEFAULT_SAMPLERATE, m, n);
    */
    
    // outputs_reportCurrentAudioDevices (&audio_devices);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_initialize (void)
{
    pthread_mutex_init (&audio_mutex, NULL); return audio_initializeNative();
}

void audio_release (void)
{
    audio_releaseNative();
    
    if (audio_soundIn)  {
        PD_MEMORY_FREE (audio_soundIn);  audio_soundIn  = NULL; 
    }
    
    if (audio_soundOut) {
        PD_MEMORY_FREE (audio_soundOut); audio_soundOut = NULL; 
    }
    
    pthread_mutex_destroy (&audio_mutex);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
