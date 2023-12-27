
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class BaseComponent :   public BaseComponentFocus,
                        public BaseComponentToolbar,
                        public juce::Component,
                        public juce::ApplicationCommandTarget {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    BaseComponent (IconsFactory* factory,
        juce::MenuBarModel& menu,
        juce::ApplicationCommandManager& commandManager,
        juce::PropertiesFile* propertiesFile = nullptr,
        const juce::String& key = juce::String()) :
            BaseComponentToolbar (factory, propertiesFile, key)
    {
        commandManager.registerAllCommandsForTarget (this);
        
        #if SPAGHETTIS_MENUBAR
        
        menubar_ = std::make_unique<juce::MenuBarComponent> (&menu);
        
        addAndMakeVisible (menubar_.get());
        
        #endif
        
        if (hasToolbar()) { makeVisible (this); }
        
        addMenuCommand (MenuCommand (Commands::closeWindow)
            .setInvoke (
                [this] (const auto&) { BaseWindow::getWindow (this)->close(); }
                ));

        setWantsKeyboardFocus (true);
    }

    ~BaseComponent()
    {
        juce::PopupMenu::dismissAllActiveMenus();
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void addMenuCommand (MenuCommand m)
    {
        commands_.set (m);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

private:
    juce::ApplicationCommandTarget* getNextCommandTarget() override
    {
        return findFirstTargetParentComponent();
    }

    void getAllCommands (juce::Array<juce::CommandID>& c) override
    {
        commands_.getCommands (c, false);
    }

    void getCommandInfo (juce::CommandID c, juce::ApplicationCommandInfo& r) override
    {
        commands_.getInfo (c, r);
    }

    bool perform (const juce::ApplicationCommandTarget::InvocationInfo& info) override
    {
        return commands_.perform (info);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    juce::Rectangle<int> setBoundsForBarsAndGetRemaining()
    {
        juce::Rectangle<int> b = getLocalBounds();

        #if SPAGHETTIS_MENUBAR
        
        menubar_->setBounds (b.removeFromTop (WindowsProperties::getMenuBarHeight()));
        
        #endif

        if (hasToolbar()) {
        //
        const int border = 2;
        
        auto t = b.removeFromBottom (WindowsProperties::getToolbarHeight());
        
        setToolbarBounds (t.reduced (border));
        //
        }
        
        return b;
    }
    
private:
    CommandsHandler commands_;
        
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

