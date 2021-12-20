
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

Perform Inputs::closePatch (core::Unique::Identifier i)
{
    return [i]() { core::inputs_closePatch (i); };
}

Perform Inputs::savePatch (core::Unique::Identifier i)
{
    return [i]() { core::inputs_savePatch (i); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
