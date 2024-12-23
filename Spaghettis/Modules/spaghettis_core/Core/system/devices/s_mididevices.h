
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef S_MIDIDEVICES_H_
#define S_MIDIDEVICES_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _mididevices {
    int         d_inSize;
    int         d_outSize;
    t_symbol    *d_inNames  [DEVICES_MAXIMUM_IO];
    t_symbol    *d_outNames [DEVICES_MAXIMUM_IO];
    } t_mididevices;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        mididevices_initialize      (t_mididevices *p);
void        mididevices_copy            (t_mididevices *dest, t_mididevices *src);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error     mididevices_appendMidiIn    (t_mididevices *p, t_symbol *device);
t_error     mididevices_appendMidiOut   (t_mididevices *p, t_symbol *device);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

int         mididevices_hasMidiIn       (t_mididevices *p, t_symbol *device);
int         mididevices_hasMidiOut      (t_mididevices *p, t_symbol *device);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol    *mididevices_getDefaultIn   (t_mididevices *p);
t_symbol    *mididevices_getDefaultOut  (t_mididevices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void        mididevices_report          (t_mididevices *p);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

juce::StringArray mididevices_getListIn     (t_mididevices *p);
juce::StringArray mididevices_getListOut    (t_mididevices *p);

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // S_MIDIDEVICES_H_
