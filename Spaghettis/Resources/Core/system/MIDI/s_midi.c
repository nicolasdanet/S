
/* Copyright (c) 1997-2020 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static t_buffer *midi_sysex[DEVICES_MAXIMUM_IO];    /* Static. */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL t_error midi_getStatusWithSymbol (t_symbol *s, int *status)
{
    int n = 0;
    
    if (s == sym_timecode)          { n = MIDI_TIMECODE;     }
    else if (s == sym_songposition) { n = MIDI_SONGPOS;      }
    else if (s == sym_songselect)   { n = MIDI_SONGSELECT;   }
    else if (s == sym_tunerequest)  { n = MIDI_TUNEREQUEST;  }
    else if (s == sym_clock)        { n = MIDI_CLOCK;        }
    else if (s == sym_tick)         { n = MIDI_TICK;         }
    else if (s == sym_start)        { n = MIDI_START;        }
    else if (s == sym_continue)     { n = MIDI_CONTINUE;     }
    else if (s == sym_stop)         { n = MIDI_STOP;         }
    else if (s == sym_activesense)  { n = MIDI_ACTIVESENSE;  }
    else if (s == sym_reset)        { n = MIDI_RESET;        }

    (*status) = n;
    
    return (n == 0) ? PD_ERROR : PD_ERROR_NONE;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midi_receiveProceedSystem (int port, int status, int a, int b)
{
    t_atom m[3]; int size = 0;
    
    SET_FLOAT (m + 1, a);
    SET_FLOAT (m + 2, b);
    
    switch (status) {
    //
    case MIDI_TIMECODE      : size = 2; SET_SYMBOL (m, sym_timecode);               break;
    case MIDI_SONGPOS       : size = 3; SET_SYMBOL (m, sym_songposition);           break;
    case MIDI_SONGSELECT    : size = 2; SET_SYMBOL (m, sym_songselect);             break;
    case MIDI_RESERVED1     : break;
    case MIDI_RESERVED2     : break;
    case MIDI_TUNEREQUEST   : size = 1; SET_SYMBOL (m, sym_tunerequest);            break;
    case MIDI_CLOCK         : size = 1; SET_SYMBOL (m, sym_clock);                  break;
    case MIDI_TICK          : size = 1; SET_SYMBOL (m, sym_tick);                   break;
    case MIDI_START         : size = 1; SET_SYMBOL (m, sym_start);                  break;
    case MIDI_CONTINUE      : size = 1; SET_SYMBOL (m, sym_continue);               break;
    case MIDI_STOP          : size = 1; SET_SYMBOL (m, sym_stop);                   break;
    case MIDI_RESERVED3     : break;
    case MIDI_ACTIVESENSE   : size = 1; SET_SYMBOL (m, sym_activesense);            break;
    case MIDI_RESET         : size = 1; SET_SYMBOL (m, sym_reset);                  break;
    default                 : break;
    //
    }
    
    if (size) { inmidi_system (port, size, m); }
}

static void midi_receiveProceedVoice (int port, int status, int a, int b)
{
    int command = status & 0xf0;
    int channel = status & 0x0f;

    switch (command) {
    //
    case MIDI_NOTEOFF       : inmidi_noteOn (port, channel, a, 0);                  break;
    case MIDI_NOTEON        : inmidi_noteOn (port, channel, a, b);                  break;
    case MIDI_POLYPRESSURE  : inmidi_polyPressure (port, channel, a, b);            break;
    case MIDI_CONTROLCHANGE : inmidi_controlChange (port, channel, a, b);           break;
    case MIDI_PROGRAMCHANGE : inmidi_programChange (port, channel, a);              break;
    case MIDI_AFTERTOUCH    : inmidi_afterTouch (port, channel, a);                 break;
    case MIDI_PITCHBEND     : inmidi_pitchBend (port, channel, ((b << 7) | a));     break;
    default                 : break;
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void midi_receive (int port, int status, int a, int b)
{
    if (MIDI_IS_VOICE (status))         { midi_receiveProceedVoice (port, status, a, b);  }
    else if (MIDI_IS_REALTIME (status)) { midi_receiveProceedSystem (port, status, a, b); }
    else if (MIDI_IS_COMMON (status))   { midi_receiveProceedSystem (port, status, a, b); }
}

PD_LOCAL void midi_receiveSysex (int port, uint8_t byte)
{
    if (MIDI_IS_REALTIME (byte)) { midi_receive (port, byte, 0, 0); }
    else {
    //
    if (MIDI_IS_SYSEX (byte)) {

        if (byte == MIDI_ENDSYSEX) {
            inmidi_sysex (port, buffer_getSize (midi_sysex[port]), buffer_getAtoms (midi_sysex[port]));
        }
        
        buffer_clear (midi_sysex[port]);

    } else {
    
        PD_ASSERT (!MIDI_IS_STATUS (byte)); buffer_appendFloat (midi_sysex[port], (t_float)byte);
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void midi_send (int port, int status, int a, int b)
{
    midi_pushNative (port, status, a, b);
}

PD_LOCAL void midi_sendSysex (int port, int argc, t_atom *argv)
{
    if (argc) { midi_pushSysexNative (port, argc, argv); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void midi_poll (void)
{
    midi_pollNative();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_LOCAL void midi_initialize (void)
{
    int i; for (i = 0; i < DEVICES_MAXIMUM_IO; i++) { midi_sysex[i] = buffer_new(); }
    
    midi_initializeNative();
}

PD_LOCAL void midi_release (void)
{
    midi_releaseNative();
    
    int i; for (i = 0; i < DEVICES_MAXIMUM_IO; i++) { buffer_free (midi_sysex[i]); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
