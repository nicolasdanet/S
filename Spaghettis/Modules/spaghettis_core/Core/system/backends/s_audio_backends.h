
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

void        audio_getListOfDevices          (t_audiodevices *);
void        audio_rescanDevices             (int);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char  *audio_getBackendNameNative     (void);

int         audio_getVectorSizeNative       (void);
t_error     audio_getListOfDevicesNative    (t_audiodevices *);
t_error     audio_initializeNative          (void);
void        audio_releaseNative             (void);
void        audio_closeNative               (void);
t_error     audio_openNative                (t_devices *);
int         audio_pollNative                (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_AUDIO_BACKENDS_H_
