
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
                            public juce::ChangeListener {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit DevicesComponent (const juce::String& keyName) : ApplicationComponent (keyName)
    {
        Spaghettis()->getAudioDevices().addChangeListener (this);
        
        for (auto& b : audioIn_)       { initializeBox (b); }
        for (auto& b : audioOut_)      { initializeBox (b); }
        for (auto& l : audioInLabel_)  { initializeLabel (l, "Audio In");  }
        for (auto& l : audioOutLabel_) { initializeLabel (l, "Audio Out"); }
        
        setOpaque (true); setSize (400, 500);
        
        Spaghettis()->handle (Inputs::rescanDevices());
    }
    
    ~DevicesComponent() override
    {
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
    void changeListenerCallback (juce::ChangeBroadcaster*) override
    {
        DBG ("?");
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
    void initializeBox (juce::ComboBox& box)
    {
        box.addItem ("Bijoux",   1);
        box.addItem ("Cailloux", 2);
        box.addItem ("Choux",    3);
        box.addItem ("Genoux",   4);
        box.addItem ("Hiboux",   5);
        box.addItem ("Joujoux",  6);
        box.addItem ("Poux",     7);

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
    
    static void dispose (juce::Rectangle<int> t, juce::Label& label, juce::ComboBox& box)
    {
        const int w = 125; label.setBounds (t.removeFromLeft (w).reduced (1)); box.setBounds (t.reduced (1));
    }
    
private:
    std::array<juce::ComboBox, numberOfAudioDevicesAllowed()> audioIn_;
    std::array<juce::ComboBox, numberOfAudioDevicesAllowed()> audioOut_;
    std::array<juce::Label, numberOfAudioDevicesAllowed()> audioInLabel_;
    std::array<juce::Label, numberOfAudioDevicesAllowed()> audioOutLabel_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DevicesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

