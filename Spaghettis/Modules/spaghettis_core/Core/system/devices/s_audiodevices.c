
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audiodevices_initialize (t_audiodevices *p)
{
    int i;

    p->d_inSize             = 0;
    p->d_outSize            = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    p->d_inChannels[i]      = 0;
    p->d_outChannels[i]     = 0;
    p->d_inNames[i]         = NULL;
    p->d_outNames[i]        = NULL;
    //
    }
}

void audiodevices_copy (t_audiodevices *dest, t_audiodevices *src)
{
    int i;
    
    dest->d_inSize          = src->d_inSize;
    dest->d_outSize         = src->d_outSize;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    dest->d_inChannels[i]   = src->d_inChannels[i];
    dest->d_outChannels[i]  = src->d_outChannels[i];
    dest->d_inNames[i]      = src->d_inNames[i];
    dest->d_outNames[i]     = src->d_outNames[i];
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error audiodevices_addAudioIn (t_audiodevices *p, t_symbol *device, int channels)
{
    if (p->d_inSize < DEVICES_MAXIMUM_IO) {
    //
    int t = PD_CLAMP (channels, -DEVICES_MAXIMUM_CHANNELS, DEVICES_MAXIMUM_CHANNELS);
    p->d_inNames[p->d_inSize]    = device;
    p->d_inChannels[p->d_inSize] = t;
    p->d_inSize++;
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

t_error audiodevices_addAudioOut (t_audiodevices *p, t_symbol *device, int channels)
{
    if (p->d_outSize < DEVICES_MAXIMUM_IO) {
    //
    int t = PD_CLAMP (channels, -DEVICES_MAXIMUM_CHANNELS, DEVICES_MAXIMUM_CHANNELS);
    p->d_outNames[p->d_outSize]    = device;
    p->d_outChannels[p->d_outSize] = t;
    p->d_outSize++;
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void audiodevices_report (t_audiodevices *p)
{
    int i;
    
    for (i = 0; i < p->d_inSize; i++) {
        const char *s = symbol_getName (p->d_inNames[i]);
        int n = p->d_inChannels[i];
        post_system (NULL, PD_TRANSLATE ("rescan: audio in / %s / %d"), s, n);
    }
    
    for (i = 0; i < p->d_outSize; i++) {
        const char *s = symbol_getName (p->d_outNames[i]);
        int n = p->d_outChannels[i];
        post_system (NULL, PD_TRANSLATE ("rescan: audio out / %s / %d"), s, n);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
