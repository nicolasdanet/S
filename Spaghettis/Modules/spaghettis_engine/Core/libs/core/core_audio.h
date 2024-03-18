
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef CORE_AUDIO_H_
#define CORE_AUDIO_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define coreaudio_error(err)                core_audioCheck ((err), __LINE__)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define COREAUDIO_INPUT_ELEMENT             1
#define COREAUDIO_OUTPUT_ELEMENT            0

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define COREAUDIO_BLOCKSIZE_MAXIMUM         4096

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_error core_audioCheck (OSStatus error, int line)
{
    t_error err = (error != noErr);
    
    #if PD_WITH_DEBUG
    
    if (err) {
    //
    post_log ("*** Core Audio / line %d", line);
    //
    }
    
    #endif // PD_WITH_DEBUG
    
    return err;
}

static inline t_symbol *core_CFStringToSymbol (CFStringRef string)
{
    if (string != NULL) {
    //
    char t[PD_STRING] = { 0 };
    
    if (CFStringGetCString (string, t, PD_STRING, kCFStringEncodingUTF8)) {
        return gensym (t);
    }
    //
    }
    
    return NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error        audiodevicelist_get     (t_deviceslist *list);

AudioObjectID  audiodevicelist_fetch   (t_symbol *name);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _audiodevice {
    AudioObjectID       d_ID;
    AudioUnit           d_unit;
    int                 d_hasListener;
    int                 d_isInput;
    int                 d_isOutput;
    int                 d_isDuplex;
    int                 d_isReady;
    int                 d_isRunning;
    } t_audiodevice;

typedef struct _audiograph {
    t_audiodevice       g_deviceIn;
    t_audiodevice       g_deviceOut;
    t_audiodevice       g_deviceDuplex;
    int                 g_sampleRate;
    int                 g_vectorSize;
    int                 g_channelsIn;
    int                 g_channelsOut;
    int                 g_hasInput;
    int                 g_hasOutput;
    int                 g_hasDuplex;
    t_int32Atomic       g_isRunning;
    t_int32Atomic       g_flag;
    t_int32Atomic       g_blank;
    AudioBufferList     *g_data;
    } t_audiograph;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    audiodevice_set                         (t_audiodevice *device, AudioObjectID deviceID);
t_error    audiodevice_setAsInput                  (t_audiodevice *device, AudioObjectID deviceID);
t_error    audiodevice_setAsOutput                 (t_audiodevice *device, AudioObjectID deviceID);
t_error    audiodevice_setAsDuplex                 (t_audiodevice *device, AudioObjectID deviceID);
void       audiodevice_setInvalid                  (t_audiodevice *device);
int        audiodevice_isValid                     (t_audiodevice *device);
int        audiodevice_isInput                     (t_audiodevice *device);
int        audiodevice_isOutput                    (t_audiodevice *device);
int        audiodevice_isDuplex                    (t_audiodevice *device);

t_symbol   *audiodevice_getName                    (t_audiodevice *device);

AudioObjectID  audiodevice_getIdentifier           (t_audiodevice *device);
int            audiodevice_getNumberOfChannelsIn   (t_audiodevice *device);
int            audiodevice_getNumberOfChannelsOut  (t_audiodevice *device);
int            audiodevice_getSampleRate           (t_audiodevice *device);

int        audiodevice_getBufferSize               (t_audiodevice *device);
t_error    audiodevice_setBufferSize               (t_audiodevice *device, int size);

t_error    audiodevice_addListeners                (t_audiodevice *device, t_audiograph *graph);
t_error    audiodevice_removeListeners             (t_audiodevice *device, t_audiograph *graph);

t_error    audiodevice_open                        (t_audiodevice *device, t_audiograph *graph);
t_error    audiodevice_close                       (t_audiodevice *device, t_audiograph *graph);

t_error    audiodevice_start                       (t_audiodevice *device);
t_error    audiodevice_stop                        (t_audiodevice *device);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline AudioUnit audiodevice_getAudioUnit (t_audiodevice *device)
{
    return device->d_unit;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    audiograph_initialize                   (t_audiograph *graph);
void       audiograph_release                      (t_audiograph *graph);

t_error    audiograph_open                         (t_audiograph *graph,
                                                                t_symbol *deviceIn,
                                                                t_symbol *deviceOut,
                                                                int numberOfChannelsIn,
                                                                int numberOfChannelsOut,
                                                                int sampleRate,
                                                                int vectorSize);

void       audiograph_close                        (t_audiograph *graph);

t_error    audiograph_check                        (t_audiograph *graph, int close);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // CORE_AUDIO_H_
