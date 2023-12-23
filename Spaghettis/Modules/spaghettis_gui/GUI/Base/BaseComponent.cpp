
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BaseComponent::BaseComponent (IconsFactory* factory, const juce::String& key) : keyName_ (key)
{
    Spaghettis()->getCommandManager().registerAllCommandsForTarget (this);
    
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
    
    addMenuCommand (MenuCommand (Commands::closeWindow)
        .setInvoke ([this] (const auto&) { BaseWindow::getWindow (this)->close(); }));

    setWantsKeyboardFocus (true);
}

BaseComponent::~BaseComponent()
{
    juce::PopupMenu::dismissAllActiveMenus();
    
    saveToolbarButtonsStates();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

IconsButton* fetchButton (juce::Toolbar* toolbar, int itemId)
{
    jassert (toolbar); const int n = toolbar->getNumItems();
    
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

}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool BaseComponent::getButtonState (int itemId)
{
    IconsButton* b = fetchButton (toolbar_.get(), itemId);
        
    if (b && b->isToggle()) { return b->getState(); } else { jassertfalse; return false; }
}

void BaseComponent::setButtonState (int itemId, bool shouldBeOn)
{
    IconsButton* b = fetchButton (toolbar_.get(), itemId);
        
    if (b && b->isToggle()) { b->setState (shouldBeOn); } else { jassertfalse; }
}

bool BaseComponent::toggleButtonState (int itemId)
{
    const bool shouldBeOn = !getButtonState (itemId);
    
    setButtonState (itemId, shouldBeOn);
    
    return shouldBeOn;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void BaseComponent::saveToolbarButtonsStates()
{
    if (toolbar_ && keyName_.isNotEmpty()) {
    //
    juce::PropertiesFile& p = Spaghettis()->getPropertiesFile();
    
    auto root   = std::make_unique<juce::XmlElement> (Id::BUTTONS);
    const int n = toolbar_->getNumItems();
    bool save   = false;
    
    for (int i = 0; i < n; ++i) {
    //
    IconsButton* b = dynamic_cast<IconsButton*> (toolbar_->getItemComponent (i));
    if (b && b->isToggle()) {
        juce::XmlElement* e = root->createNewChildElement (Id::BUTTON);
        e->setAttribute (Id::item,  b->getName());
        e->setAttribute (Id::state, b->getState());
        save = true;
    }
    //
    }
    
    if (save) { p.setValue (keyName_ + "Buttons", root.get()); }
    //
    }
}

void BaseComponent::loadToolbarButtonsStates()
{
    if (toolbar_ && keyName_.isNotEmpty()) {
    //
    juce::PropertiesFile& p = Spaghettis()->getPropertiesFile();
    
    const std::unique_ptr<juce::XmlElement> root (p.getXmlValue (keyName_ + "Buttons"));
    
    if (root && root->hasTagName (Id::BUTTONS)) {

        for (auto* e : root->getChildWithTagNameIterator (Id::BUTTON)) {
            if (e->hasAttribute (Id::item) && e->hasAttribute (Id::state)) {
                const int itemId = Icons::getInstance()->getItemId (e->getStringAttribute (Id::item));
                const bool state = e->getBoolAttribute (Id::state);
                setButtonState (itemId, state);
            }
        }

    } else {    /* Default states. */
    
        const int n = toolbar_->getNumItems();

        for (int i = 0; i < n; ++i) {
            IconsButton* b = dynamic_cast<IconsButton*> (toolbar_->getItemComponent (i));
            if (b && b->isToggle()) {
                b->setState (b->getDefaultState());
            }
        }
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

juce::Rectangle<int> BaseComponent::getToolbarBounds() const
{
    return toolbar_->getBounds();
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

    if (toolbar_) {
    //
    const int border = 2;
    
    auto t = b.removeFromBottom (WindowsProperties::getToolbarHeight());
    
    toolbar_->setBounds (t.reduced (border));
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
