
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_devices audio_devices;                         /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

t_sample *audio_soundIn;                                /* Static. */
t_sample *audio_soundOut;                               /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int              audio_totalOfChannelsIn;        /* Static. */
static int              audio_totalOfChannelsOut;       /* Static. */
static t_float64Atomic  audio_sampleRate;               /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int              audio_state;                    /* Static. */
static pthread_mutex_t  audio_mutex;                    /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_start (void)
{
    if (!audio_isOpened()) { return audio_open(); } else { return PD_ERROR_NONE; }
}

t_error audio_stop (void)
{
    if (audio_isOpened()) { audio_close(); } return PD_ERROR_NONE;
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

static void audio_log (t_error err, t_devices *p)
{
    t_symbol *i = devices_getInName (p, 0);
    t_symbol *o = devices_getOutName (p, 0);
    
    void (*f)(t_object *, const char *fmt, ...) = err ? post_error : post_system;
    
    if (!i) { (f) (NULL, PD_TRANSLATE ("DSP: no input"));  }
    else {
        (f) (NULL, PD_TRANSLATE ("DSP: %s / %d channels"), symbol_getName (i), devices_getInChannels (p, 0));
    }
    
    if (!o) { (f) (NULL, PD_TRANSLATE ("DSP: no output")); }
    else {
        (f) (NULL, PD_TRANSLATE ("DSP: %s / %d channels"), symbol_getName (o), devices_getOutChannels (p, 0));
    }
    
    (f) (NULL, PD_TRANSLATE ("DSP: %d Hz"), AUDIO_DEFAULT_SAMPLERATE);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_open (void)
{
    t_error err = devices_checkAudio (&audio_devices);
    
    if (err == PD_ERROR_NONE) {
    //
    /* ??? */
    /*
    pthread_mutex_lock (&audio_mutex);
    
        {
            int m = devices_getTotalOfChannelsIn (&audio_devices);
            int n = devices_getTotalOfChannelsOut (&audio_devices);
            audio_vectorInitialize (AUDIO_DEFAULT_SAMPLERATE, m, n);
            err = audio_openNative (&audio_devices);
        }

        audio_state = err ? 0 : 1;
    
    pthread_mutex_unlock (&audio_mutex);
    */
    //
    }
    
    audio_log (err, &audio_devices);
    
    return err;
}

void audio_close (void)
{
    pthread_mutex_lock (&audio_mutex);
    
    if (audio_isOpened()) { audio_closeNative(); } audio_state = 0;
    
    pthread_mutex_unlock (&audio_mutex);
}

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

void audio_setCurrentDevices (t_devices *p)
{
    devices_copy (&audio_devices, p);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_initialize (void)
{
    pthread_mutex_init (&audio_mutex, NULL);
    
    devices_initialize (&audio_devices);
    
    return audio_initializeNative();
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
