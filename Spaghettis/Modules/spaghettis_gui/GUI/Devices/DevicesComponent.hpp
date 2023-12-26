
/* Copyright (c) 2021 Jojo and others. */

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

class DevicesComponent :    protected DevicesFactoryHelper,
                            public  BaseComponent,
                            private juce::ComboBox::Listener,
                            private juce::ChangeListener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

using StringGenerator = std::function<juce::String()>;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit DevicesComponent (juce::PropertiesFile* propertiesFile, const juce::String& keyName) :
        DevicesFactoryHelper (this),
        BaseComponent (getIconsFactory(), propertiesFile, keyName),
        audioInTag_ ("Audio In"),
        audioOutTag_ ("Audio Out"),
        midiInTag_ ("Midi In"),
        midiOutTag_ ("Midi Out"),
        noneTag_ ("No Device")
    {
        Spaghettis()->getAudioDevices().addChangeListener (this);
        Spaghettis()->getMidiDevices().addChangeListener (this);
        
        initialize();
        
        setOpaque (true); setSize (700, getTotalHeight());
        
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
    static int getComboBoxHeight()
    {
        return static_cast<int> (Fonts::getMenuFont().getHeight() * 1.75);
    }
    
    static int getTotalHeight()
    {
        const int toolbar = WindowsProperties::getToolbarHeight();
        const int devices = getComboBoxHeight() * numberOfDevices() * 4;
        
        return toolbar + devices;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Colours::fetchColour (Colours::windowsBackground));
    }
    
    void resized() override
    {
        juce::Rectangle<int> area (setBoundsForBarsAndGetRemaining());

        const int n = numberOfDevices();
        const int h = getComboBoxHeight();
        
        for (int i = 0; i < n; ++i) {
            dispose (area.removeFromTop (h), audioInLabel_[i],  audioIn_[i]);
            dispose (area.removeFromTop (h), audioOutLabel_[i], audioOut_[i]);
        }
        for (int i = 0; i < n; ++i) {
            dispose (area.removeFromTop (h), midiInLabel_[i],   midiIn_[i]);
            dispose (area.removeFromTop (h), midiOutLabel_[i],  midiOut_[i]);
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool tryGrabFocus() override
    {
        return tryGrabFocusForComponent (this);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    template <class T>
    void updateViewProceed (const T& devices, std::array<juce::ComboBox, numberOfDevices()>& a, bool b)
    {
        const int firstItemId = 1;
        
        int n = 0;
            
        for (auto& c : a) {
            c.clear (juce::dontSendNotification);
            c.addItemList (devices.getAvailableNames (b), firstItemId + 1);
            c.addItem (noneTag_, firstItemId);
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
// MARK: -

private:
    void initializeBox (juce::ComboBox& box, StringGenerator& f)
    {
        box.setTooltip (NEEDS_TRANS ("Select an I/O device"));
        box.setComponentID (f());
        box.addListener (this);
        
        addAndMakeVisible (box);
    }
    
    void initializeLabel (juce::Label& label, StringGenerator& f)
    {
        const juce::Colour text (Colours::fetchColour (Colours::devicesParameterText));
        const juce::Colour background (Colours::fetchColour (Colours::devicesParameterBackground));
        
        label.setText (f(), juce::dontSendNotification);
        label.setColour (juce::Label::textColourId,       text);
        label.setColour (juce::Label::backgroundColourId, background);
        
        addAndMakeVisible (label);
    }
    
    void releaseBox (juce::ComboBox& box)
    {
        box.hidePopup();
        box.removeListener (this);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void initialize()
    {
        auto g = [] (const juce::String& s)
        {
            return [s, n = 0]() mutable { return s + " " + juce::String (n++); };
        };
        
        auto h = [] (const juce::String& s)
        {
            return [s, n = 0]() mutable { return juce::String (n++) + " : " + s; };
        };
        
        StringGenerator f;
        
        f = g (audioInTag_);  for (auto& b : audioIn_)       { initializeBox (b, f); }
        f = g (audioOutTag_); for (auto& b : audioOut_)      { initializeBox (b, f); }
        f = g (midiInTag_);   for (auto& b : midiIn_)        { initializeBox (b, f); }
        f = g (midiOutTag_);  for (auto& b : midiOut_)       { initializeBox (b, f); }
        
        f = h (audioInTag_);  for (auto& l : audioInLabel_)  { initializeLabel (l, f); }
        f = h (audioOutTag_); for (auto& l : audioOutLabel_) { initializeLabel (l, f); }
        f = h (midiInTag_);   for (auto& l : midiInLabel_)   { initializeLabel (l, f); }
        f = h (midiOutTag_);  for (auto& l : midiOutLabel_)  { initializeLabel (l, f); }
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
        const int w = 180; label.setBounds (t.removeFromLeft (w).reduced (1)); box.setBounds (t.reduced (1));
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DevicesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

