
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class AvailableDevices {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    AvailableDevices()  = default;
    ~AvailableDevices() = default;

    AvailableDevices (const AvailableDevices&) = default;
    AvailableDevices (AvailableDevices&&) = default;
    AvailableDevices& operator = (const AvailableDevices&) = default;
    AvailableDevices& operator = (AvailableDevices&&) = default;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void setAudioDevices (juce::StringArray i, juce::StringArray o)
    {
        /* ??? */
        DBG ("### AUDIO");
        DBG (juce::String ("I / ") + i.joinIntoString (" "));
        DBG (juce::String ("O / ") + o.joinIntoString (" "));
    
        audioIn_.swapWith (i);
        audioOut_.swapWith (o);
    }
    
    void setMidiDevices (juce::StringArray i, juce::StringArray o)
    {
        /* ??? */
        DBG ("### MIDI");
        DBG (juce::String ("I / ") + i.joinIntoString (" "));
        DBG (juce::String ("O / ") + o.joinIntoString (" "));
    
        midiIn_.swapWith (i);
        midiOut_.swapWith (o);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::StringArray getAudioIn() const
    {
        return audioIn_;
    }
    
    juce::StringArray getAudioOut() const
    {
        return audioOut_;
    }
    
    juce::StringArray getMidiIn() const
    {
        return midiIn_;
    }
    
    juce::StringArray getMidiOut() const
    {
        return midiOut_;
    }
    
private:
    juce::StringArray audioIn_;
    juce::StringArray audioOut_;
    juce::StringArray midiIn_;
    juce::StringArray midiOut_;
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

