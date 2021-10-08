
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

class AudioDevice {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    AudioDevice (const juce::String& name, int channels = 0) : name_ (name), channels_ (channels)
    {
    }
    
    ~AudioDevice() = default;

public:
    AudioDevice (const AudioDevice&) = default;
    AudioDevice (AudioDevice&&) = default;
    AudioDevice& operator = (const AudioDevice&) = default;
    AudioDevice& operator = (AudioDevice&&) = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getName() const
    {
        return name_;
    }
    
    int getChannels() const
    {
        return channels_;
    }
    
private:
    juce::String name_;
    int channels_;
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using MidiDevice = AudioDevice;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

template <class T> class Devices : public juce::ChangeBroadcaster {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Devices()  = default;
    ~Devices() = default;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void setAvailableDevices (std::vector<T> i, std::vector<T> o)
    {
        availableDevicesIn_  = std::move (i);
        availableDevicesOut_ = std::move (o);
        
        sendChangeMessage();
    }

    void setCurrentDevices (std::vector<T> i, std::vector<T> o)
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
    std::vector<T> getDevicesIn() const
    {
        return currentDevicesIn_;
    }

    std::vector<T> getDevicesOut() const
    {
        return currentDevicesOut_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    std::vector<T> getDevicesInChangedAt (const juce::String& name, int n) const;
    std::vector<T> getDevicesOutChangedAt (const juce::String& name, int n) const;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -
    
private:
    static juce::StringArray getNames (const std::vector<T>&);
    static juce::String getNameAt (const std::vector<T>&, int);
    static int getChannelsFor (const std::vector<T>&, const juce::String&);
    static void changeDeviceAt (std::vector<T>&, int, const juce::String&, int);

private:
    std::vector<T> availableDevicesIn_;
    std::vector<T> availableDevicesOut_;
    std::vector<T> currentDevicesIn_;
    std::vector<T> currentDevicesOut_;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Devices)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using AudioDevices = Devices<AudioDevice>;
using MidiDevices  = Devices<MidiDevice>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

