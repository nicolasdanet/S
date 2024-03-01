
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

Perform Inputs::closePatch (core::UniqueId u)
{
    return [u]() { core::inputs_closePatch (u); };
}

Perform Inputs::savePatch (core::UniqueId u)
{
    return [u]() { core::inputs_savePatch (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::setEditView (core::UniqueId u, juce::Rectangle<int> bounds)
{
    return [u, bounds]() { core::inputs_setEditView (u, bounds); };
}

Perform Inputs::setEditViewProperties (core::UniqueId u, core::Point::Real pt, int z, bool inspector, int w)
{
    return [u, pt, z, inspector, w]() { core::inputs_setEditViewProperties (u, pt, z, inspector, w); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::undo (core::UniqueId u)
{
    return [u]() { core::inputs_undo (u); };
}

Perform Inputs::redo (core::UniqueId u)
{
    return [u]() { core::inputs_redo (u); };
}

Perform Inputs::cut (core::UniqueId u)
{
    return [u]() { core::inputs_cut (u); };
}

Perform Inputs::copy (core::UniqueId u)
{
    return [u]() { core::inputs_copy (u); };
}

Perform Inputs::paste (core::UniqueId u, core::Point::Real pt)
{
    return [u, pt]() { core::inputs_paste (u, pt); };
}

Perform Inputs::duplicate (core::UniqueId u)
{
    return [u]() { core::inputs_duplicate (u); };
}

Perform Inputs::remove (core::UniqueId u)
{
    return [u]() { core::inputs_remove (u); };
}

Perform Inputs::encapsulate (core::UniqueId u)
{
    return [u]() { core::inputs_encapsulate (u); };
}

Perform Inputs::deencapsulate (core::UniqueId u)
{
    return [u]() { core::inputs_deencapsulate (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::selectObject (core::UniqueId u)
{
    return [u]() { core::inputs_selectObject (u); };
}

Perform Inputs::deselectObject (core::UniqueId u)
{
    return [u]() { core::inputs_deselectObject (u); };
}

Perform Inputs::moveBackObject (core::UniqueId u)
{
    return [u]() { core::inputs_moveBackObject (u); };
}

Perform Inputs::moveFrontObject (core::UniqueId u)
{
    return [u]() { core::inputs_moveFrontObject (u); };
}

Perform Inputs::snapObject (core::UniqueId u)
{
    return [u]() { core::inputs_snapObject (u); };
}

Perform Inputs::positionObject (core::UniqueId u, core::Point::Real pt)
{
    return [u, pt]() { core::inputs_positionObject (u, pt); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::sendObjectBang (core::UniqueId u)
{
    return [u]() { core::inputs_sendObjectBang (u); };
}

Perform Inputs::sendObjectFloat (core::UniqueId u, double f)
{
    return [u, f]() { core::inputs_sendObjectFloat (u, f); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::openHelp (core::UniqueId u)
{
    return [u]() { core::inputs_openHelp (u); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::createObject (core::UniqueId u, core::Point::Real pt, juce::String s)
{
    return [u, pt, s]() { core::inputs_createObject (u, pt, s); };
}

Perform Inputs::parametersObject (core::UniqueId u, data::Group group)
{
    return [u, g = std::move (group)]() { core::inputs_parametersObject (u, g); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Inputs::disconnectLine (core::UniqueId u, int m, core::UniqueId v, int n)
{
    return [u, m, v, n]() { core::inputs_disconnectLine (u, m, v, n); };
}

Perform Inputs::connectLine (core::UniqueId u, int m, core::UniqueId v, int n)
{
    return [u, m, v, n]() { core::inputs_connectLine (u, m, v, n); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
