
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

void outputs_reportDsp (int n)
{
    wrapper_send (Outputs::reportDsp (n ? true : false));
}

void outputs_patchOpened (t_symbol *name, t_symbol *directory)
{
    jassert (name && directory);
    
    juce::File f (juce::String (symbol_getName (directory)) + "/" + juce::String (symbol_getName (name)));
    
    wrapper_send (Outputs::patchOpened (f));
}

void outputs_reportAvailableAudioDevices (t_deviceslist *l)
{
    std::vector<AudioDevice> i, o;
        
    for (int j = 0; j < deviceslist_getInSize (l); ++j) {
        t_symbol *t  = deviceslist_getInAtIndex (l, j);
        int channels = deviceslist_getInChannelsAtIndex (l, j);
        PD_ASSERT (t);
        i.emplace_back (t->s_name, channels);
    }
    
    for (int j = 0; j < deviceslist_getOutSize (l); ++j) {
        t_symbol *t  = deviceslist_getOutAtIndex (l, j);
        int channels = deviceslist_getOutChannelsAtIndex (l, j);
        PD_ASSERT (t);
        o.emplace_back (t->s_name, channels);
    }
    
    wrapper_send (Outputs::reportAvailableAudioDevices (i, o));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void outputs_reportDsp                      (int n)                                 { }
void outputs_patchOpened                    (t_symbol *name, t_symbol *directory)   { }
void outputs_reportAvailableAudioDevices    (t_deviceslist *l)                      { }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_BUILDING_APPLICATION

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
