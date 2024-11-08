
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

    d->d_default = 0;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    d->d_in[i]   = NULL;
    d->d_out[i]  = NULL;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

void devices_set (t_devices *d, const juce::StringArray& inputs, const juce::StringArray& outputs)
{
    devices_initialize (d);
    
    {
        int k = 0;
        
        for (const auto& i : inputs) {
            if (k < DEVICES_MAXIMUM_IO) {
                d->d_in[k++] = i.isNotEmpty()  ? gensym (i.toRawUTF8()) : sym_none;
            }
        }
    }
    
    {
        int k = 0;
        
        for (const auto& o : outputs) {
            if (k < DEVICES_MAXIMUM_IO) {
                d->d_out[k++] = o.isNotEmpty() ? gensym (o.toRawUTF8()) : sym_none;
            }
        }
    }
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_copy (t_devices *d, t_devices *from)
{
    int i;

    d->d_default = from->d_default;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    d->d_in[i]   = from->d_in[i];
    d->d_out[i]  = from->d_out[i];
    //
    }
}

int devices_areEquals (t_devices *d, t_devices *from)
{
    int i;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    if (d->d_in[i]  != from->d_in[i])  { return 0; }
    if (d->d_out[i] != from->d_out[i]) { return 0; }
    //
    }
    
    return 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_setDefault (t_devices *d, int n)
{
    d->d_default = n;
}

int devices_hasDefault (t_devices *d)
{
    return (d->d_default != 0);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int devices_setDefaultDevices (t_devices *d, t_symbol *input, t_symbol *output)
{
    int k = 0;
    
    if (input && output) {
    //
    int i;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
        if (d->d_in[i]  == sym_none) { d->d_in[i]  = input;  k = 1; }
        if (d->d_out[i] == sym_none) { d->d_out[i] = output; k = 1; }
    }
    //
    }
    
    if (k) { d->d_default = 1; }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int devices_setDefaultAudio (t_devices *d)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    return devices_setDefaultDevices (d, audiodevices_getDefaultIn (&l), audiodevices_getDefaultOut (&l));
}

int devices_setDefaultMidi (t_devices *d)
{
    t_mididevices l; midi_getListOfDevices (&l);
    
    return devices_setDefaultDevices (d, mididevices_getDefaultIn (&l), mididevices_getDefaultOut (&l));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error devices_checkAudio (t_devices *d)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    int i;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_in[i];
        if (s && !audiodevices_hasAudioIn (&l, s)) {
            return PD_ERROR;
        }
    }
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_out[i];
        if (s && !audiodevices_hasAudioOut (&l, s)) {
            return PD_ERROR;
        }
    }
    
    if (d->d_in[0] != NULL && d->d_out[0] != NULL) { return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
}

t_error devices_checkMidi (t_devices *d)
{
    t_mididevices l; midi_getListOfDevices (&l);
    
    int i;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_in[i];
        if (s && !mididevices_hasMidiIn (&l, s))  {
            return PD_ERROR;
        }
    }
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
        t_symbol *s = d->d_out[i];
        if (s && !mididevices_hasMidiOut (&l, s)) {
            return PD_ERROR;
        }
    }
    
    if (d->d_in[0] != NULL && d->d_out[0] != NULL) { return PD_ERROR_NONE; }
    else {
        return PD_ERROR;
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_logAudio (t_devices *p, t_error err)
{
    t_symbol *i = devices_getInName (p, 0);
    t_symbol *o = devices_getOutName (p, 0);
    
    void (*f)(t_object *, const char *fmt, ...) = err ? post_error : post_system;
    
    if (!i) { (f) (NULL, PD_TRANSLATE ("DSP: no input"));  }
    if (!o) { (f) (NULL, PD_TRANSLATE ("DSP: no output")); }
    
    if (devices_hasDefault (p)) {
        post_warning (NULL, PD_TRANSLATE ("DSP: using default devices"));
    }
    
    if (i && o) {
        (f) (NULL, PD_TRANSLATE ("DSP: %s / %d channels"), symbol_getName (o), devices_getOutChannels (p, 0));
        (f) (NULL, PD_TRANSLATE ("DSP: %s / %d channels"), symbol_getName (i), devices_getInChannels (p, 0));
    }
    
    (f) (NULL, PD_TRANSLATE ("DSP: %d Hz"), AUDIO_DEFAULT_SAMPLERATE);
}

void devices_logMidi (t_devices *p, t_error err)
{
    int m = devices_getInSize (p);
    int n = devices_getOutSize (p);
    
    void (*f)(t_object *, const char *fmt, ...) = err ? post_error : post_system;
    
    int i;
    
    if (!m) { (f) (NULL, PD_TRANSLATE ("MIDI: no input"));  }
    if (!n) { (f) (NULL, PD_TRANSLATE ("MIDI: no output")); }
    
    if (devices_hasDefault (p)) {
        post_warning (NULL, PD_TRANSLATE ("MIDI: using default devices"));
    }
    
    for (i = 0; i < m; i++) {
        (f) (NULL, PD_TRANSLATE ("MIDI: %s"), symbol_getName (devices_getInName (p, i)));
    }
    for (i = 0; i < n; i++) {
        (f) (NULL, PD_TRANSLATE ("MIDI: %s"), symbol_getName (devices_getOutName (p, i)));
    }
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

int devices_getInSize (t_devices *d)
{
    int i, k = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    if (d->d_in[i] != NULL) { k++; }
    else {
        break;
    }
    //
    }
    
    return k;
}

int devices_getOutSize (t_devices *d)
{
    int i, k = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    if (d->d_out[i] != NULL) { k++; }
    else {
        break;
    }
    //
    }
    
    return k;
}

int devices_getInChannels (t_devices *d, int n)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    return audiodevices_getNumberOfChannelsIn (&l, d->d_in[n]);
}

int devices_getOutChannels (t_devices *d, int n)
{
    t_audiodevices l; audio_getListOfDevices (&l);
    
    return audiodevices_getNumberOfChannelsOut (&l, d->d_out[n]);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
