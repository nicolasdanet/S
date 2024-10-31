
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

void inputs_dspSwitch (void)
{
    dsp_setState (!dsp_getState());
}

void inputs_searchPathsRescan (int logged)
{
    searchpath_rescan (logged);
}

void inputs_searchPathsSet (const juce::StringArray& paths)
{
    searchpath_clear(); for (const auto& p : paths) { searchpath_appendRoot (p.toRawUTF8()); }
}

void inputs_snapSet (bool snapToGrid)
{
    instance_snapSet (static_cast<int> (snapToGrid == true));
}

void inputs_snapSetGrid (int gridSize)
{
    instance_snapSetGrid (gridSize);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_devicesRescanAudio (int logged)
{
    audio_rescanDevices (logged > 0);
}

void inputs_devicesRescanMidi (int logged)
{
    midi_rescanDevices (logged > 0);
}

void inputs_devicesSetAudio (const juce::StringArray& i, const juce::StringArray& o)
{
    t_devices d; devices_set (&d, i, o); audio_setCurrentDevices (&d);
}

void inputs_devicesSetMidi (const juce::StringArray& i, const juce::StringArray& o)
{
    t_devices d; devices_set (&d, i, o); midi_setCurrentDevices (&d);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_patchUndo (core::UniqueId u)
{
    unique_patchUndo (u);
}

void inputs_patchRedo (core::UniqueId u)
{
    unique_patchRedo (u);
}

void inputs_patchCut (core::UniqueId u)
{
    unique_patchCut (u);
}

void inputs_patchCopy (core::UniqueId u)
{
    unique_patchCopy (u);
}

void inputs_patchPaste (core::UniqueId u, Point::Real pt)
{
    t_point m = point_make (pt.getPoint().getX(), pt.getPoint().getY()); unique_patchPaste (u, &m);
}

void inputs_patchDuplicate (core::UniqueId u)
{
    unique_patchDuplicate (u);
}

void inputs_patchRemove (core::UniqueId u)
{
    unique_patchRemove (u);
}

void inputs_patchEncapsulate (core::UniqueId u)
{
    unique_patchEncapsulate (u);
}

void inputs_patchDeencapsulate (core::UniqueId u)
{
    unique_patchDeencapsulate (u);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_patchNew (const juce::File& f)
{
    if (!f.existsAsFile()) {
        t_symbol *name = gensym (f.getFileName().toRawUTF8());
        t_symbol *directory = gensym (f.getParentDirectory().getFullPathName().toRawUTF8());
        instance_patchNew (name, directory);
    }
}

void inputs_patchOpen (const juce::File& f)
{
    if (f.existsAsFile()) {
        t_symbol *name = gensym (f.getFileName().toRawUTF8());
        t_symbol *directory = gensym (f.getParentDirectory().getFullPathName().toRawUTF8());
        instance_patchOpen (name, directory);
    } else {
        error_doesNotExist (NULL, f.getFullPathName().toRawUTF8());
    }
}

void inputs_patchClose (core::UniqueId u)
{
    unique_patchClose (u);
}

void inputs_patchSave (core::UniqueId u)
{
    unique_patchSave (u);
}

void inputs_patchSetDirty (core::UniqueId u)
{
    unique_patchSetDirty (u);
}

void inputs_patchSetEditView (core::UniqueId u, juce::Rectangle<int> bounds)
{
    t_rectangle r; rectangle_setCopy (&r, bounds);
    
    unique_patchSetEditView (u, &r);
}

void inputs_patchSetEditViewProperties (core::UniqueId u,
    core::Point::Real pt,
    int zoom,
    bool inspector,
    int w)
{
    int x = pt.getPoint().getX();
    int y = pt.getPoint().getY();
    
    unique_patchSetEditViewProperties (u, x, y, zoom, static_cast<int> (inspector), w);
}

void inputs_patchCreateObject (core::UniqueId u, Point::Real pt, juce::String s)
{
    unique_patchCreateObject (u, pt, s);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_objectSelect (core::UniqueId u)
{
    unique_objectSelect (u);
}

void inputs_objectDeselect (core::UniqueId u)
{
    unique_objectDeselect (u);
}

void inputs_objectMoveBack (core::UniqueId u)
{
    unique_objectMoveBack (u);
}

void inputs_objectMoveFront (core::UniqueId u)
{
    unique_objectMoveFront (u);
}

void inputs_objectSnap (core::UniqueId u)
{
    unique_objectSnap (u);
}

void inputs_objectPosition (core::UniqueId u, Point::Real pt)
{
    unique_objectPosition (u, pt.getPoint().getX(), pt.getPoint().getY());
}

void inputs_objectSendBang (core::UniqueId u)
{
    unique_objectBang (u);
}

void inputs_objectSendFloat (core::UniqueId u, double f)
{
    unique_objectFloat (u, f);
}

void inputs_objectOpenHelp (core::UniqueId u)
{
    unique_objectHelp (u);
}

void inputs_objectSetParameters (core::UniqueId u, const data::Group& group)
{
    unique_objectParameter (u, group);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void inputs_lineConnect (core::UniqueId u, int m, core::UniqueId v, int n)
{
    unique_lineConnect (u, m, v, n);
}

void inputs_lineDisconnect (core::UniqueId u, int m, core::UniqueId v, int n)
{
    unique_lineDisconnect (u, m, v, n);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
