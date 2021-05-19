
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ApplicationWindow :   public juce::DocumentWindow,
                            public juce::Timer {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ApplicationWindow (const juce::String& name, const juce::String& keyName) :
        juce::DocumentWindow (name,
            Spaghettis()->getColour (juce::ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons,
            false),
        keyName_ (keyName)
    {
        setUsingNativeTitleBar (true);
        setResizable (true, true);
    }

    ~ApplicationWindow()
    {
        juce::PropertiesFile& preferences = Spaghettis()->getPreferences();
        
        preferences.setValue (keyName_, juce::var (getWindowStateAsString()));
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void timerCallback() override
    {
        DBG (getName());
        
        ApplicationComponent* c = dynamic_cast<ApplicationComponent*> (getContentComponent());
        
        timerCount_++; jassert (timerCount_ <= timerAttempts_);
        
        if (!c || c->tryGrabFocus() || timerCount_ > timerAttempts_) { stopTimer(); }
    }

private:
    void timerStart()
    {
        timerCount_ = 0; startTimer (timerInterval_);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void activeWindowStatusChanged() override
    {
        if (isActiveWindow()) { timerStart(); }
    }

    void makeVisible()
    {
        juce::PropertiesFile& preferences = Spaghettis()->getPreferences();
        
        const juce::String s = preferences.getValue (keyName_);
        
        if (s.isNotEmpty()) { restoreWindowStateFromString (s); }
        
        setVisible (true); addToDesktop(); toFront (true);
    }

private:
    juce::String keyName_;
    int timerCount_;
    
private:
    static const int timerInterval_ = 20;
    static const int timerAttempts_ = 10;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationWindow)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
