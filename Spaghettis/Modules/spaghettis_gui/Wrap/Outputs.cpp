
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

Perform Outputs::quit()
{
    return []() { juce::JUCEApplication::getInstance()->systemRequestedQuit(); };
}

Perform Outputs::clearConsole()
{
    return []() { Spaghettis()->clearConsole(); };
}

Perform Outputs::reportDsp (bool state)
{
    return [state]() { Spaghettis()->setDspState (state); };
}

Perform Outputs::patchOpened (juce::File file)
{
    return [f = std::move (file)]() { Spaghettis()->appendRecentFile (f); };
}

Perform Outputs::reportAvailableAudioDevices (std::vector<AudioDevice>&& i, std::vector<AudioDevice>&& o)
{
    auto f = [devicesIn = std::move (i), devicesOut = std::move (o)]()
    {
        Spaghettis()->getAudioDevices().setAvailableDevices (devicesIn, devicesOut);
    };
    
    return f;
}

Perform Outputs::reportCurrentAudioDevices (std::vector<AudioDevice>&& i, std::vector<AudioDevice>&& o)
{
    auto f = [devicesIn = std::move (i), devicesOut = std::move (o)]()
    {
        Spaghettis()->getAudioDevices().setCurrentDevices (devicesIn, devicesOut);
    };
    
    return f;
}

Perform Outputs::reportAvailableMidiDevices (std::vector<MidiDevice>&& i, std::vector<MidiDevice>&& o)
{
    auto f = [devicesIn = std::move (i), devicesOut = std::move (o)]()
    {
        Spaghettis()->getMidiDevices().setAvailableDevices (devicesIn, devicesOut);
    };
    
    return f;
}

Perform Outputs::reportCurrentMidiDevices (std::vector<MidiDevice>&& i, std::vector<MidiDevice>&& o)
{
    auto f = [devicesIn = std::move (i), devicesOut = std::move (o)]()
    {
        Spaghettis()->getMidiDevices().setCurrentDevices (devicesIn, devicesOut);
    };
    
    return f;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Perform Outputs::objectAdded (core::Unique unique, core::Description view)
{
    return [u = std::move (unique), v = std::move (view)]() { Spaghettis()->getPatches().addObject (u, v); };
}

Perform Outputs::objectRemoved (core::Unique unique)
{
    return [u = std::move (unique)]() { Spaghettis()->getPatches().removeObject (u); };
}

Perform Outputs::objectRenamed (core::Unique unique, core::Unique::Identifier i)
{
    return [i, u = std::move (unique)]() { Spaghettis()->getPatches().renameObject (u, i); };
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
