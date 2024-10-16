
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_AUDIO_BACKENDS_H_
#define S_AUDIO_BACKENDS_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audio_open          (void);
void    audio_close         (void);
int     audio_isOpened      (void);

void    audio_getDevices    (t_devices *p);
void    audio_setDevices    (t_devices *p, int setAlsoParameters);

void    audio_rescanDevices (void);

t_error audio_check         (t_devices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char  *audio_nameNative       (void);

int     audio_getVectorSizeNative   (void);
t_error audio_getListsNative        (t_audiodevices *);
t_error audio_initializeNative      (void);
void    audio_releaseNative         (void);
void    audio_closeNative           (void);
t_error audio_openNative            (t_devices *);
int     audio_pollNative            (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline int audio_getVectorSize (void)
{
    return audio_getVectorSizeNative();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_AUDIO_BACKENDS_H_
