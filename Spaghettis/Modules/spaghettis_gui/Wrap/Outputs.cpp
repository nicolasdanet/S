
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

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

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
