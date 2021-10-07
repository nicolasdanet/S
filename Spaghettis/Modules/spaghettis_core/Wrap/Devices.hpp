
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using AudioDevice = std::pair<juce::String, int>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class AudioDevices : public juce::ChangeBroadcaster {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    enum {
        AUDIODEVICES_NAME       = 0,
        AUDIODEVICES_CHANNELS   = 1
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::String getName (const AudioDevice& d)
    {
        return std::get<AUDIODEVICES_NAME> (d);
    }
    
    static int getChannels (const AudioDevice& d)
    {
        return std::get<AUDIODEVICES_CHANNELS> (d);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    AudioDevices()  = default;
    ~AudioDevices() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void setAvailableDevices (std::vector<AudioDevice> i, std::vector<AudioDevice> o)
    {
        availableDevicesIn_  = std::move (i);
        availableDevicesOut_ = std::move (o);
        
        sendChangeMessage();
    }

    void setCurrentDevices (std::vector<AudioDevice> i, std::vector<AudioDevice> o)
    {
        currentDevicesIn_  = std::move (i);
        currentDevicesOut_ = std::move (o);
        
        sendChangeMessage();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::StringArray getAvailableNamesIn() const
    {
        return getNames (availableDevicesIn_);
    }

    juce::StringArray getAvailableNamesOut() const
    {
        return getNames (availableDevicesOut_);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getNameInAt (int n) const
    {
        return getNameAt (currentDevicesIn_, n);
    }

    juce::String getNameOutAt (int n) const
    {
        return getNameAt (currentDevicesOut_, n);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::vector<AudioDevice> getDevicesIn() const
    {
        return currentDevicesIn_;
    }

    std::vector<AudioDevice> getDevicesOut() const
    {
        return currentDevicesOut_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::vector<AudioDevice> getDevicesInChangedAt (const juce::String& name, int n) const;
    std::vector<AudioDevice> getDevicesOutChangedAt (const juce::String& name, int n) const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
    
private:
    static juce::StringArray getNames (const std::vector<AudioDevice>&);
    static juce::String getNameAt (const std::vector<AudioDevice>&, int);
    static int getChannelsFor (const std::vector<AudioDevice>&, const juce::String&);
    static void changeDeviceAt (std::vector<AudioDevice>&, int, const juce::String&, int);

private:
    std::vector<AudioDevice> availableDevicesIn_;
    std::vector<AudioDevice> availableDevicesOut_;
    std::vector<AudioDevice> currentDevicesIn_;
    std::vector<AudioDevice> currentDevicesOut_;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioDevices)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

