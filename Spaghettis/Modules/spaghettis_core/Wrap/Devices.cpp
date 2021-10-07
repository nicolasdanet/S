
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
    
    if (n < devices.size()) { return std::get<AUDIODEVICES_NAME> (devices[n]); }
    
    return juce::String();
}

int AudioDevices::getChannelsFor (const std::vector<AudioDevice>& devices, const juce::String& name)
{
    auto f = [&] (const AudioDevice& d) { return std::get<AUDIODEVICES_NAME> (d) == name; };
    auto r = std::find_if (devices.begin(), devices.end(), f);
    return r != devices.end() ? std::get<AUDIODEVICES_CHANNELS> (*r) : 0;
}

void AudioDevices::changeDeviceAt (std::vector<AudioDevice>& devices,
    int i,
    const juce::String& name,
    int channels)
{
    const std::vector<AudioDevice>::size_type n = i;
    
    if (n < devices.size()) { devices[n] = AudioDevice (name, channels); }
    else {
        devices.emplace_back (name, channels);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

std::vector<AudioDevice> AudioDevices::getDevicesInChangedAt (const juce::String& name, int n) const
{
    return currentDevicesIn_;
}

std::vector<AudioDevice> AudioDevices::getDevicesOutChangedAt (const juce::String& name, int n) const
{
    return currentDevicesOut_;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
