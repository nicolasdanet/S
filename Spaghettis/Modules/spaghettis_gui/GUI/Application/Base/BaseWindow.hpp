
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class BaseWindow :  public  juce::DocumentWindow,
                    private juce::Timer {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    explicit BaseWindow (const juce::String& name, const juce::String& s = juce::String()) :
        juce::DocumentWindow (name,
            Spaghettis()->getColour (Colours::windowBackground),
            DocumentWindow::allButtons,
            false),
        keyName_ (s),
        timerCount_ (0),
        mimimumHeight_ (0)
    {
        setUsingNativeTitleBar (true);
        setResizable (true, true);
    }
    
    ~BaseWindow() override
    {
        if (keyName_.isNotEmpty()) {
        //
        juce::PropertiesFile& p = Spaghettis()->getProperties();
        
        auto e = std::make_unique<juce::XmlElement> (Ids::POSITION);
        
        e->setAttribute (Ids::value, getWindowStateAsString());
        
        p.setValue (keyName_ + "Position", e.get());
        //
        }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    juce::String getPropertiesKeyName() const
    {
        return keyName_;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void setDirtyFlag (bool isDirty) const
    {
        juce::ComponentPeer* peer = getPeer();
        
        if (peer) {
            peer->setHasChangedSinceSaved (isDirty);
        }
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void timerCallback() override;

private:
    void timerStart()
    {
        const int primeInterval = 19; timerCount_ = 0; startTimer (primeInterval);
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void activeWindowStatusChanged() override
    {
        if (isActiveWindow()) { ensureAlertWindowsAlwaysOnTop(); timerStart(); }
    }

    void moved() override
    {
        juce::ResizableWindow::moved(); ensureAlertWindowsAlwaysOnTop();
    }
    
    void resized() override
    {
        juce::ResizableWindow::resized(); ensureAlertWindowsAlwaysOnTop();
    }
    
    void makeVisible (juce::Rectangle<int> window = juce::Rectangle<int>());

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void requireMinimumHeight (int h)
    {
        jassert (h > 0); mimimumHeight_ = h;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void ensureAlertWindowsAlwaysOnTop()
    {
        #if JUCE_LINUX
        
        Spaghettis()->getAlertRegister().ensureAlertWindowsAlwaysOnTop();
        
        #endif
    }

    static void updateMenuBar()
    {
        Spaghettis()->getCommandManager().commandStatusChanged();
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void setMinimumHeight (int h);

private:
    juce::String keyName_;
    int timerCount_;
    int mimimumHeight_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseWindow)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
