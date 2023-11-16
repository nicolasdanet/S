
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void devices_init (t_devices *p)
{
    int i;
    
    p->d_sampleRate     = AUDIO_DEFAULT_SAMPLERATE;
    p->d_vectorSize     = INTERNAL_BLOCKSIZE;
    p->d_inSize         = 0;
    p->d_outSize        = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    p->d_in[i]          = 0;
    p->d_out[i]         = 0;
    p->d_inChannels[i]  = 0;
    p->d_outChannels[i] = 0;
    //
    }
}

void devices_initAsAudio (t_devices *p)
{
    devices_init (p);
    
    p->d_isMidi = 0;
}

void devices_initAsMidi (t_devices *p)
{
    devices_init (p);
    
    p->d_isMidi = 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void devices_setDefaultsAudio (t_devices *p)
{
    if (p->d_inSize == 0) { 
        p->d_in[0]              = 0;
        p->d_inChannels[0]      = 2;
        p->d_inSize             = 1;
    }
    
    if (p->d_outSize == 0) { 
        p->d_out[0]             = 0;
        p->d_outChannels[0]     = 2;
        p->d_outSize            = 1;
    }
}

static void devices_setDefaultsMidi (t_devices *p)
{
    if (p->d_inSize == 0) { 
        p->d_in[0]              = 0;
        p->d_inSize             = 1;
    }
    
    if (p->d_outSize == 0) { 
        p->d_out[0]             = 0;
        p->d_outSize            = 1;
    }
}

void devices_setDefaultsIfNone (t_devices *p)
{
    if (p->d_isMidi) { devices_setDefaultsMidi (p); }
    else {
        devices_setDefaultsAudio (p);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_setSampleRate (t_devices *p, int n)
{
    PD_ASSERT (!p->d_isMidi);
    
    if (n < 1) { n = AUDIO_DEFAULT_SAMPLERATE; }
    
    p->d_sampleRate = n;
}

int devices_getSampleRate (t_devices *p)
{
    return p->d_sampleRate;
}

void devices_setVectorSize (t_devices *p, int n)
{
    PD_ASSERT (!p->d_isMidi);
    
    if (n > 0 && PD_IS_POWER_2 (n)) { p->d_vectorSize = n; }
}

int devices_getVectorSize (t_devices *p)
{
    return p->d_vectorSize;
}

int devices_getInSize (t_devices *p)
{
    return p->d_inSize;
}

int devices_getOutSize (t_devices *p)
{
    return p->d_outSize;
}

int devices_getInAtIndex (t_devices *p, int i)
{
    return (i < p->d_inSize) ? p->d_in[i] : -1;
}

int devices_getOutAtIndex (t_devices *p, int i)
{
    return (i < p->d_outSize) ? p->d_out[i] : -1;
}

int devices_getInChannelsAtIndex (t_devices *p, int i)
{
    PD_ASSERT (!p->d_isMidi);
    PD_ASSERT (i < DEVICES_MAXIMUM_IO);
    
    return p->d_inChannels[i];
}

int devices_getOutChannelsAtIndex (t_devices *p, int i)
{
    PD_ASSERT (!p->d_isMidi);
    PD_ASSERT (i < DEVICES_MAXIMUM_IO);
    
    return p->d_outChannels[i];
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void devices_check (t_devices *p)
{
    int i;
    
    PD_ASSERT (!p->d_isMidi);
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
        p->d_inChannels[i]  = PD_MAX (0, p->d_inChannels[i]);
        p->d_outChannels[i] = PD_MAX (0, p->d_outChannels[i]);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error devices_appendMidiIn (t_devices *p, int n)
{
    PD_ASSERT (p->d_isMidi);
    
    if (n < 0 || p->d_inSize >= DEVICES_MAXIMUM_IO) { return PD_ERROR; }
    else {
        p->d_in[p->d_inSize] = n;
        p->d_inSize++;
    }
    
    return PD_ERROR_NONE;
}

t_error devices_appendMidiOut (t_devices *p, int n)
{
    PD_ASSERT (p->d_isMidi);
    
    if (n < 0 || p->d_outSize >= DEVICES_MAXIMUM_IO) { return PD_ERROR; }
    else {
        p->d_out[p->d_outSize] = n;
        p->d_outSize++;
    }
    
    return PD_ERROR_NONE;
}

t_error devices_appendAudioIn (t_devices *p, int n, int channels)
{
    PD_ASSERT (!p->d_isMidi);
    
    if (n < 0 || p->d_inSize >= DEVICES_MAXIMUM_IO) { return PD_ERROR; }
    else {
    //
    p->d_in[p->d_inSize] = n;
    p->d_inChannels[p->d_inSize] = PD_CLAMP (channels, -DEVICES_MAXIMUM_CHANNELS, DEVICES_MAXIMUM_CHANNELS);
    p->d_inSize++;
    //
    }
    
    return PD_ERROR_NONE;
}

t_error devices_appendAudioOut (t_devices *p, int n, int channels)
{
    PD_ASSERT (!p->d_isMidi);
    
    if (n < 0 || p->d_outSize >= DEVICES_MAXIMUM_IO) { return PD_ERROR; }
    else {
    //
    p->d_out[p->d_outSize] = n;
    p->d_outChannels[p->d_outSize] = PD_CLAMP (channels, -DEVICES_MAXIMUM_CHANNELS, DEVICES_MAXIMUM_CHANNELS);
    p->d_outSize++;
    //
    }
    
    return PD_ERROR_NONE;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
