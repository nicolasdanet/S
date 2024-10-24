
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void mididevices_initialize (t_mididevices *p)
{
    int i;

    p->d_inSize         = 0;
    p->d_outSize        = 0;

    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    p->d_inNames[i]     = NULL;
    p->d_outNames[i]    = NULL;
    //
    }
}

void mididevices_copy (t_mididevices *dest, t_mididevices *src)
{
    int i;
    
    dest->d_inSize      = src->d_inSize;
    dest->d_outSize     = src->d_outSize;
    
    for (i = 0; i < DEVICES_MAXIMUM_IO; i++) {
    //
    dest->d_inNames[i]  = src->d_inNames[i];
    dest->d_outNames[i] = src->d_outNames[i];
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error mididevices_appendMidiIn (t_mididevices *p, t_symbol *device)
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

t_error mididevices_appendMidiOut (t_mididevices *p, t_symbol *device)
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int mididevices_hasMidiIn (t_mididevices *p, t_symbol *device)
{
    if (device != NULL) {
    //
    int i;
            
    for (i = 0; i < p->d_inSize; i++) {
        if (p->d_inNames[i] == device) {
            return 1;
        }
    }
    //
    }
    
    return 0;
}

int mididevices_hasMidiOut (t_mididevices *p, t_symbol *device)
{
    if (device != NULL) {
    //
    int i;
    
    for (i = 0; i < p->d_outSize; i++) {
        if (p->d_outNames[i] == device) {
            return 1;
        }
    }
    //
    }
    
    return 0;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void mididevices_report (t_mididevices *p)
{
    int i;
    
    for (i = 0; i < p->d_inSize; i++) {
        post_system (NULL, PD_TRANSLATE ("rescan: midi in / %s"), symbol_getName (p->d_inNames[i]));
    }
    
    for (i = 0; i < p->d_outSize; i++) {
        post_system (NULL, PD_TRANSLATE ("rescan: midi out / %s"), symbol_getName (p->d_outNames[i]));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

juce::StringArray mididevices_getListIn (t_mididevices *p)
{
    juce::StringArray devices;
    
    int i;
    
    for (i = 0; i < p->d_inSize; i++) {
        devices.add (juce::String (symbol_getName (p->d_inNames[i])));
    }
    
    return devices;
}

juce::StringArray mididevices_getListOut (t_mididevices *p)
{
    juce::StringArray devices;
    
    int i;
    
    for (i = 0; i < p->d_outSize; i++) {
        devices.add (juce::String (symbol_getName (p->d_outNames[i])));
    }
    
    return devices;
}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
