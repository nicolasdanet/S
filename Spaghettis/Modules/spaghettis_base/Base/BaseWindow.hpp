
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
    explicit BaseWindow (juce::ApplicationCommandManager& commandManager,
        juce::PropertiesFile* propertiesFile,
        const juce::String& name,
        const juce::String& key = juce::String());
    
    ~BaseWindow() override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getKeyName() const;

public:
    void requireMinimumHeight (int);
    
public:
    void setDirtyFlag (bool) const;
    void close();
    
public:
    bool isFullyInitialized() const;
    void grabFocus();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    void makeVisible (juce::Rectangle<int> window = juce::Rectangle<int>(), bool showAsLocked = false);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void timerCallback() override;
    void timerStart();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void activeWindowStatusChanged() override;
    void moved() override;
    void resized() override;

private:
    virtual void hasBeenMovedOrResized();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    void applyMinimumHeight();
    void showAsLocked();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    static void ensureAlertWindowsAlwaysOnTop();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static BaseWindow* getWindow (juce::Component*);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::ApplicationCommandManager& commandManager_;
    juce::PropertiesFile* propertiesFile_;
    
private:
    juce::String name_;
    juce::String keyName_;
    int timerCount_;
    int mimimumHeight_;
    bool initialized_;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseWindow)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------