
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PreferencesComponent :    public  ApplicationComponent,
                                private juce::Timer {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

friend class Preferences;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    PreferencesComponent (const juce::String& keyName) : ApplicationComponent (keyName)
    {
        Spaghettis()->getPreferences().buildConcertinaPanel (*this);
        
        addAndMakeVisible (panel_);

        setOpaque (true); setSize (400, 500); startTimer (300);
    }
    
    ~PreferencesComponent() override
    {
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
        panel_.setBounds (getBoundsRemaining());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/*
public:
    bool tryGrabFocus() override
    {
        return tryGrabFocusForComponent (&panel_);
    }
*/

public:
    void expandPanel (int i)
    {
        panel_.expandPanelFully (panel_.getPanel (i), true);
    }
    
    void timerCallback() override
    {
        stopTimer(); expandPanel (0);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void addPanel (juce::PropertyPanel* p)
    {
        const int headerSize = Spaghettis()->getLookAndFeel().getPropertyPanelHeight() + 6;
        
        auto h = std::make_unique<PropertyHeader> (p->getName(), panel_.getNumPanels(), this);
        
        panel_.addPanel (-1, p, true);
        panel_.setCustomPanelHeader (p, h.release(), true);
        panel_.setPanelHeaderSize (p, headerSize);
    }
    
private:
    juce::ConcertinaPanel panel_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

