
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ApplicationWindow :   public  juce::DocumentWindow,
                            private juce::Timer {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit ApplicationWindow (const juce::String& name, const juce::String& keyName) :
        juce::DocumentWindow (name,
            Spaghettis()->getColour (Colours::windowBackground),
            DocumentWindow::allButtons,
            false),
        keyName_ (keyName)
    {
        setUsingNativeTitleBar (true);
        setResizable (true, true);
    }

    explicit ApplicationWindow (const juce::String& name) : ApplicationWindow (name, name + "Window")
    {
    }
    
    ~ApplicationWindow()
    {
        juce::PropertiesFile& p = Spaghettis()->getProperties();
        
        auto e = std::make_unique<juce::XmlElement> (Ids::POSITION);
        
        e->setAttribute (Ids::value, getWindowStateAsString());
        
        p.setValue (keyName_ + "Position", e.get());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    juce::String getKeyName() const
    {
        return keyName_;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void timerCallback() override
    {
        const int timerAttempts = 10;
                
        ApplicationComponent* c = dynamic_cast<ApplicationComponent*> (getContentComponent());
        
        timerCount_++; jassert (timerCount_ <= timerAttempts);
        
        if (!c || c->tryGrabFocus() || timerCount_ > timerAttempts) {
            Spaghettis()->getLookAndFeel().getWindowTitleHeight (this);
            stopTimer();
        }
    }

private:
    void timerStart()
    {
        const int timerInterval = 20; timerCount_ = 0; startTimer (timerInterval);
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
        juce::PropertiesFile& p = Spaghettis()->getProperties();
        
        const std::unique_ptr<juce::XmlElement> e (p.getXmlValue (keyName_ + "Position"));
        
        if (e && e->hasTagName (Ids::POSITION) && e->hasAttribute (Ids::value)) {
            const juce::String s = e->getStringAttribute (Ids::value);
            if (s.isNotEmpty()) {
                restoreWindowStateFromString (s);
            }
        }
        
        setVisible (true); addToDesktop(); toFront (true);
    }

    void makeVisibleWithMinimumHeight (int h)
    {
        makeVisible(); setMinimumHeight (h);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void setMinimumHeight (int h)
    {
        juce::ComponentBoundsConstrainer *c = getConstrainer();
        
        jassert (c);
        
        #if SPAGHETTIS_MENUBAR
        
        h += Spaghettis()->getLookAndFeel().getDefaultMenuBarHeight();
        
        #endif
        
        h += Spaghettis()->getLookAndFeel().getWindowTitleHeight (this);
        
        c->setMinimumHeight (h);
    }

private:
    juce::String keyName_;
    int timerCount_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationWindow)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
