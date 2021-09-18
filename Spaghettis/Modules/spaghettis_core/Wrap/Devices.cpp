
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void AudioDevices::setAvailableDevices (std::vector<AudioDevice> i, std::vector<AudioDevice> o)
{
    availableDevicesIn_  = std::move (i);
    availableDevicesOut_ = std::move (o);
    
    for (const auto& d : availableDevicesIn_) {
        DBG (std::get<AUDIODEVICES_NAME> (d) + " / " + juce::String (std::get<AUDIODEVICES_CHANNELS> (d)));
    }
    
    for (const auto& d : availableDevicesOut_) {
        DBG (std::get<AUDIODEVICES_NAME> (d) + " / " + juce::String (std::get<AUDIODEVICES_CHANNELS> (d)));
    }
}

void AudioDevices::setCurrentDevices (std::vector<AudioDevice> i, std::vector<AudioDevice> o)
{
    currentDevicesIn_  = std::move (i);
    currentDevicesOut_ = std::move (o);
    
    for (const auto& d : currentDevicesIn_) {
        DBG (std::get<AUDIODEVICES_NAME> (d) + " / " + juce::String (std::get<AUDIODEVICES_CHANNELS> (d)));
    }
    
    for (const auto& d : currentDevicesOut_) {
        DBG (std::get<AUDIODEVICES_NAME> (d) + " / " + juce::String (std::get<AUDIODEVICES_CHANNELS> (d)));
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
