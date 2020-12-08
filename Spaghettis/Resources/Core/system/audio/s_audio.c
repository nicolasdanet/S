
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

extern t_deviceslist audio_devices;

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

PD_LOCAL void audio_vectorInitialize (t_float, int, int);
PD_LOCAL void metadata_report        (t_error err);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL int audio_isOpened (void)
{
    return (audio_state != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void audio_report (t_error err, t_devices *p)
{
    t_symbol *i     = devices_getInSize (p)  ? devices_getInAtIndexAsSymbol (p, 0)  : &s_;
    t_symbol *o     = devices_getOutSize (p) ? devices_getOutAtIndexAsSymbol (p, 0) : &s_;
    int m           = devices_getInSize (p)  ? devices_getInChannelsAtIndex (p, 0)  : 0;
    int n           = devices_getOutSize (p) ? devices_getOutChannelsAtIndex (p, 0) : 0;
    int sampleRate  = devices_getSampleRate (p);
    
    #if PD_APPLE
    
    int vectorSize  = devices_getVectorSize (p);
    
    #endif
    
    void (*f)(const char *fmt, ...) = err ? post_error : post;
    
    (f) (PD_TRANSLATE ("dsp: %s / %d channels"), i->s_name, m);                 // --
    (f) (PD_TRANSLATE ("dsp: %s / %d channels"), o->s_name, n);                 // --
    
    #if PD_APPLE
    
    (f) (PD_TRANSLATE ("dsp: %d Hz / %d frames"), sampleRate, vectorSize);      // --
    
    #else 
    
    (f) (PD_TRANSLATE ("dsp: %d Hz"), sampleRate);                              // --
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Notice that for now only the first device is opened. */

PD_LOCAL t_error audio_open (void)
{
    t_error err = PD_ERROR;
    
    t_devices audio; devices_initAsAudio (&audio);
    
    audio_getDevices (&audio);
    
    if (audio_check (&audio) == PD_ERROR_NONE) {
    //
    pthread_mutex_lock (&audio_mutex);
    
        if (devices_getInSize (&audio) || devices_getOutSize (&audio)) {
            int m = audio_getTotalOfChannelsIn();
            int n = audio_getTotalOfChannelsOut();
            audio_vectorInitialize (devices_getSampleRate (&audio), m, n);
            err = audio_openNative (&audio);
        }

        audio_state = err ? 0 : 1;
    
    pthread_mutex_unlock (&audio_mutex);
    //
    }

    metadata_report (err); audio_report (err, &audio);
    
    return err;
}

PD_LOCAL void audio_close (void)
{
    pthread_mutex_lock (&audio_mutex);
    
    if (audio_isOpened()) { audio_closeNative(); } audio_state = 0;
    
    pthread_mutex_unlock (&audio_mutex);
}

PD_LOCAL int audio_poll (void)
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

PD_LOCAL t_error audio_stop (void)
{
    if (audio_isOpened()) { audio_close(); } return PD_ERROR_NONE;
}

PD_LOCAL t_error audio_start (void)
{
    if (!audio_isOpened()) { return audio_open(); } else { return PD_ERROR_NONE; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL double audio_getNanosecondsToSleep (void)
{
    double t = INTERNAL_BLOCKSIZE / audio_getSampleRate();
    
    return PD_SECONDS_TO_NANOSECONDS (t);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_float audio_getSampleRate (void)
{
    t_float f = PD_ATOMIC_FLOAT64_READ (&audio_sampleRate); return (f <= 0.0 ? AUDIO_DEFAULT_SAMPLERATE : f);
}

PD_LOCAL int audio_getTotalOfChannelsIn (void) 
{
    return audio_totalOfChannelsIn;
}

PD_LOCAL int audio_getTotalOfChannelsOut (void)
{
    return audio_totalOfChannelsOut; 
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void audio_vectorInitialize (t_float sampleRate, int totalOfChannelsIn, int totalOfChannelsOut)
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
    
    PD_ATOMIC_FLOAT64_WRITE (sampleRate, &audio_sampleRate);
}

PD_LOCAL void audio_vectorShrinkIn (int totalOfChannelsIn)
{
    PD_ASSERT (totalOfChannelsIn <= audio_totalOfChannelsIn);
    
    audio_totalOfChannelsIn = totalOfChannelsIn;
}

PD_LOCAL void audio_vectorShrinkOut (int totalOfChannelsOut)
{
    PD_ASSERT (totalOfChannelsOut <= audio_totalOfChannelsOut);
    
    audio_totalOfChannelsOut = totalOfChannelsOut;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error audio_initialize (void)
{
    pthread_mutex_init (&audio_mutex, NULL); return audio_initializeNative();
}

PD_LOCAL void audio_release (void)
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
