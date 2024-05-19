
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "core_midi.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midiport_init (t_midiport *midiport)
{
    midiport->mp_endpoint    = 0;
    midiport->mp_port        = 0;
    midiport->mp_hasConnect  = 0;
    midiport->mp_buffer      = NULL;
    midiport->mp_bufferSysex = NULL;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midiport_inputProceedSysex (t_midiport *midiport, MIDIPacket *packet)
{
    if (midiport->mp_bufferSysex) {
    //
    int32_t size = packet->length;
    Byte *data   = packet->data;
    
    if (size && ringbuffer_getAvailableWrite (midiport->mp_bufferSysex) >= size) {
        ringbuffer_write (midiport->mp_bufferSysex, data, size);
    } else {
        PD_ABORT (1);   /* Do something less disruptive? */
    }
    //
    }
}

static void midiport_inputProceed (t_midiport *midiport, MIDIPacket *packet)
{
    if (midiport->mp_buffer) {
    //
    if (fifo32_getAvailableWrite (midiport->mp_buffer) > 0) {
    //
    uint32_t status = packet->data[0];
    uint32_t a      = (packet->length > 1) ? packet->data[1] : 0;
    uint32_t b      = (packet->length > 2) ? packet->data[2] : 0;
    
    uint32_t data   = (status << 16) | (a << 8) | (b << 0);
    
    fifo32_write (midiport->mp_buffer, &data, 1);
    //
    } else {
        PD_ABORT (1);   /* Do something less disruptive? */
    }
    //
    }
}

static void midiport_inputCallback (const MIDIPacketList *pktlist,
    void *readProcRefCon,
    void *srcConnRefCon)
{
    t_midiport *midiport = (t_midiport *)readProcRefCon;
    
    UInt16 i; MIDIPacket *packet = (MIDIPacket *)pktlist->packet;

    for (i = 0; i < pktlist->numPackets; i++) {
    //
    if (packet->length) {
    //
    uint32_t status = packet->data[0];

    if (MIDI_IS_VOICE (status) || MIDI_IS_COMMON (status)) {
        midiport_inputProceed (midiport, packet);
    } else if (MIDI_IS_REALTIME (status) || MIDI_IS_SYSEX (status) || !MIDI_IS_STATUS (status)) {
        midiport_inputProceedSysex (midiport, packet);
    } else { PD_BUG; }
    //
    } else { PD_BUG; }
    
    packet = MIDIPacketNext (packet);
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

t_error midiport_openInput (t_midiport *midiport, t_symbol *name)
{
    midiport_init (midiport);
    
    t_error err = PD_ERROR_NONE;
    
    midiport->mp_endpoint = mididevicelist_fetchSource (name);
    
    if (!midiport->mp_endpoint) { return PD_ERROR; }
    else {
    //
    MIDIPortRef t = 0;
    
    err |= coremidi_error (MIDIInputPortCreate (coreclient_get(),
                                CFSTR ("MIDI Input"),
                                midiport_inputCallback,
                                midiport,
                                &t));
    
    if (!err) {
    //
    midiport->mp_port = t;
    
    err |= coremidi_error (MIDIPortConnectSource (midiport->mp_port, midiport->mp_endpoint, NULL));
    
    if (!err) {
        midiport->mp_hasConnect  = 1;
        midiport->mp_buffer      = fifo32_new();
        midiport->mp_bufferSysex = ringbuffer_new (sizeof (Byte),     (65536 * 2));
    }
    //
    }
    //
    }
    
    return err;
}

t_error midiport_openOutput (t_midiport *midiport, t_symbol *name)
{
    midiport_init (midiport);
    
    t_error err = PD_ERROR_NONE;
    
    midiport->mp_endpoint = mididevicelist_fetchDestination (name);
    
    if (!midiport->mp_endpoint) { return PD_ERROR; }
    else {
    //
    MIDIPortRef t = 0;
    
    err |= coremidi_error (MIDIOutputPortCreate (coreclient_get(),
                                CFSTR ("MIDI Output"),
                                &t));
    
    if (!err) {
    //
    midiport->mp_port = t;
    //
    }
    //
    }
    
    return err;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void midiport_close (t_midiport *midiport)
{
    if (midiport->mp_bufferSysex) { ringbuffer_free (midiport->mp_bufferSysex); }
    if (midiport->mp_buffer)      { fifo32_free (midiport->mp_buffer); }
    if (midiport->mp_hasConnect)  {
    //
    t_error err = coremidi_error (MIDIPortDisconnectSource (midiport->mp_port, midiport->mp_endpoint));
    
    PD_ASSERT (!err); PD_UNUSED (err);
    //
    }
    
    if (midiport->mp_port) { MIDIPortDispose (midiport->mp_port); }
    
    midiport_init (midiport);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Largely stolen from JUCE (assume that 32 bytes is enough size for packet's headers). */

void midiport_pushSysex (t_midiport *midiport, int argc, t_atom *argv)
{
    const int maximumPacketSize = 256;
    const int numberOfPackets   = (argc + maximumPacketSize - 1) / maximumPacketSize;
    Byte *allocatedBuffer       = (Byte *)PD_MEMORY_GET ((32 * numberOfPackets) + argc);
    MIDIPacketList *packetList  = (MIDIPacketList *)allocatedBuffer;
    MIDITimeStamp timeStamp     = AudioGetCurrentHostTime();
    
    packetList->numPackets = (UInt32)numberOfPackets;
    
    {
    //
    MIDIPacket *p = packetList->packet;
    
    int bytesLeft = argc;
    int i, j, n = 0;
    
    for (i = 0; i < numberOfPackets; i++) {
        p->timeStamp = timeStamp;
        p->length    = (UInt16)PD_MIN (maximumPacketSize, bytesLeft);
        for (j = 0; j < p->length; j++) { p->data[j] = (Byte)atom_getFloat (argv + n + j); }
        n += p->length;
        bytesLeft -= p->length;
        p = MIDIPacketNext (p);
    }
    //
    }
    
    MIDISend (midiport->mp_port, midiport->mp_endpoint, packetList);
    
    PD_MEMORY_FREE (allocatedBuffer);
}

void midiport_push (t_midiport *midiport, int status, int a, int b)
{
    if (midiport->mp_port) {
    //
    MIDIPacketList packetList;
    
    packetList.numPackets          = 1;
    packetList.packet[0].length    = 3;
    packetList.packet[0].data[0]   = status;
    packetList.packet[0].data[1]   = a;
    packetList.packet[0].data[2]   = b;
    packetList.packet[0].timeStamp = AudioGetCurrentHostTime();
    
    MIDISend (midiport->mp_port, midiport->mp_endpoint, &packetList);
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static void midiport_pollProceedSysex (t_midiport *midiport, int port)
{
    if (midiport->mp_bufferSysex) {
    //
    while (ringbuffer_getAvailableRead (midiport->mp_bufferSysex) > 0) {
    //
    Byte byte = 0; ringbuffer_read (midiport->mp_bufferSysex, &byte, 1);
    
    midi_receiveSysex (port, byte);
    //
    }
    //
    }
}

static void midiport_pollProceed (t_midiport *midiport, int port)
{
    if (midiport->mp_buffer) {
    //
    while (fifo32_getAvailableRead (midiport->mp_buffer) > 0) {
    //
    uint32_t status, a, b, data = 0;
    
    fifo32_read (midiport->mp_buffer, &data, 1);
    
    status = 0xff & (data >> 16);
    a      = 0xff & (data >> 8);
    b      = 0xff & (data >> 0);
    
    midi_receive (port, status, a, b);
    //
    }
    //
    }
}

void midiport_poll (t_midiport *midiport, int port)
{
    midiport_pollProceedSysex (midiport, port);
    midiport_pollProceed (midiport, port);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
