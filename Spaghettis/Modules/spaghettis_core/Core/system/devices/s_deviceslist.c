
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void deviceslist_initProceed (t_deviceslist *p, int k)
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

void deviceslist_init (t_deviceslist *p)
{
    deviceslist_initProceed (p, 1);
}

void deviceslist_reset (t_deviceslist *p)
{
    deviceslist_initProceed (p, 0);
}

void deviceslist_copy (t_deviceslist *dest, t_deviceslist *src)
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

int deviceslist_areEquals (t_deviceslist *p, t_deviceslist *q)
{
    if (p->d_inSize                 != q->d_inSize)         { return 0; }
    else if (p->d_outSize           != q->d_outSize)        { return 0; }
    else {
    //
    int i;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    if (p->d_inChannels[i]          != q->d_inChannels[i])  { return 0; }
    else if (p->d_outChannels[i]    != q->d_outChannels[i]) { return 0; }
    else if (p->d_inNames[i]        != q->d_inNames[i])     { return 0; }
    else if (p->d_outNames[i]       != q->d_outNames[i])    { return 0; }
    //
    }
    //
    }
    
    return 1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int deviceslist_containsIn (t_deviceslist *p, t_symbol *device)
{
    PD_ASSERT (device);
    
    int i; for (i = 0; i < DEVICES_MAXIMUM_IO; i++) { if (p->d_inNames[i]  == device) { return i; } }
    
    return -1;
}

int deviceslist_containsOut (t_deviceslist *p, t_symbol *device)
{
    PD_ASSERT (device);
    
    int i; for (i = 0; i < DEVICES_MAXIMUM_IO; i++) { if (p->d_outNames[i] == device) { return i; } }
    
    return -1;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int deviceslist_getInSize (t_deviceslist *p)
{
    return p->d_inSize;
}

int deviceslist_getOutSize (t_deviceslist *p)
{
    return p->d_outSize;
}

int deviceslist_getInChannelsAtIndex (t_deviceslist *p, int i)
{
    PD_ASSERT (i >= 0);
    PD_ASSERT (i < DEVICES_MAXIMUM_IO);
    
    return p->d_inChannels[i];
}

int deviceslist_getOutChannelsAtIndex (t_deviceslist *p, int i)
{
    PD_ASSERT (i >= 0);
    PD_ASSERT (i < DEVICES_MAXIMUM_IO);
    
    return p->d_outChannels[i];
}

t_symbol *deviceslist_getInAtIndex (t_deviceslist *p, int i)
{
    PD_ASSERT (i >= 0);
    PD_ASSERT (i < DEVICES_MAXIMUM_IO);
    
    return p->d_inNames[i];
}

t_symbol *deviceslist_getOutAtIndex (t_deviceslist *p, int i)
{
    PD_ASSERT (i >= 0);
    PD_ASSERT (i < DEVICES_MAXIMUM_IO);
    
    return p->d_outNames[i];
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int deviceslist_getTotalOfChannelsIn (t_deviceslist *p)
{
    int i, k = 0;
    
    for (i = 0; i < p->d_inSize; i++) {
        int n = p->d_inChannels[i]; if (n > 0) { k += n; }
    }
    
    return k;
}

int deviceslist_getTotalOfChannelsOut (t_deviceslist *p)
{
    int i, k = 0;
    
    for (i = 0; i < p->d_outSize; i++) {
        int n = p->d_outChannels[i]; if (n > 0) { k += n; }
    }
    
    return k;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error deviceslist_appendMidiIn (t_deviceslist *p, t_symbol *device)
{
    if (p->d_inSize < DEVICES_MAXIMUM_IO) {
    //
    p->d_inNames[p->d_inSize] = device;
    p->d_inSize++;
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

t_error deviceslist_appendMidiOut (t_deviceslist *p, t_symbol *device)
{
    if (p->d_outSize < DEVICES_MAXIMUM_IO) {
    //
    p->d_outNames[p->d_outSize] = device;
    p->d_outSize++;
    return PD_ERROR_NONE;
    //
    }
    
    return PD_ERROR;
}

t_error deviceslist_appendAudioIn (t_deviceslist *p, t_symbol *device, int channels)
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

t_error deviceslist_appendAudioOut (t_deviceslist *p, t_symbol *device, int channels)
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

void deviceslist_setDevices (t_deviceslist *l, t_devices *p, int setParameters)
{
    int i;
    
    if (setParameters) { deviceslist_init (l); } else { deviceslist_reset (l); }
    
    /* ??? */
    /*
    if (p->d_isMidi) {
    
        for (i = 0; i < devices_getInSize (p); i++) {
            deviceslist_appendMidiInAsNumber (l, devices_getInAtIndex (p, i));
        }
        
        for (i = 0; i < devices_getOutSize (p); i++) {
            deviceslist_appendMidiOutAsNumber (l, devices_getOutAtIndex (p, i));
        }
    
    } else {
        
        for (i = 0; i < devices_getInSize (p); i++) {
            deviceslist_appendAudioInAsNumber (l,
                devices_getInAtIndex (p, i),
                devices_getInChannelsAtIndex (p, i));
        }

        for (i = 0; i < devices_getOutSize (p); i++) {
            deviceslist_appendAudioOutAsNumber (l,
                devices_getOutAtIndex (p, i),
                devices_getOutChannelsAtIndex (p, i));
        }
    }
    */
}

void deviceslist_getDevices (t_deviceslist *l, t_devices *p)
{
    int i;
    /* ??? */
    /*
    if (p->d_isMidi) {
        
        for (i = 0; i < deviceslist_getInSize (l); i++) {
            devices_appendMidiInWithSymbol (p, deviceslist_getInAtIndex (l, i));
        }
        
        for (i = 0; i < deviceslist_getOutSize (l); i++) {
            devices_appendMidiOutWithSymbol (p, deviceslist_getOutAtIndex (l, i));
        }
    
    } else {
    
        for (i = 0; i < deviceslist_getInSize (l); i++) {
            devices_appendAudioInWithSymbol (p,
                deviceslist_getInAtIndex (l, i),
                deviceslist_getInChannelsAtIndex (l, i));
        }
        
        for (i = 0; i < deviceslist_getOutSize (l); i++) {
            devices_appendAudioOutWithSymbol (p,
                deviceslist_getOutAtIndex (l, i),
                deviceslist_getOutChannelsAtIndex (l, i));
        }
    }
    */
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
