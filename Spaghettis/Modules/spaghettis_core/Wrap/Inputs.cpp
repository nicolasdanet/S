
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::ping()
{
    return []() { core::inputs_ping(); };
}

Perform Inputs::newPatch (juce::File file)
{
    return [f = std::move (file)]() { core::inputs_newPatch (f); };
}

Perform Inputs::openPatch (juce::File file)
{
    return [f = std::move (file)]() { core::inputs_openPatch (f); };
}

Perform Inputs::rescanSearchPaths (Logged type)
{
    const int logged = (type == Logged::base) ? 0 : ((type == Logged::full) ? 1 : -1);
    
    return [logged]() { core::inputs_rescanSearchPaths (logged); };
}

Perform Inputs::switchDsp()
{
    return []() { core::inputs_switchDsp(); };
}

Perform Inputs::setSearchPaths (juce::StringArray paths)
{
    return [p = std::move (paths)]() { core::inputs_setSearchPaths (p); };
}

Perform Inputs::setSnapToGrid (bool isSet)
{
    return [isSet]() { core::inputs_setSnap (isSet); };
}

Perform Inputs::setSnapToGridSize (int size)
{
    return [size]() { core::inputs_setSnapSize (size); };
}

Perform Inputs::rescanDevices (void)
{
    return []() { core::inputs_rescanDevices(); };
}

Perform Inputs::setAudioDevices (std::vector<AudioDevice>&& i, std::vector<AudioDevice>&& o)
{
    auto f = [devicesIn = std::move (i), devicesOut = std::move (o)]()
    {
        core::inputs_setAudioDevices (devicesIn, devicesOut);
    };
    
    return f;
}

Perform Inputs::setMidiDevices (std::vector<MidiDevice>&& i, std::vector<MidiDevice>&& o)
{
    auto f = [devicesIn = std::move (i), devicesOut = std::move (o)]()
    {
        core::inputs_setMidiDevices (devicesIn, devicesOut);
    };
    
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::closePatch (core::UniqueId i)
{
    return [i]() { core::inputs_closePatch (i); };
}

Perform Inputs::savePatch (core::UniqueId i)
{
    return [i]() { core::inputs_savePatch (i); };
}

Perform Inputs::setEditView (core::UniqueId i, juce::Rectangle<int> bounds)
{
    return [i, bounds]() { core::inputs_setEditView (i, bounds); };
}

Perform Inputs::setRunView (core::UniqueId i, juce::Rectangle<int> bounds)
{
    return [i, bounds]() { core::inputs_setRunView (i, bounds); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::undo (core::UniqueId i)
{
    return [i]() { core::inputs_undo (i); };
}

Perform Inputs::redo (core::UniqueId i)
{
    return [i]() { core::inputs_redo (i); };
}

Perform Inputs::cut (core::UniqueId i)
{
    return [i]() { core::inputs_cut (i); };
}

Perform Inputs::copy (core::UniqueId i)
{
    return [i]() { core::inputs_copy (i); };
}

Perform Inputs::paste (core::UniqueId i, std::optional<juce::Point<int>> pt)
{
    return [i, pt]() { core::inputs_paste (i, pt); };
}

Perform Inputs::duplicate (core::UniqueId i)
{
    return [i]() { core::inputs_duplicate (i); };
}

Perform Inputs::remove (core::UniqueId i)
{
    return [i]() { core::inputs_remove (i); };
}

Perform Inputs::encapsulate (core::UniqueId i)
{
    return [i]() { core::inputs_encapsulate (i); };
}

Perform Inputs::deencapsulate (core::UniqueId i)
{
    return [i]() { core::inputs_deencapsulate (i); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::selectObject (core::UniqueId i)
{
    return [i]() { core::inputs_selectObject (i); };
}

Perform Inputs::deselectObject (core::UniqueId i)
{
    return [i]() { core::inputs_deselectObject (i); };
}

Perform Inputs::moveBackObject (core::UniqueId i)
{
    return [i]() { core::inputs_moveBackObject (i); };
}

Perform Inputs::moveFrontObject (core::UniqueId i)
{
    return [i]() { core::inputs_moveFrontObject (i); };
}

Perform Inputs::snapObject (core::UniqueId i)
{
    return [i]() { core::inputs_snapObject (i); };
}

Perform Inputs::positionObject (core::UniqueId i, juce::Point<int> pt)
{
    return [i, pt]() { core::inputs_positionObject (i, pt); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::parameterObject (core::UniqueId i, core::Group group)
{
    return [i, g = std::move (group)]() { core::inputs_parameterObject (i, g); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::disconnectLine (core::UniqueId u, int m, core::UniqueId v, int n)
{
    return [u, m, v, n]() { core::inputs_disconnectLine (u, m, v, n); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
