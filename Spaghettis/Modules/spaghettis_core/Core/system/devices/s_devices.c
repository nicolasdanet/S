
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_initialize (t_devices *d)
{
    int i;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    d->d_in[i]  = NULL;
    d->d_out[i] = NULL;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_copy (t_devices *d, t_devices *from)
{
    int i;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    d->d_in[i]  = from->d_in[i];
    d->d_out[i] = from->d_out[i];
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void devices_logAudio (t_error err, t_devices *p)
{
    t_symbol *i = devices_getInName (p, 0);
    t_symbol *o = devices_getOutName (p, 0);
    int m       = devices_getInChannels (p, 0);
    int n       = devices_getOutChannels (p, 0);
    
    void (*f)(t_object *, const char *fmt, ...) = err ? post_error : post_system;
    
    if (i == NULL) { i = &s_; }
    if (o == NULL) { o = &s_; }
    
    (f) (NULL, PD_TRANSLATE ("dsp: %s / %d channels"), symbol_getName (i), m);
    (f) (NULL, PD_TRANSLATE ("dsp: %s / %d channels"), symbol_getName (o), n);
    (f) (NULL, PD_TRANSLATE ("dsp: %d Hz"), AUDIO_DEFAULT_SAMPLERATE);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error devices_checkAudio (t_devices *d)
{
    t_error err = PD_ERROR;
    
    t_audiodevices l; audio_getListOfDevices (&l);
    
    t_devices t; devices_initialize (&t);
    
    int i, k;

    for (i = 0, k = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_in[i];
        if (audiodevices_hasAudioIn (&l, s)) {
            t.d_in[k++] = s;
        }
    }
    
    for (i = 0, k = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_out[i];
        if (audiodevices_hasAudioOut (&l, s)) {
            t.d_out[k++] = s;
        }
    }
    
    devices_copy (d, &t);
    
    if (d->d_in[0] != NULL && d->d_out[0] != NULL) { err = PD_ERROR_NONE; }
    
    devices_logAudio (err, d);
    
    return err;
}

t_error devices_checkMidi (t_devices *d)
{
    t_mididevices l; midi_getListOfDevices (&l);
    
    t_devices t; devices_initialize (&t);
    
    int i, k;

    for (i = 0, k = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_in[i];
        if (mididevices_hasMidiIn (&l, s)) {
            t.d_in[k++] = s;
        }
    }
    
    for (i = 0, k = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_out[i];
        if (mididevices_hasMidiOut (&l, s)) {
            t.d_out[k++] = s;
        }
    }
    
    devices_copy (d, &t);
    
    if (d->d_in[0] != NULL || d->d_out[0] != NULL) { return PD_ERROR_NONE; }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int devices_getTotalOfChannelsIn (t_devices *d)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    int i, k = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    k += audiodevices_getNumberOfChannelsIn (&l, d->d_in[i]);
    //
    }
    
    return k;
}

int devices_getTotalOfChannelsOut (t_devices *d)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    int i, k = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    k += audiodevices_getNumberOfChannelsOut (&l, d->d_out[i]);
    //
    }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol *devices_getInName (t_devices *d, int n)
{
    PD_ASSERT (n < DEVICES_MAXIMUM_IO);
    
    return d->d_in[n];
}

t_symbol *devices_getOutName (t_devices *d, int n)
{
    PD_ASSERT (n < DEVICES_MAXIMUM_IO);
    
    return d->d_out[n];
}

int devices_getInChannels (t_devices *d, int n)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    int k = audiodevices_getNumberOfChannelsIn (&l, d->d_in[n]);

    PD_ASSERT (k);
    
    return k;
}

int devices_getOutChannels (t_devices *d, int n)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    int k = audiodevices_getNumberOfChannelsOut (&l, d->d_out[n]);

    PD_ASSERT (k);
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
