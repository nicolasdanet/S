
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#ifndef CORE_MIDI_H_
#define CORE_MIDI_H_

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "core_audio.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#define coremidi_error(err) core_midiCheck ((err), __LINE__)

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static inline t_error core_midiCheck (OSStatus error, int line)
{
    t_error err = (error != noErr);
    
    #if PD_WITH_DEBUG
    
    if (err) {
    //
    post_log ("*** Core MIDI / line %d", line);
    //
    }
    
    #endif // PD_WITH_DEBUG
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

typedef struct _midiport {
    MIDIEndpointRef     mp_endpoint;
    MIDIPortRef         mp_port;
    int                 mp_hasConnect;
    t_fifo32            *mp_buffer;
    t_fifo8             *mp_bufferSysex;
    } t_midiport;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_symbol *midiname_get (MIDIEndpointRef endpoint);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

MIDIClientRef      coreclient_get                  (void);

void       coreclient_initialize                   (void);
void       coreclient_release                      (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    mididevicelist_get                      (t_deviceslist *list);

MIDIEndpointRef    mididevicelist_fetchSource      (t_symbol *name);
MIDIEndpointRef    mididevicelist_fetchDestination (t_symbol *name);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error    midiport_openInput                      (t_midiport *midiport, t_symbol *name);
t_error    midiport_openOutput                     (t_midiport *midiport, t_symbol *name);

void       midiport_close                          (t_midiport *midiport);

void       midiport_pushSysex                      (t_midiport *midiport, int argc, t_atom *argv);
void       midiport_push                           (t_midiport *midiport, int status, int a, int b);
void       midiport_poll                           (t_midiport *midiport, int port);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
#endif // CORE_MIDI_H_