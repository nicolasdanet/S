
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class PreferencesComponent :    public  ApplicationComponent,
                                private juce::ChangeListener,
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
    void timerCallback() override
    {
        stopTimer(); panel_.expandPanelFully (panel_.getPanel (0), true);
    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        const auto f = [source] (const std::unique_ptr<PropertyHeader>& h) { return h.get() == source; };
        const auto p = std::find_if (headers_.begin(), headers_.end(), f);
        const int i  = static_cast<int>(std::distance (headers_.begin(), p));

        if (i != static_cast<int> (headers_.size())) { panel_.expandPanelFully (panel_.getPanel (i), true); }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void addPanel (juce::PropertyPanel* p)
    {
        const int headerSize = Spaghettis()->getLookAndFeel().getPropertyPanelHeight() + 6;
        
        auto h = std::make_unique<PropertyHeader> (p->getName());
        
        h->addChangeListener (this);
        
        panel_.addPanel (-1, p, true);
        panel_.setCustomPanelHeader (p, h.get(), false);
        panel_.setPanelHeaderSize (p, headerSize);
        
        headers_.push_back (std::move (h));
    }
    
private:
    juce::ConcertinaPanel panel_;
    std::vector<std::unique_ptr<PropertyHeader>> headers_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

