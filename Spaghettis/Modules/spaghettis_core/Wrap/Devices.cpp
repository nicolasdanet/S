
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void AudioDevices::report (const std::vector<AudioDevice>& devices, const juce::String& s)
{
    for (const auto& d : devices) {
        const juce::String name (std::get<AUDIODEVICES_NAME> (d));
        const juce::String channels (std::get<AUDIODEVICES_CHANNELS> (d));
        SPAGHETTIS_DEBUG (s + " : " + name + " / " + channels);
    }
}

juce::StringArray AudioDevices::getNames (const std::vector<AudioDevice>& devices)
{
    juce::StringArray a; for (const auto& d : devices) { a.add (std::get<AUDIODEVICES_NAME> (d)); }
    
    a.sortNatural();
    
    return a;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void AudioDevices::setAvailableDevices (std::vector<AudioDevice> i, std::vector<AudioDevice> o)
{
    availableDevicesIn_  = std::move (i);
    availableDevicesOut_ = std::move (o);
    
    sendChangeMessage();
    
    report (availableDevicesIn_,  "Available");
    report (availableDevicesOut_, "Available");
}

void AudioDevices::setCurrentDevices (std::vector<AudioDevice> i, std::vector<AudioDevice> o)
{
    currentDevicesIn_  = std::move (i);
    currentDevicesOut_ = std::move (o);
    
    sendChangeMessage();
    
    report (currentDevicesIn_,  "Current");
    report (currentDevicesOut_, "Current");
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::StringArray AudioDevices::getAvailableNamesIn()
{
    return getNames (availableDevicesIn_);
}

juce::StringArray AudioDevices::getAvailableNamesOut()
{
    return getNames (availableDevicesOut_);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
