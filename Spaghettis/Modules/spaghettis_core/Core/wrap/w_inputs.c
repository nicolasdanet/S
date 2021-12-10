
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

PD_LOCAL void inputs_ping (void)
{
    post_system (NULL, "?");
}

PD_LOCAL void inputs_newPatch (const juce::File& f)
{
    if (!f.existsAsFile()) {
        t_symbol *name = gensym (f.getFileName().toRawUTF8());
        t_symbol *directory = gensym (f.getParentDirectory().getFullPathName().toRawUTF8());
        instance_patchNew (name, directory);
    }
}

PD_LOCAL void inputs_openPatch (const juce::File& f)
{
    if (f.existsAsFile()) {
        t_symbol *name = gensym (f.getFileName().toRawUTF8());
        t_symbol *directory = gensym (f.getParentDirectory().getFullPathName().toRawUTF8());
        instance_patchOpen (name, directory);
    }
}

PD_LOCAL void inputs_switchDsp (void)
{
    dsp_setState (!dsp_getState());
}

PD_LOCAL void inputs_rescanSearchPaths (int logged)
{
    searchpath_rescan (logged);
}

PD_LOCAL void inputs_setSearchPaths (const juce::StringArray& paths)
{
    searchpath_clear(); for (const auto& p : paths) { searchpath_appendRoot (p.toRawUTF8()); }
}

PD_LOCAL void inputs_setSnap (bool snapToGrid)
{
    instance_snapSet (static_cast<int> (snapToGrid == true));
}

PD_LOCAL void inputs_setSnapSize (int gridSize)
{
    instance_snapSetGrid (gridSize);
}

PD_LOCAL void inputs_rescanDevices (void)
{
    audio_rescanDevices();
    midi_rescanDevices();
}

PD_LOCAL void inputs_setAudioDevices (const std::vector<AudioDevice>& i, const std::vector<AudioDevice>& o)
{
    t_devices audio; devices_initAsAudio (&audio);
    
    jassert (i.size() <= DEVICES_MAXIMUM_IO);
    jassert (o.size() <= DEVICES_MAXIMUM_IO);
    
    for (const auto& d : i) { devices_appendAudioIn (&audio, d);  }
    for (const auto& d : o) { devices_appendAudioOut (&audio, d); }
    
    audio_close();
    audio_setDevices (&audio, 0);
    settings_save();
}

PD_LOCAL void inputs_setMidiDevices (const std::vector<MidiDevice>& i, const std::vector<MidiDevice>& o)
{
    t_devices midi; devices_initAsMidi (&midi);
    
    jassert (i.size() <= DEVICES_MAXIMUM_IO);
    jassert (o.size() <= DEVICES_MAXIMUM_IO);
    
    for (const auto& d : i) { devices_appendMidiIn (&midi, d);  }
    for (const auto& d : o) { devices_appendMidiOut (&midi, d); }
    
    midi_close();
    midi_setDevices (&midi, 0);
    midi_open();
    settings_save();
}

PD_LOCAL void inputs_closePatch (core::Unique::Identifier i, int save)
{
    t_object *o = instance_registerGetObject (i);
    
    if (object_isCanvas (o)) {
    //
    t_glist *g = cast_glist (o); jassert (glist_isRoot (g));
    
    if (save) { glist_save (g); }
    
    glist_close (g);
    //
    } else { jassertfalse; }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif // PD_BUILDING_APPLICATION

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
