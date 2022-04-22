
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
    explicit BaseComponent (IconsFactory* factory, const juce::String& s = juce::String()) : keyName_ (s)
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
    
    ~BaseComponent() override
    {
        #if SPAGHETTIS_MENUBAR
        
        juce::PopupMenu::dismissAllActiveMenus();
        
        #endif
        
        saveToolbarButtonsStates();
        
        removeKeyListener (Spaghettis()->getCommandManager().getKeyMappings());
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    static bool tryGrabFocusForComponent (juce::Component *c)
    {
        c->grabKeyboardFocus();
        
        if (juce::ModalComponentManager::getInstance()->getNumModalComponents() > 0) { return true; }
        else {
            return c->hasKeyboardFocus (true);
        }
    }
    
public:
    virtual bool tryGrabFocus() = 0;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    bool getButtonState (int itemId);
    void loadToolbarButtonsStates();

private:
    void setButtonState (int itemId, bool shouldBeOn);
    void saveToolbarButtonsStates();

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

protected:
    juce::Rectangle<int> setBarsBoundsAndGetRemaining()
    {
        juce::Rectangle<int> b = getLocalBounds();

        #if SPAGHETTIS_MENUBAR
        
        menubar_->setBounds (b.removeFromTop (Spaghettis()->getLookAndFeel().getDefaultMenuBarHeight()));
        
        #endif

        if (toolbar_) {
        //
        const int border = 2;
        
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
        commands_.getAllCommands (c);
    }

    void getCommandInfo (const juce::CommandID c, juce::ApplicationCommandInfo& r) override
    {
        commands_.getCommandInfo (c, r);
    }

    bool perform (const juce::ApplicationCommandTarget::InvocationInfo& info) override
    {
        return commands_.perform (info);
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static void setDefaultWithSize (juce::Component *c, int w = 0, int h = 0)
    {
        c->setOpaque (true); c->setSize (w, h);
    }
    
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

