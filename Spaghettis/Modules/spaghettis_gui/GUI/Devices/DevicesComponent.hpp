
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

constexpr int numberOfDevices() { return 2; }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class DevicesComponent :    public ApplicationComponent,
                            public juce::ComboBox::Listener,
                            public juce::ChangeListener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit DevicesComponent (const juce::String& keyName) : ApplicationComponent (keyName),
        audioInTag_ ("Audio In"),
        audioOutTag_ ("Audio Out"),
        midiInTag_ ("Midi In"),
        midiOutTag_ ("Midi Out"),
        noneTag_ ("TOTO")
    {
        Spaghettis()->getAudioDevices().addChangeListener (this);
        Spaghettis()->getMidiDevices().addChangeListener (this);
        
        initialize();
                
        setOpaque (true); setSize (400, 500);
        
        Spaghettis()->handle (Inputs::rescanDevices());
    }
    
    ~DevicesComponent() override
    {
        release();
        
        Spaghettis()->getMidiDevices().removeChangeListener (this);
        Spaghettis()->getAudioDevices().removeChangeListener (this);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Spaghettis()->getColour (Colours::windowBackground));
    }
    
    void resized() override
    {
        const int h = static_cast<int> (Spaghettis()->getLookAndFeel().getComboBoxFont().getHeight() * 1.5);
        
        juce::Rectangle<int> area (getBoundsRemaining());

        const int n = numberOfDevices();
        
        for (int i = 0; i < n; ++i) { dispose (area.removeFromTop (h), audioInLabel_[i],  audioIn_[i]);  }
        for (int i = 0; i < n; ++i) { dispose (area.removeFromTop (h), audioOutLabel_[i], audioOut_[i]); }
        for (int i = 0; i < n; ++i) { dispose (area.removeFromTop (h), midiInLabel_[i],   midiIn_[i]);   }
        for (int i = 0; i < n; ++i) { dispose (area.removeFromTop (h), midiOutLabel_[i],  midiOut_[i]);  }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    template <class T>
    void updateViewProceed (const T& devices, std::array<juce::ComboBox, numberOfDevices()>& a, bool b)
    {
        int n = 0;
            
        for (auto& c : a) {
            c.clear (juce::dontSendNotification);
            c.addItem (noneTag_, firstItemId_);
            c.addItemList (devices.getAvailableNames (b), firstItemId_ + 1);
            const juce::String s (devices.getNameAt (n++, b));
            setSelectedItemByString (c, s.isEmpty() ? noneTag_ : s);
        }
    }
    
    void updateView()
    {
        updateViewProceed (Spaghettis()->getAudioDevices(), audioIn_,  true);
        updateViewProceed (Spaghettis()->getAudioDevices(), audioOut_, false);
        updateViewProceed (Spaghettis()->getMidiDevices(),  midiIn_,   true);
        updateViewProceed (Spaghettis()->getMidiDevices(),  midiOut_,  false);
    }

    void updateAudioDevice (const juce::String& name, int n, bool isDeviceIn)
    {
        const AudioDevices& d (Spaghettis()->getAudioDevices());
        
        std::vector<AudioDevice> i (isDeviceIn ? d.getDevicesInChangedAt (name, n) : d.getDevicesIn());
        std::vector<AudioDevice> o (isDeviceIn ? d.getDevicesOut() : d.getDevicesOutChangedAt (name, n));
        
        Spaghettis()->handle (Inputs::setAudioDevices (std::move (i), std::move (o)));
    }
    
    void updateMidiDevice (const juce::String& name, int n, bool isDeviceIn)
    {
        const MidiDevices& d (Spaghettis()->getMidiDevices());
        
        std::vector<MidiDevice> i (isDeviceIn ? d.getDevicesInChangedAt (name, n) : d.getDevicesIn());
        std::vector<MidiDevice> o (isDeviceIn ? d.getDevicesOut() : d.getDevicesOutChangedAt (name, n));
        
        Spaghettis()->handle (Inputs::setMidiDevices (std::move (i), std::move (o)));
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void comboBoxChanged (juce::ComboBox *box) override
    {
        const juce::String s (box->getComponentID());
        
        juce::String name (box->getText()); if (name == noneTag_) { name = juce::String(); }
        
        const bool isAudioIn  = s.startsWith (audioInTag_);
        const bool isAusioOut = s.startsWith (audioOutTag_);
        const bool isMidiIn   = s.startsWith (midiInTag_);
        const bool isMidiOut  = s.startsWith (midiOutTag_);
        
        const int n = s.getTrailingIntValue();
        
        jassert (n >= 0);
        
        if (isAudioIn || isAusioOut)    { updateAudioDevice (name, n, (isAudioIn == true)); }
        else if (isMidiIn || isMidiOut) { updateMidiDevice (name, n, (isMidiIn == true)); }
    }

    void changeListenerCallback (juce::ChangeBroadcaster*) override
    {
        updateView();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/*
public:
    bool tryGrabFocus() override
    {
        
    }
*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void initializeBox (juce::ComboBox& box, const juce::String& s, int i)
    {
        const juce::String tooltip (NEEDS_TRANS ("Select a device for " + s.toLowerCase()));
        
        box.setTooltip (tooltip);
        box.setComponentID (s + " " + juce::String (i));
        box.addListener (this);
        
        addAndMakeVisible (box);
    }
    
    void initializeLabel (juce::Label& label, const juce::String& s)
    {
        const juce::Colour text (Spaghettis()->getColour (Colours::devicesParameterText));
        const juce::Colour background (Spaghettis()->getColour (Colours::devicesParameterBackground));
        
        label.setText (s, juce::dontSendNotification);
        
        label.setColour (juce::Label::textColourId,       text);
        label.setColour (juce::Label::backgroundColourId, background);
        
        addAndMakeVisible (label);
    }
    
    void releaseBox (juce::ComboBox& box)
    {
        box.removeListener (this);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void initialize()
    {
        {
            int m = 0; int n = 0;
            
            for (auto& b : audioIn_)        { initializeBox (b,   audioInTag_,  m++); }
            for (auto& b : audioOut_)       { initializeBox (b,   audioOutTag_, n++); }
            for (auto& l : audioInLabel_)   { initializeLabel (l, audioInTag_);       }
            for (auto& l : audioOutLabel_)  { initializeLabel (l, audioOutTag_);      }
        }
        
        {
            int m = 0; int n = 0;
            
            for (auto& b : midiIn_)         { initializeBox (b,   midiInTag_,   m++); }
            for (auto& b : midiOut_)        { initializeBox (b,   midiOutTag_,  n++); }
            for (auto& l : midiInLabel_)    { initializeLabel (l, midiInTag_);        }
            for (auto& l : midiOutLabel_)   { initializeLabel (l, midiOutTag_);       }
        }
    }
    
    void release()
    {
        for (auto& b : midiOut_)  { releaseBox (b); }
        for (auto& b : midiIn_)   { releaseBox (b); }
        for (auto& b : audioOut_) { releaseBox (b); }
        for (auto& b : audioIn_)  { releaseBox (b); }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void dispose (juce::Rectangle<int> t, juce::Label& label, juce::ComboBox& box)
    {
        const int w = 125; label.setBounds (t.removeFromLeft (w).reduced (1)); box.setBounds (t.reduced (1));
    }
    
    static void setSelectedItemByString (juce::ComboBox& box, const juce::String& s)
    {
        const int n = box.getNumItems();
        
        for (int i = 0; i < n; ++i) {
            if (box.getItemText (i) == s) {
                box.setSelectedItemIndex (i, juce::dontSendNotification);
                break;
            }
        }
    }

private:
    const juce::String audioInTag_;
    const juce::String audioOutTag_;
    const juce::String midiInTag_;
    const juce::String midiOutTag_;
    const juce::String noneTag_;
    
private:
    std::array<juce::ComboBox, numberOfDevices()> audioIn_;
    std::array<juce::ComboBox, numberOfDevices()> audioOut_;
    std::array<juce::Label,    numberOfDevices()> audioInLabel_;
    std::array<juce::Label,    numberOfDevices()> audioOutLabel_;

private:
    std::array<juce::ComboBox, numberOfDevices()> midiIn_;
    std::array<juce::ComboBox, numberOfDevices()> midiOut_;
    std::array<juce::Label,    numberOfDevices()> midiInLabel_;
    std::array<juce::Label,    numberOfDevices()> midiOutLabel_;

private:
    static const int firstItemId_ = 1;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DevicesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

