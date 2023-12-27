
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BaseComponent::BaseComponent (IconsFactory* factory,
    juce::MenuBarModel& menu,
    juce::ApplicationCommandManager& commandManager,
    juce::PropertiesFile* propertiesFile,
    const juce::String& key) :
        BaseComponentToolbar (factory, propertiesFile, key)
{
    commandManager.registerAllCommandsForTarget (this);
    
    #if SPAGHETTIS_MENUBAR
    
    menubar_ = std::make_unique<juce::MenuBarComponent> (&menu);
    
    addAndMakeVisible (menubar_.get());
    
    #endif
    
    if (hasToolbar()) { makeVisible (this); }
    
    addMenuCommand (MenuCommand (Commands::closeWindow)
        .setInvoke ([this] (const auto&) { BaseWindow::getWindow (this)->close(); }));

    setWantsKeyboardFocus (true);
}

BaseComponent::~BaseComponent()
{
    juce::PopupMenu::dismissAllActiveMenus();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseComponent::addMenuCommand (MenuCommand m)
{
    commands_.set (m);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::ApplicationCommandTarget* BaseComponent::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void BaseComponent::getAllCommands (juce::Array<juce::CommandID>& c)
{
    commands_.getCommands (c, false);
}

void BaseComponent::getCommandInfo (juce::CommandID c, juce::ApplicationCommandInfo& r)
{
    commands_.getInfo (c, r);
}

bool BaseComponent::perform (const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    return commands_.perform (info);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::Rectangle<int> BaseComponent::setBoundsForBarsAndGetRemaining()
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BaseComponent* BaseComponent::getComponent (juce::Component* c)
{
    BaseWindow* w    = BaseWindow::getWindow (c);
    BaseComponent* t = dynamic_cast<BaseComponent*> (w->getContentComponent());
    
    jassert (t != nullptr);
    
    return t;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
