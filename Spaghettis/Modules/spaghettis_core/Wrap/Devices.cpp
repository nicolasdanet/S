
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::StringArray AudioDevices::getNames (const std::vector<AudioDevice>& devices)
{
    juce::StringArray a; for (const auto& d : devices) { a.add (std::get<AUDIODEVICES_NAME> (d)); }
    
    a.sortNatural();
    
    return a;
}

juce::String AudioDevices::getNameAt (const std::vector<AudioDevice>& devices, int i)
{
    jassert (i >= 0); const std::vector<AudioDevice>::size_type n = i;
    
    if (n <= devices.size()) { return std::get<AUDIODEVICES_NAME> (devices[i]); }
    
    return juce::String();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void AudioDevices::setAvailableDevices (std::vector<AudioDevice> i, std::vector<AudioDevice> o)
{
    availableDevicesIn_  = std::move (i);
    availableDevicesOut_ = std::move (o);
    
    sendChangeMessage();
}

void AudioDevices::setCurrentDevices (std::vector<AudioDevice> i, std::vector<AudioDevice> o)
{
    currentDevicesIn_  = std::move (i);
    currentDevicesOut_ = std::move (o);
    
    sendChangeMessage();
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
// MARK: -

juce::String AudioDevices::getCurrentNameInAtIndex (int n)
{
    return getNameAt (currentDevicesIn_, n);
}

juce::String AudioDevices::getCurrentNameOutAtIndex (int n)
{
    return getNameAt (currentDevicesOut_, n);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
