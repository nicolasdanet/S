
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
    PreferencesComponent (const juce::String& keyName) : ApplicationComponent (keyName),
        expanded_ (0),
        expandedLast_ (0)
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
        if (panel_.getNumPanels() > 1) {
        //
        if (expanded_ == i) { jassert (expandedLast_ != i); expandPanel (expandedLast_); }
        else {
            expandedLast_   = expanded_;
            expanded_       = i;
            panel_.expandPanelFully (panel_.getPanel (i), true);
        }
        //
        }
    }
    
    bool isExpanded (int i)
    {
        return (i == expanded_);
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
        const int i = panel_.getNumPanels();
        auto h = std::make_unique<PropertyHeader> (p->getName(), i, this);
        
        panel_.addPanel (-1, p, true);
        panel_.setCustomPanelHeader (p, h.release(), true);
        panel_.setPanelHeaderSize (p, headerSize);
        
        expanded_ = i;
    }
    
private:
    juce::ConcertinaPanel panel_;
    int expanded_;
    int expandedLast_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

