
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

constexpr int numberOfAudioDevicesAllowed() { return 1; }
constexpr int numberOfMidiDevicesAllowed()  { return 4; }
    
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
        audioOutTag_ ("Audio Out")
    {
        Spaghettis()->getAudioDevices().addChangeListener (this);
        
        int m = 0;
        int n = 0;
        
        for (auto& b : audioIn_)       { initializeBox (b,   audioInTag_,  m++); }
        for (auto& b : audioOut_)      { initializeBox (b,   audioOutTag_, n++); }
        for (auto& l : audioInLabel_)  { initializeLabel (l, audioInTag_); }
        for (auto& l : audioOutLabel_) { initializeLabel (l, audioOutTag_); }
                
        setOpaque (true); setSize (400, 500);
        
        Spaghettis()->handle (Inputs::rescanDevices());
    }
    
    ~DevicesComponent() override
    {
        for (auto& b : audioOut_) { releaseBox (b); }
        for (auto& b : audioIn_)  { releaseBox (b); }
        
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
        const int n = numberOfAudioDevicesAllowed();
        
        juce::Rectangle<int> area (getBoundsRemaining());

        for (int i = 0; i < n; ++i) { dispose (area.removeFromTop (h), audioInLabel_[i],  audioIn_[i]);  }
        for (int i = 0; i < n; ++i) { dispose (area.removeFromTop (h), audioOutLabel_[i], audioOut_[i]); }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void updateView()
    {
        int m = 0;
        
        for (auto& c : audioIn_) {
        //
        c.clear (juce::dontSendNotification);
        c.addItemList (Spaghettis()->getAudioDevices().getAvailableNamesIn(), firstItemIdOffset_);
        setSelectedItemByString (c, Spaghettis()->getAudioDevices().getNameInAt (m++));
        //
        }
        
        int n = 0;
        
        for (auto& c : audioOut_) {
        //
        c.clear (juce::dontSendNotification);
        c.addItemList (Spaghettis()->getAudioDevices().getAvailableNamesOut(), firstItemIdOffset_);
        setSelectedItemByString (c, Spaghettis()->getAudioDevices().getNameOutAt (n++));
        //
        }
    }

private:
    void updateDevice (const juce::String& name, int n, bool isDeviceIn)
    {
        const AudioDevices& d (Spaghettis()->getAudioDevices());
        
        std::vector<AudioDevice> i (isDeviceIn ? d.getDevicesInChangedAt (name, n) : d.getDevicesIn());
        std::vector<AudioDevice> o (isDeviceIn ? d.getDevicesOut() : d.getDevicesOutChangedAt (name, n));
        
        Spaghettis()->handle (Inputs::setAudioDevices (std::move (i), std::move (o)));
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void comboBoxChanged (juce::ComboBox *box) override
    {
        const juce::String s (box->getComponentID());
        
        const bool i = s.startsWith (audioInTag_);
        const bool o = s.startsWith (audioOutTag_);
        const int n  = s.getTrailingIntValue();
        
        jassert (n >= 0);
        
        if (i || o) { updateDevice (box->getText(), n, (i == true)); }
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
    static void dispose (juce::Rectangle<int> t, juce::Label& label, juce::ComboBox& box)
    {
        const int w = 125; label.setBounds (t.removeFromLeft (w).reduced (1)); box.setBounds (t.reduced (1));
    }
    
    static void setSelectedItemByString (juce::ComboBox& box, const juce::String& s)
    {
        if (s.isNotEmpty()) {
        //
        const int n = box.getNumItems();
        
        for (int i = 0; i < n; ++i) {
            if (box.getItemText (i) == s) {
                box.setSelectedItemIndex (i, juce::dontSendNotification);
                break;
            }
        }
        //
        }
    }

private:
    const juce::String audioInTag_;
    const juce::String audioOutTag_;
    
private:
    std::array<juce::ComboBox, numberOfAudioDevicesAllowed()> audioIn_;
    std::array<juce::ComboBox, numberOfAudioDevicesAllowed()> audioOut_;
    std::array<juce::Label, numberOfAudioDevicesAllowed()> audioInLabel_;
    std::array<juce::Label, numberOfAudioDevicesAllowed()> audioOutLabel_;

private:
    static const int firstItemIdOffset_ = 1;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DevicesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

