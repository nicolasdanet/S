
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#include "../../m_spaghettis.h"
#include "../../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_LOCAL void inmidi_noteOn (int port, int channel, int pitch, int velocity)
{
    if (symbol_hasThingQuiet (sym__notein)) {
    //
    t_atom a[3];
    
    SET_FLOAT (a + 0, pitch);
    SET_FLOAT (a + 1, velocity);
    SET_FLOAT (a + 2, channel + 1 + (port << 4));
    
    pd_list (symbol_getThing (sym__notein), 3, a);
    //
    }
}

PD_LOCAL void inmidi_polyPressure (int port, int channel, int pitch, int value)
{
    if (symbol_hasThingQuiet (sym__polytouchin)) {
    //
    t_atom a[3];
    
    SET_FLOAT (a + 0, pitch);
    SET_FLOAT (a + 1, value);
    SET_FLOAT (a + 2, channel + 1 + (port << 4));
    
    pd_list (symbol_getThing (sym__polytouchin), 3, a);
    //
    }
}

PD_LOCAL void inmidi_controlChange (int port, int channel, int control, int value)
{
    if (symbol_hasThingQuiet (sym__ctlin)) {
    //
    t_atom a[3];
    
    SET_FLOAT (a + 0, control);
    SET_FLOAT (a + 1, value);
    SET_FLOAT (a + 2, channel + 1 + (port << 4));
    
    pd_list (symbol_getThing (sym__ctlin), 3, a);
    //
    }
}

PD_LOCAL void inmidi_programChange (int port, int channel, int value)
{
    if (symbol_hasThingQuiet (sym__pgmin)) {
    //
    t_atom a[2];
    
    SET_FLOAT (a + 0, value + 1);
    SET_FLOAT (a + 1, channel + 1 + (port << 4));
    
    pd_list (symbol_getThing (sym__pgmin), 2, a);
    //
    }
}

PD_LOCAL void inmidi_afterTouch (int port, int channel, int value)
{
    if (symbol_hasThingQuiet (sym__touchin)) {
    //
    t_atom a[2];
    
    SET_FLOAT (a + 0, value);
    SET_FLOAT (a + 1, channel + 1 + (port << 4));
    
    pd_list (symbol_getThing (sym__touchin), 2, a);
    //
    }
}

PD_LOCAL void inmidi_pitchBend (int port, int channel, int value)
{
    if (symbol_hasThingQuiet (sym__bendin)) {
    //
    t_atom a[2];
    
    SET_FLOAT (a + 0, value);
    SET_FLOAT (a + 1, channel + 1 + (port << 4));
    
    pd_list (symbol_getThing (sym__bendin), 2, a);
    //
    }
}

PD_LOCAL void inmidi_sysex (int port, int argc, t_atom *argv)
{
    if (symbol_hasThingQuiet (sym__sysexin)) {
    if (symbol_hasThingQuiet (sym__sysexportin)) {
    //
    pd_float (symbol_getThing (sym__sysexportin), (t_float)port);
    pd_list (symbol_getThing (sym__sysexin), argc, argv);
    //
    }
    }
}

PD_LOCAL void inmidi_system (int port, int argc, t_atom *argv)
{
    if (symbol_hasThingQuiet (sym__midisystemin)) {
    if (symbol_hasThingQuiet (sym__midisystemportin)) {
    //
    pd_float (symbol_getThing (sym__midisystemportin), (t_float)port);
    pd_list (symbol_getThing (sym__midisystemin), argc, argv);
    //
    }
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
