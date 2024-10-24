
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* It seems that ALSA doesn't support MIDI tick (0xf9). */

/* < https://mailman.alsa-project.org/pipermail/alsa-devel/2013-March/060221.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static int alsa_numberOfDevicesIn;                  /* Static. */
static int alsa_numberOfDevicesOut;                 /* Static. */

static int alsa_devicesIn[DEVICES_MAXIMUM_IO];      /* Static. */
static int alsa_devicesOut[DEVICES_MAXIMUM_IO];     /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static snd_seq_t            *alsa_handle;           /* Static. */
static snd_midi_event_t     *alsa_parser;           /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static uint8_t              *alsa_buffer;           /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#define ALSA_SYSEX_SIZE     256

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Fetch port number with port identifier. */

static int alsa_getPortIn (int identifier)
{
    int port = 0;
    
    for (port = 0; port < alsa_numberOfDevicesIn; port++) {
        if (alsa_devicesIn[port] == identifier) {
            return port;
        }
    }
    
    PD_BUG; return -1;
}

/* Fetch port identifier with port number. */

static int alsa_getIdentifierOut (int port)
{
    return alsa_devicesOut[port];
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

const char *midi_getBackendNameNative (void)
{
    static const char *name = "ALSA"; return name;      /* Static. */
}

void midi_initializeNative (void)
{
    alsa_buffer = (uint8_t *)PD_MEMORY_GET (sizeof (uint8_t) * ALSA_SYSEX_SIZE);
}

void midi_releaseNative (void)
{
    PD_MEMORY_FREE (alsa_buffer);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error midi_openNative (t_devices *p)
{
    int numberOfDevicesIn  = devices_getInSize (p);
    int numberOfDevicesOut = devices_getOutSize (p);
    
    alsa_numberOfDevicesIn  = 0;
    alsa_numberOfDevicesOut = 0;

    if (numberOfDevicesOut || numberOfDevicesIn) {
    //
    t_error err = PD_ERROR;
    
    if (numberOfDevicesIn > 0 && numberOfDevicesOut > 0) { 
        err = snd_seq_open (&alsa_handle, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK);
        
    } else if (numberOfDevicesIn > 0) {
        err = snd_seq_open (&alsa_handle, "default", SND_SEQ_OPEN_INPUT,  SND_SEQ_NONBLOCK);
        
    } else if (numberOfDevicesOut > 0) {
        err = snd_seq_open (&alsa_handle, "default", SND_SEQ_OPEN_OUTPUT, SND_SEQ_NONBLOCK);
    }
    
    err |= (snd_midi_event_new (12, &alsa_parser) != 0);

    if (err) { PD_BUG; }
    else {
    //
    int i;
    
    for (i = 0; i < numberOfDevicesIn; i++) {
        char portname[PD_STRING] = { 0 };
        err |= string_sprintf (portname, PD_STRING, PD_NAME " Midi-In %d", i + 1);
        if (!err) {
            int identifier = snd_seq_create_simple_port (alsa_handle,
                            portname,
                            SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE, 
                            SND_SEQ_PORT_TYPE_APPLICATION);
            if (!(err |= (identifier < 0))) {
                alsa_devicesIn[alsa_numberOfDevicesIn] = identifier; alsa_numberOfDevicesIn++;
            }
        }
    }

    for (i = 0; i < numberOfDevicesOut; i++) {
        char portname[PD_STRING] = { 0 };
        err |= string_sprintf (portname, PD_STRING, PD_NAME " Midi-Out %d", i + 1);
        if (!err) {
            int identifier = snd_seq_create_simple_port (alsa_handle,
                            portname,
                            SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ, 
                            SND_SEQ_PORT_TYPE_APPLICATION);
            if (!(err |= (identifier < 0))) {
                alsa_devicesOut[alsa_numberOfDevicesOut] = identifier; alsa_numberOfDevicesOut++;
            }
        }
    }
   
    if (err) { PD_BUG; }
    else {
        snd_seq_client_info_t *info = NULL;
        snd_seq_client_info_malloc (&info);
        snd_seq_get_client_info (alsa_handle, info);
        snd_seq_client_info_set_name (info, PD_NAME);
        snd_seq_set_client_info (alsa_handle, info);
        snd_seq_client_info_free (info);
    }
    //
    }
    
    return err;
    //
    }
    
    return PD_ERROR_NONE;
}

void midi_closeNative()
{
    alsa_numberOfDevicesIn  = 0;
    alsa_numberOfDevicesOut = 0;
    
    if (alsa_parser) { snd_midi_event_free (alsa_parser); }
    if (alsa_handle) { snd_seq_close (alsa_handle); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midi_pushNativeSystemEvent (snd_seq_event_t *e, int status, int a, int b)
{
    uint8_t data[3] = { 0 };
    
    data[0] = status;
    data[1] = 0x7f & a;
    data[2] = 0x7f & b;
    
    snd_midi_event_reset_encode (alsa_parser);
    
    switch (status) {
    //
    case MIDI_TIMECODE      : snd_midi_event_encode (alsa_parser, data, 2, e); break;
    case MIDI_SONGPOS       : snd_midi_event_encode (alsa_parser, data, 3, e); break;
    case MIDI_SONGSELECT    : snd_midi_event_encode (alsa_parser, data, 2, e); break;
    case MIDI_TUNEREQUEST   : /* Falls through. */
    case MIDI_CLOCK         : /* Falls through. */
    case MIDI_TICK          : /* Falls through. */
    case MIDI_START         : /* Falls through. */
    case MIDI_CONTINUE      : /* Falls through. */
    case MIDI_STOP          : /* Falls through. */
    case MIDI_ACTIVESENSE   : /* Falls through. */
    case MIDI_RESET         : snd_midi_event_encode (alsa_parser, data, 1, e); break;
    default                 : break;
    //
    }
}

static void midi_pushNativeEvent (snd_seq_event_t *e, int status, int a, int b)
{
    if (status >= MIDI_PITCHBEND)               {
        snd_seq_ev_set_pitchbend (e,   (status - MIDI_PITCHBEND),     (((b << 7) | a) - 8192));
    } else if (status >= MIDI_AFTERTOUCH)       {
        snd_seq_ev_set_chanpress (e,   (status - MIDI_AFTERTOUCH),    a);
    } else if (status >= MIDI_PROGRAMCHANGE)    {
        snd_seq_ev_set_pgmchange (e,   (status - MIDI_PROGRAMCHANGE), a);
    } else if (status >= MIDI_CONTROLCHANGE)    {
        snd_seq_ev_set_controller (e,  (status - MIDI_CONTROLCHANGE), a, b);
    } else if (status >= MIDI_POLYPRESSURE)     {
        snd_seq_ev_set_keypress (e,    (status - MIDI_POLYPRESSURE),  a, b);
    } else if (status >= MIDI_NOTEON)           {
        if (b) {
            snd_seq_ev_set_noteon (e,  (status - MIDI_NOTEON),        a, b);
        } else {
            snd_seq_ev_set_noteoff (e, (status - MIDI_NOTEON),        a, 0);
        }
    } else {
        snd_seq_ev_set_noteoff (e, (status - MIDI_NOTEOFF),           a, 0);
    }
}

void midi_pushNative (int port, int status, int a, int b)
{
    snd_seq_event_t e; snd_seq_ev_clear (&e);
    
    if (port >= 0 && port < alsa_numberOfDevicesOut) {
    //
    if (MIDI_IS_COMMON (status) || MIDI_IS_REALTIME (status)) {
        midi_pushNativeSystemEvent (&e, status, a, b);
    } else if (MIDI_IS_VOICE (status)) {
        midi_pushNativeEvent (&e, status, a, b);
    } else {
        PD_BUG; return;
    }
    
    snd_seq_ev_set_direct (&e);
    snd_seq_ev_set_subs (&e);
    snd_seq_ev_set_source (&e, alsa_getIdentifierOut (port));
    snd_seq_event_output_direct (alsa_handle, &e);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midi_pushSysexNativeEvent (int port, int argc, t_atom *argv, uint8_t *buffer)
{
    snd_seq_event_t e; snd_seq_ev_clear (&e);
    
    int i; for (i = 0; i < argc; i++) { buffer[i] = (uint8_t)atom_getFloat (argv + i); }
    
    snd_seq_ev_set_sysex (&e, argc, buffer);
    snd_seq_ev_set_direct (&e);
    snd_seq_ev_set_subs (&e);
    snd_seq_ev_set_source (&e, alsa_getIdentifierOut (port));
    snd_seq_event_output_direct (alsa_handle, &e);
}

static void midi_pushSysexNativeAllocate (int port, int argc, t_atom *argv)
{
    uint8_t *allocated = (uint8_t *)PD_MEMORY_GET (sizeof (uint8_t) * argc);
    
    midi_pushSysexNativeEvent (port, argc, argv, allocated);
    
    PD_MEMORY_FREE (allocated);
}

void midi_pushSysexNative (int port, int argc, t_atom *argv)
{
    if (port >= 0 && port < alsa_numberOfDevicesOut) {
    //
    if (argc > ALSA_SYSEX_SIZE) { midi_pushSysexNativeAllocate (port, argc, argv); }
    else {
    //
    midi_pushSysexNativeEvent (port, argc, argv, alsa_buffer);
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midi_pollNativeEventSysex (int port, snd_seq_event_t *e)
{
    uint32_t size = e->data.ext.len;
    uint8_t *data = (uint8_t *)e->data.ext.ptr;
    
    if (size) { uint32_t i; for (i = 0; i < size; i++) { midi_receiveSysex (port, data[i]); } }
}

static void midi_pollNativeEvent (int port, snd_seq_event_t *e)
{
    if (port >= 0 && port < alsa_numberOfDevicesIn) {
    //
    int status = 0;
    
    switch (e->type) {
    //
    case SND_SEQ_EVENT_NOTEOFF      : status = e->data.note.channel    + MIDI_NOTEOFF;          break;
    case SND_SEQ_EVENT_NOTEON       : status = e->data.note.channel    + MIDI_NOTEON;           break;
    case SND_SEQ_EVENT_KEYPRESS     : status = e->data.note.channel    + MIDI_POLYPRESSURE;     break;
    case SND_SEQ_EVENT_CONTROLLER   : status = e->data.control.channel + MIDI_CONTROLCHANGE;    break;
    case SND_SEQ_EVENT_PGMCHANGE    : status = e->data.control.channel + MIDI_PROGRAMCHANGE;    break;
    case SND_SEQ_EVENT_CHANPRESS    : status = e->data.control.channel + MIDI_AFTERTOUCH;       break;
    case SND_SEQ_EVENT_PITCHBEND    : status = e->data.control.channel + MIDI_PITCHBEND;        break;
    case SND_SEQ_EVENT_QFRAME       : status = MIDI_TIMECODE;                                   break;
    case SND_SEQ_EVENT_SONGPOS      : status = MIDI_SONGPOS;                                    break;
    case SND_SEQ_EVENT_SONGSEL      : status = MIDI_SONGSELECT;                                 break;
    case SND_SEQ_EVENT_TUNE_REQUEST : status = MIDI_TUNEREQUEST;                                break;
    case SND_SEQ_EVENT_CLOCK        : status = MIDI_CLOCK;                                      break;
    case SND_SEQ_EVENT_TICK         : status = MIDI_TICK;                                       break;
    case SND_SEQ_EVENT_START        : status = MIDI_START;                                      break;
    case SND_SEQ_EVENT_CONTINUE     : status = MIDI_CONTINUE;                                   break;
    case SND_SEQ_EVENT_STOP         : status = MIDI_STOP;                                       break;
    case SND_SEQ_EVENT_SENSING      : status = MIDI_ACTIVESENSE;                                break;
    case SND_SEQ_EVENT_RESET        : status = MIDI_RESET;                                      break;
    default : break;
    //
    }

    if (status) {
    //
    int a = 0;
    int b = 0;
    
    switch (e->type) {
    //
    case SND_SEQ_EVENT_NOTEOFF      :       /* Falls through. */
    case SND_SEQ_EVENT_NOTEON       :       /* Falls through. */
    case SND_SEQ_EVENT_KEYPRESS     :
        a = e->data.note.note;
        b = e->data.note.velocity;
        break;
    case SND_SEQ_EVENT_CONTROLLER   :
        a = e->data.control.param;
        b = e->data.control.value;
        break;
    case SND_SEQ_EVENT_PGMCHANGE    :       /* Falls through. */
    case SND_SEQ_EVENT_CHANPRESS    :
        a = e->data.control.value;
        break;
        
    case SND_SEQ_EVENT_PITCHBEND    :
        a = 0x7f & ((e->data.control.value + 8192));
        b = 0x7f & ((e->data.control.value + 8192) >> 7);
        break;
    
    case SND_SEQ_EVENT_QFRAME       :
        a = e->data.control.value;
        break;
    case SND_SEQ_EVENT_SONGPOS      :
        a = 0x7f & (e->data.control.value);
        b = 0x7f & (e->data.control.value >> 7);
        break;
    case SND_SEQ_EVENT_SONGSEL      :
        a = e->data.control.value;
        break;
    
    default : break;
    //
    }
    
    midi_receive (port, status, a, b);
    //
    }
    //
    }
}

void midi_pollNative (void)
{
    if (alsa_numberOfDevicesIn) {
    //
    while (snd_seq_event_input_pending (alsa_handle, 1) > 0) {
    //
    snd_seq_event_t *e = NULL;
    
    if (snd_seq_event_input (alsa_handle, &e) > 0) {
        if (e->type == SND_SEQ_EVENT_SYSEX) {
            midi_pollNativeEventSysex (alsa_getPortIn (e->dest.port), e);
        } else {
            midi_pollNativeEvent (alsa_getPortIn (e->dest.port), e);
        }
    } else {
        break;
    }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error midi_getListOfDevicesNative (t_mididevices *p)
{
    int i;
    int m = PD_MIN (4, DEVICES_MAXIMUM_IO);
    int n = PD_MIN (4, DEVICES_MAXIMUM_IO);
    
    t_error err = PD_ERROR_NONE;
    
    for (i = 0; i < m; i++) { err |= mididevices_appendMidiIn (p, gensym ("ALSA virtual device"));  }
    for (i = 0; i < n; i++) { err |= mididevices_appendMidiOut (p, gensym ("ALSA virtual device")); }
  
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
