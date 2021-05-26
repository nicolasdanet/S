
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class ApplicationComponent :    public juce::Component,
                                public juce::ApplicationCommandTarget {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    ApplicationComponent()
    {
        Spaghettis()->getCommandManager().registerAllCommandsForTarget (this);
        
        addKeyListener (Spaghettis()->getCommandManager().getKeyMappings());
        
        #if SPAGHETTIS_MENUBAR
        
        menuBar_ = std::make_unique<juce::MenuBarComponent> (&Spaghettis()->getMenuBarModel());
        
        addAndMakeVisible (menuBar_.get());
        
        #endif
        
        setWantsKeyboardFocus (true);
    }
    
    ~ApplicationComponent() override
    {
        removeKeyListener (Spaghettis()->getCommandManager().getKeyMappings());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static bool tryGrabFocusForComponent (juce::Component *c)
    {
        c->grabKeyboardFocus(); return c->hasKeyboardFocus (true);
    }
    
public:
    virtual bool tryGrabFocus()
    {
        return tryGrabFocusForComponent (this);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    juce::Rectangle<int> getBoundsMenubarResized() const
    {
        juce::Rectangle<int> b = getLocalBounds();

        #if SPAGHETTIS_MENUBAR
        
        menuBar_->setBounds (b.removeFromTop (Spaghettis()->getLookAndFeel().getDefaultMenuBarHeight()));
        
        #endif
        
        return b;
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::ApplicationCommandTarget* getNextCommandTarget() override
    {
        return findFirstTargetParentComponent();
    }

    void getAllCommands (juce::Array<juce::CommandID>& c) override
    {
        Commands::getAllCommands (c);
    }

    void getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r) override
    {
        Commands::getCommandInfo (c, r);
    }

    bool perform (const juce::ApplicationCommandTarget::InvocationInfo& info) override
    {
        return Commands::perform (info);
    }

#if SPAGHETTIS_MENUBAR

private:
    std::unique_ptr<juce::MenuBarComponent> menuBar_;

#endif

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

