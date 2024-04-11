
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "../m_headers.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

#if defined ( PD_BUILDING_APPLICATION )

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void outputs_quit (void)
{
    wrapper_send (Outputs::quit());
}

void outputs_clearConsole (void)
{
    wrapper_send (Outputs::clearConsole());
}

void outputs_reportDsp (int n)
{
    wrapper_send (Outputs::reportDsp (n ? true : false));
}

void outputs_patchLoadbang (t_glist *g)
{
    wrapper_send (Outputs::patchLoadbang (UniquePath (cast_object (g))));
}

void outputs_patchOpened (t_symbol *name, t_symbol *directory)
{
    jassert (name && directory);
    
    juce::File f (makeString (symbol_getName (directory)) + "/" + makeString (symbol_getName (name)));
    
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
        jassert (t);
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
        jassert (t);
        d.emplace_back (t->s_name, channels);
    }
    
    return d;
}

static std::vector<MidiDevice> outputs_getMidiIn (t_deviceslist *l)
{
    std::vector<MidiDevice> d;
    
    for (int j = 0; j < deviceslist_getInSize (l); ++j) {
        t_symbol *t  = deviceslist_getInAtIndex (l, j);
        jassert (t);
        d.emplace_back (t->s_name);
    }
    
    return d;
}

static std::vector<MidiDevice> outputs_getMidiOut (t_deviceslist *l)
{
    std::vector<MidiDevice> d;
    
    for (int j = 0; j < deviceslist_getOutSize (l); ++j) {
        t_symbol *t  = deviceslist_getOutAtIndex (l, j);
        jassert (t);
        d.emplace_back (t->s_name);
    }
    
    return d;
}

void outputs_reportAvailableAudioDevices (t_deviceslist *l)
{
    wrapper_send (Outputs::reportAvailableAudioDevices (outputs_getAudioIn (l), outputs_getAudioOut (l)));
}

void outputs_reportCurrentAudioDevices (t_deviceslist *l)
{
    wrapper_send (Outputs::reportCurrentAudioDevices (outputs_getAudioIn (l), outputs_getAudioOut (l)));
}

void outputs_reportAvailableMidiDevices (t_deviceslist *l)
{
    wrapper_send (Outputs::reportAvailableMidiDevices (outputs_getMidiIn (l), outputs_getMidiOut (l)));
}

void outputs_reportCurrentMidiDevices (t_deviceslist *l)
{
    wrapper_send (Outputs::reportCurrentMidiDevices (outputs_getMidiIn (l), outputs_getMidiOut (l)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void outputs_classNew (t_symbol *s)
{
    wrapper_send (Outputs::classNew (makeString (symbol_getName (s))));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void outputs_objectAdded (t_object *x)
{
    const UniquePath p (x); wrapper_send (Outputs::added (p, Report::object (p, x, Tags())));
}

void outputs_objectChanged (t_object *x, const Tags& t)
{
    const UniquePath p (x); wrapper_send (Outputs::changed (p, Report::object (p, x, t)));
}

void outputs_objectUpdated (t_object *x, const Tags& t)
{
    t_glist *g = object_isCanvas (x) ? cast_glist (x) : object_getOwner (x);
    
    outputs_objectChanged (x, t);
    
    jassert (g);
    
    glist_setDirty (g, 1);
}

void outputs_objectRemoved (t_object *x)
{
    wrapper_send (Outputs::removed (UniquePath (x)));
}

void outputs_objectRenamed (t_object *x, t_id t)
{
    wrapper_send (Outputs::renamed (UniquePath (x), t));
}

void outputs_lineAdded (t_id u, t_object *src, int m, t_object *dest, int n, t_glist *owner)
{
    const UniquePath p (u, owner);
    
    wrapper_send (Outputs::added (p, Report::lineAdded (p, src, m, dest, n)));
}

void outputs_lineChanged (t_id u, t_object *src, int m, t_object *dest, int n, t_glist *owner)
{
    const UniquePath p (u, owner);
    
    wrapper_send (Outputs::changed (p, Report::lineChanged (p, src, m, dest, n)));
}

void outputs_lineRemoved (t_id u, t_glist *owner)
{
    wrapper_send (Outputs::removed (UniquePath (u, owner)));
}

void outputs_patchOrder (t_glist *g, std::vector<UniqueId>&& ids)
{
    wrapper_send (Outputs::patchOrder (UniquePath (cast_object (g)), std::move (ids)));
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#else

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void main_exit (void);

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void outputs_quit (void)
{
    #if defined ( PD_BUILDING_TERMINAL )
    
    main_exit();
    
    #endif
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void outputs_clearConsole (void)
{
}

void outputs_reportDsp (int n)
{
}

void outputs_patchLoadbang (t_glist *g)
{
}

void outputs_patchOpened (t_symbol *name, t_symbol *directory)
{
}

void outputs_reportAvailableAudioDevices (t_deviceslist *l)
{
}

void outputs_reportCurrentAudioDevices (t_deviceslist *l)
{
}

void outputs_reportAvailableMidiDevices (t_deviceslist *l)
{
}

void outputs_reportCurrentMidiDevices (t_deviceslist *l)
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
