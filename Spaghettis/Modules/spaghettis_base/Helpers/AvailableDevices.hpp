
/* Copyright (c) 2024 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class AvailableDevices : public juce::ChangeBroadcaster {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    AvailableDevices()  = default;
    ~AvailableDevices() = default;
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

public:
    void setAudioDevices (juce::StringArray i, juce::StringArray o)
    {
        /* ??? */
        // if (audioIn_ != i || audioOut_ != o) {
        //
        audioIn_.swapWith (i);
        audioOut_.swapWith (o);
        
        sendChangeMessage();
        //
        // }
    }
    
    void setMidiDevices (juce::StringArray i, juce::StringArray o)
    {
        /* ??? */
        // if (midiIn_ != i || midiOut_ != o) {
        //
        midiIn_.swapWith (i);
        midiOut_.swapWith (o);
        
        sendChangeMessage();
        //
        // }
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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AvailableDevices)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

