
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_spaghettis.h"
#include "../m_core.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

PD_FORCE void outputs_quit (void)
{
    wrapper_send (Outputs::quit());
}

PD_LOCAL void outputs_clearConsole (void)
{
    wrapper_send (Outputs::clearConsole());
}

PD_LOCAL void outputs_reportDsp (int n)
{
    wrapper_send (Outputs::reportDsp (n ? true : false));
}

PD_LOCAL void outputs_patchOpened (t_symbol *name, t_symbol *directory)
{
    jassert (name && directory);
    
    juce::File f (juce::String (symbol_getName (directory)) + "/" + juce::String (symbol_getName (name)));
    
    wrapper_send (Outputs::patchOpened (f));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static std::vector<AudioDevice> outputs_getAudioIn (t_deviceslist *l)
{
    std::vector<AudioDevice> d;
    
    for (int j = 0; j < deviceslist_getInSize (l); ++j) {
        t_symbol *t  = deviceslist_getInAtIndex (l, j);
        int channels = deviceslist_getInChannelsAtIndex (l, j);
        PD_ASSERT (t);
        d.emplace_back (t->s_name, channels);
    }
    
    return d;
}

static std::vector<AudioDevice> outputs_getAudioOut (t_deviceslist *l)
{
    std::vector<AudioDevice> d;
    
    for (int j = 0; j < deviceslist_getOutSize (l); ++j) {
        t_symbol *t  = deviceslist_getOutAtIndex (l, j);
        int channels = deviceslist_getOutChannelsAtIndex (l, j);
        PD_ASSERT (t);
        d.emplace_back (t->s_name, channels);
    }
    
    return d;
}

static std::vector<MidiDevice> outputs_getMidiIn (t_deviceslist *l)
{
    std::vector<MidiDevice> d;
    
    for (int j = 0; j < deviceslist_getInSize (l); ++j) {
        t_symbol *t  = deviceslist_getInAtIndex (l, j);
        PD_ASSERT (t);
        d.emplace_back (t->s_name);
    }
    
    return d;
}

static std::vector<MidiDevice> outputs_getMidiOut (t_deviceslist *l)
{
    std::vector<MidiDevice> d;
    
    for (int j = 0; j < deviceslist_getOutSize (l); ++j) {
        t_symbol *t  = deviceslist_getOutAtIndex (l, j);
        PD_ASSERT (t);
        d.emplace_back (t->s_name);
    }
    
    return d;
}

PD_LOCAL void outputs_reportAvailableAudioDevices (t_deviceslist *l)
{
    wrapper_send (Outputs::reportAvailableAudioDevices (outputs_getAudioIn (l), outputs_getAudioOut (l)));
}

PD_LOCAL void outputs_reportCurrentAudioDevices (t_deviceslist *l)
{
    wrapper_send (Outputs::reportCurrentAudioDevices (outputs_getAudioIn (l), outputs_getAudioOut (l)));
}

PD_LOCAL void outputs_reportAvailableMidiDevices (t_deviceslist *l)
{
    wrapper_send (Outputs::reportAvailableMidiDevices (outputs_getMidiIn (l), outputs_getMidiOut (l)));
}

PD_LOCAL void outputs_reportCurrentMidiDevices (t_deviceslist *l)
{
    wrapper_send (Outputs::reportCurrentMidiDevices (outputs_getMidiIn (l), outputs_getMidiOut (l)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

PD_FORCE void outputs_quit                          (void)                                  { }
PD_LOCAL void outputs_clearConsole                  (void)                                  { }
PD_LOCAL void outputs_reportDsp                     (int n)                                 { }
PD_LOCAL void outputs_patchOpened                   (t_symbol *name, t_symbol *directory)   { }
PD_LOCAL void outputs_reportAvailableAudioDevices   (t_deviceslist *l)                      { }
PD_LOCAL void outputs_reportCurrentAudioDevices     (t_deviceslist *l)                      { }
PD_LOCAL void outputs_reportAvailableMidiDevices    (t_deviceslist *l)                      { }
PD_LOCAL void outputs_reportCurrentMidiDevices      (t_deviceslist *l)                      { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_BUILDING_APPLICATION

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
