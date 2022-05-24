
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BaseComponent::BaseComponent (IconsFactory* factory, const juce::String& s) : keyName_ (s)
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

BaseComponent::~BaseComponent()
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

bool BaseComponent::tryGrabFocusForComponent (juce::Component *c)
{
    c->grabKeyboardFocus();
    
    if (juce::ModalComponentManager::getInstance()->getNumModalComponents() > 0) { return true; }
    else {
        return c->hasKeyboardFocus (true);
    }
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

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseComponent::saveToolbarButtonsStates()
{
    if (toolbar_ && keyName_.isNotEmpty()) {
    //
    juce::PropertiesFile& p = Spaghettis()->getProperties();
    
    auto root   = std::make_unique<juce::XmlElement> (Ids::BUTTONS);
    const int n = toolbar_->getNumItems();
    bool save   = false;
    
    for (int i = 0; i < n; ++i) {
    //
    IconsButton* b = dynamic_cast<IconsButton*> (toolbar_->getItemComponent (i));
    if (b && b->isToggle()) {
        juce::XmlElement* e = root->createNewChildElement (Ids::BUTTON);
        e->setAttribute (Ids::item,  b->getName());
        e->setAttribute (Ids::state, b->getState());
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
    juce::PropertiesFile& p = Spaghettis()->getProperties();
    
    const std::unique_ptr<juce::XmlElement> root (p.getXmlValue (keyName_ + "Buttons"));
    
    if (root && root->hasTagName (Ids::BUTTONS)) {

        for (auto* e : root->getChildWithTagNameIterator (Ids::BUTTON)) {
            if (e->hasAttribute (Ids::item) && e->hasAttribute (Ids::state)) {
                const int itemId = Icons::getInstance().getItemId (e->getStringAttribute (Ids::item));
                const bool state = e->getBoolAttribute (Ids::state);
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
// MARK: -

juce::Rectangle<int> BaseComponent::setBarsBoundsAndGetRemaining()
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

void BaseComponent::addMenuBarCommand (juce::CommandID command, std::function<void()> f)
{
    commands_.set (command, f);
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
    commands_.getAllCommands (c);
}

void BaseComponent::getCommandInfo (juce::CommandID c, juce::ApplicationCommandInfo& r)
{
    commands_.getCommandInfo (c, r);
}

bool BaseComponent::perform (const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    return commands_.perform (info);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
