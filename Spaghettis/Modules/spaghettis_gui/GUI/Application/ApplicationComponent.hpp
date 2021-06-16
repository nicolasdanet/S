
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
    ApplicationComponent (const juce::String& keyName, IconsFactory *factory = nullptr) : keyName_ (keyName)
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
        saveToolbarButtonsStates(); removeKeyListener (Spaghettis()->getCommandManager().getKeyMappings());
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

public:
    bool getIconToggleState (int itemId)
    {
        IconsButton* b = fetchButton (toolbar_.get(), itemId);
        
        if (b && b->isToggle()) {
        //
        return b->getToggleState();
        //
        }
        
        jassertfalse; return false;
    }
    
    void setIconToggleState (int itemId, bool shouldBeOn)
    {
        IconsButton* b = fetchButton (toolbar_.get(), itemId);
        
        if (b && b->isToggle()) {
        //
        return b->setToggleState (shouldBeOn, juce::dontSendNotification);
        //
        } else { jassertfalse; }
    }

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void saveToolbarButtonsStates()
    {
        if (toolbar_) {
        //
        juce::PropertiesFile& preferences = Spaghettis()->getPreferences();
        
        auto root   = std::make_unique<juce::XmlElement> ("BUTTONS");
        const int n = toolbar_->getNumItems();
        bool save   = false;
        
        for (int i = 0; i < n; ++i) {
        //
        IconsButton* b = dynamic_cast<IconsButton*> (toolbar_->getItemComponent (i));
        if (b && b->isToggle()) {
            juce::XmlElement* e = root->createNewChildElement ("BUTTON");
            e->setAttribute (Ids::item,  b->getName());
            e->setAttribute (Ids::state, b->getToggleState());
            save = true;
        }
        //
        }
        
        if (save) { preferences.setValue (keyName_ + "Buttons", root.get()); }
        //
        }
    }
    
    void loadToolbarButtonsStates()
    {
        if (toolbar_) {
        //
        juce::PropertiesFile& preferences = Spaghettis()->getPreferences();
        
        std::unique_ptr<juce::XmlElement> root = preferences.getXmlValue (keyName_ + "Buttons");
        
        if (root && root->hasTagName ("BUTTONS")) {
        //
        for (auto* e : root->getChildWithTagNameIterator ("BUTTON")) {
            if (e->hasAttribute (Ids::item) && e->hasAttribute (Ids::state)) {
                const int itemId = Icons::getInstance().getItemId (e->getStringAttribute (Ids::item));
                const bool state = e->getBoolAttribute (Ids::state);
                setIconToggleState (itemId, state);
            }
        }
        //
        }
        //
        }
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* A free function to fetch icons with item identifiers. */

private:
    static IconsButton* fetchButton (juce::Toolbar* toolbar, int itemId)
    {
        const int n = toolbar->getNumItems();
        
        for (int i = 0; i < n; ++i) {
        //
        if (toolbar->getItemId (i) == itemId) {
            IconsButton* b = dynamic_cast<IconsButton*> (toolbar->getItemComponent (i));
            jassert (b);
            return b;
        }
        //
        }
        
        return nullptr;
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

/* Handy reusable free functions for list box components. */

protected:
    static void listBoxInitialize (juce::ListBox& listBox, bool multiple)
    {
        const int h = static_cast<int> (Spaghettis()->getLookAndFeel().getFontConsole().getHeight() * 1.5);
        
        listBox.setMultipleSelectionEnabled (multiple);
        listBox.setClickingTogglesRowSelection (true);
        listBox.setRowHeight (h);
        listBox.getViewport()->setScrollBarsShown (false, false, true, true);
    }
    
    static int lisBoxGetNumberOfRowsToDraw (int contentSize)
    {
        return juce::jmax (32, contentSize);
    }
    
    static void listBoxUpdateProceed (juce::ListBox& listBox, int size, bool updateRows)
    {
        if (updateRows) {
            listBox.updateContent();
            listBox.deselectAllRows();
            listBox.repaint();
        }
        
        {
            int i = listBox.getRowContainingPosition (0, 0);
            int j = listBox.getRowContainingPosition (0, listBox.getBottom());
            
            if (i >= 0) {
                const bool show = (j - i) < size;
                listBox.getViewport()->setScrollBarsShown (show, show, true, true);
            }
        }
    }
    
    template <class T> static void listBoxUpdate (juce::ListBox& listBox, T& c, bool updateRows)
    {
        listBoxUpdateProceed (listBox, static_cast<int> (c.size()), updateRows);
    }
    
private:
    juce::String keyName_;
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

