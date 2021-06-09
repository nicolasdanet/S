
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
    ApplicationComponent (IconsFactory *factory = nullptr)
    {
        Spaghettis()->getCommandManager().registerAllCommandsForTarget (this);
        
        addKeyListener (Spaghettis()->getCommandManager().getKeyMappings());
        
        #if SPAGHETTIS_MENUBAR
        
        menubar_ = std::make_unique<juce::MenuBarComponent> (&Spaghettis()->getMenuBarModel());
        
        addAndMakeVisible (menubar_.get());
        
        #endif
        
        if (factory) {
        //
        toolbar_ = std::make_unique<juce::Toolbar>();
        
        toolbar_->setVertical (false);
        toolbar_->setStyle (juce::Toolbar::ToolbarItemStyle::iconsOnly);
        toolbar_->setEditingActive (false);
        toolbar_->addDefaultItems (*factory);
        
        addAndMakeVisible (toolbar_.get());
        //
        }
        
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
    juce::Rectangle<int> getBoundsRemaining()
    {
        juce::Rectangle<int> b = getLocalBounds();

        #if SPAGHETTIS_MENUBAR
        
        menubar_->setBounds (b.removeFromTop (Spaghettis()->getLookAndFeel().getDefaultMenuBarHeight()));
        
        #endif

        if (toolbar_) {
        //
        #if JUCE_LINUX
        const int border = 2;
        #else
        const int border = 3;
        #endif
        
        auto t = b.removeFromBottom (Spaghettis()->getLookAndFeel().getToolbarHeight());
        
        toolbar_->setBounds (t.reduced (border));
        //
        }
        
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

private:
    std::unique_ptr<juce::Toolbar> toolbar_;
    
#if SPAGHETTIS_MENUBAR

private:
    std::unique_ptr<juce::MenuBarComponent> menubar_;

#endif

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ApplicationComponent)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

