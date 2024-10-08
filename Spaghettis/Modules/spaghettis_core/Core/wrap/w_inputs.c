
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

void inputs_ping (void)
{
    post_system (NULL, "?");
}

void inputs_newPatch (const juce::File& f)
{
    if (!f.existsAsFile()) {
        t_symbol *name = gensym (f.getFileName().toRawUTF8());
        t_symbol *directory = gensym (f.getParentDirectory().getFullPathName().toRawUTF8());
        instance_patchNew (name, directory);
    }
}

void inputs_openPatch (const juce::File& f)
{
    if (f.existsAsFile()) {
        t_symbol *name = gensym (f.getFileName().toRawUTF8());
        t_symbol *directory = gensym (f.getParentDirectory().getFullPathName().toRawUTF8());
        instance_patchOpen (name, directory);
    } else {
        error_doesNotExist (NULL, f.getFullPathName().toRawUTF8());
    }
}

void inputs_switchDsp (void)
{
    dsp_setState (!dsp_getState());
}

void inputs_rescanSearchPaths (int logged)
{
    searchpath_rescan (logged);
}

void inputs_setSearchPaths (const juce::StringArray& paths)
{
    searchpath_clear(); for (const auto& p : paths) { searchpath_appendRoot (p.toRawUTF8()); }
}

void inputs_setSnap (bool snapToGrid)
{
    instance_snapSet (static_cast<int> (snapToGrid == true));
}

void inputs_setSnapSize (int gridSize)
{
    instance_snapSetGrid (gridSize);
}

void inputs_rescanDevices (void)
{
    audio_rescanDevices();
    midi_rescanDevices();
}

void inputs_setAudioDevices (const std::vector<AudioDevice>& i, const std::vector<AudioDevice>& o)
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

void inputs_setMidiDevices (const std::vector<MidiDevice>& i, const std::vector<MidiDevice>& o)
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_closePatch (core::UniqueId u)
{
    unique_patchClose (u);
}

void inputs_savePatch (core::UniqueId u)
{
    unique_patchSave (u);
}

void inputs_setDirty (core::UniqueId u)
{
    unique_patchSetDirty (u);
}

void inputs_setEditView (core::UniqueId u, juce::Rectangle<int> bounds)
{
    t_rectangle r; rectangle_setCopy (&r, bounds);
    
    unique_patchSetEditView (u, &r);
}

void inputs_setEditViewProperties (core::UniqueId u, core::Point::Real pt, int zoom, bool inspector, int w)
{
    int x = pt.getPoint().getX();
    int y = pt.getPoint().getY();
    
    unique_patchSetEditViewProperties (u, x, y, zoom, static_cast<int> (inspector), w);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_undo (core::UniqueId u)
{
    unique_patchUndo (u);
}

void inputs_redo (core::UniqueId u)
{
    unique_patchRedo (u);
}

void inputs_cut (core::UniqueId u)
{
    unique_patchCut (u);
}

void inputs_copy (core::UniqueId u)
{
    unique_patchCopy (u);
}

void inputs_paste (core::UniqueId u, Point::Real pt)
{
    t_point m = point_make (pt.getPoint().getX(), pt.getPoint().getY()); unique_patchPaste (u, &m);
}

void inputs_duplicate (core::UniqueId u)
{
    unique_patchDuplicate (u);
}

void inputs_remove (core::UniqueId u)
{
    unique_patchRemove (u);
}

void inputs_encapsulate (core::UniqueId u)
{
    unique_patchEncapsulate (u);
}

void inputs_deencapsulate (core::UniqueId u)
{
    unique_patchDeencapsulate (u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_selectObject (core::UniqueId u)
{
    unique_objectSelect (u);
}

void inputs_deselectObject (core::UniqueId u)
{
    unique_objectDeselect (u);
}

void inputs_moveBackObject (core::UniqueId u)
{
    unique_objectMoveBack (u);
}

void inputs_moveFrontObject (core::UniqueId u)
{
    unique_objectMoveFront (u);
}

void inputs_snapObject (core::UniqueId u)
{
    unique_objectSnap (u);
}

void inputs_positionObject (core::UniqueId u, Point::Real pt)
{
    unique_objectPosition (u, pt.getPoint().getX(), pt.getPoint().getY());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_sendObjectBang (core::UniqueId u)
{
    unique_objectBang (u);
}

void inputs_sendObjectFloat (core::UniqueId u, double f)
{
    unique_objectFloat (u, f);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_openHelp (core::UniqueId u)
{
    unique_objectHelp (u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_createObject (core::UniqueId u, Point::Real pt, juce::String s)
{
    unique_patchCreateObject (u, pt, s);
}

void inputs_parametersObject (core::UniqueId u, const data::Group& group)
{
    unique_objectParameter (u, group);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_disconnectLine (core::UniqueId u, int m, core::UniqueId v, int n)
{
    unique_objectLineDisconnect (u, m, v, n);
}

void inputs_connectLine (core::UniqueId u, int m, core::UniqueId v, int n)
{
    unique_objectLineConnect (u, m, v, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
