
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class BaseComponent :   public juce::Component,
                        public juce::ApplicationCommandTarget {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    BaseComponent (IconsFactory* factory, const juce::String& s = juce::String());
    
    ~BaseComponent() override;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    static bool tryGrabFocusForComponent (juce::Component*);
    
public:
    virtual bool tryGrabFocus() = 0;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool getButtonState (int itemId);
    void loadToolbarButtonsStates();
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void setButtonState (int itemId, bool shouldBeOn);
    void saveToolbarButtonsStates();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    juce::Rectangle<int> setBoundsForBarsAndGetRemaining();
    juce::Rectangle<int> getBoundsForToolbar() const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addMenuBarCommand (juce::CommandID, std::function<void()>);
    void addMenuBarCommand (juce::CommandID, std::function<void()>, std::function<bool()>);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (juce::Array<juce::CommandID>&) override;
    void getCommandInfo (juce::CommandID, juce::ApplicationCommandInfo&) override;
    bool perform (const juce::ApplicationCommandTarget::InvocationInfo&) override;

private:
    juce::String keyName_;
    std::unique_ptr<juce::Toolbar> toolbar_;
    BaseCommands commands_;
        
#if SPAGHETTIS_MENUBAR

private:
    std::unique_ptr<juce::MenuBarComponent> menubar_;

#endif

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

