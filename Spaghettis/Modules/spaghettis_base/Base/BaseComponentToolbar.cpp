
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BaseComponentToolbar::BaseComponentToolbar (IconsFactory* factory,
    juce::PropertiesFile* propertiesFile,
    const juce::String& key) :
        propertiesFile_ (propertiesFile),
        keyName_ (key)
{
    if (factory) {
    //
    toolbar_ = std::make_unique<juce::Toolbar>();
    
    toolbar_->setVertical (false);
    toolbar_->setStyle (juce::Toolbar::ToolbarItemStyle::iconsOnly);
    toolbar_->setEditingActive (false);
    toolbar_->addDefaultItems (*factory);
    //
    }
}

BaseComponentToolbar::~BaseComponentToolbar()
{
    saveToolbarButtonsStates();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool BaseComponentToolbar::hasToolbar() const
{
    return toolbar_ != nullptr;
}

void BaseComponentToolbar::makeVisible (juce::Component* c)
{
    jassert (toolbar_); c->addAndMakeVisible (toolbar_.get());
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

IconsButton* fetchButton (juce::Toolbar* toolbar, int itemId)
{
    jassert (toolbar);
    
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

}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool BaseComponentToolbar::getButtonState (int itemId)
{
    IconsButton* b = fetchButton (toolbar_.get(), itemId);
        
    if (b && b->isToggle()) { return b->getState(); } else { jassertfalse; return false; }
}

void BaseComponentToolbar::setButtonState (int itemId, bool shouldBeOn)
{
    IconsButton* b = fetchButton (toolbar_.get(), itemId);
        
    if (b && b->isToggle()) { b->setState (shouldBeOn); } else { jassertfalse; }
}

bool BaseComponentToolbar::toggleButtonState (int itemId)
{
    const bool shouldBeOn = !getButtonState (itemId);
    
    setButtonState (itemId, shouldBeOn);
    
    return shouldBeOn;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseComponentToolbar::saveToolbarButtonsStates()
{
    if (toolbar_ && propertiesFile_ && keyName_.isNotEmpty()) {
    //
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
    
    if (save) { propertiesFile_->setValue (keyName_ + "Buttons", root.get()); }
    //
    }
}

void BaseComponentToolbar::loadToolbarButtonsStates()
{
    if (toolbar_ && propertiesFile_ && keyName_.isNotEmpty()) {
    //
    const std::unique_ptr<juce::XmlElement> root (propertiesFile_->getXmlValue (keyName_ + "Buttons"));
    
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
// MARK: -

juce::Rectangle<int> BaseComponentToolbar::getToolbarBounds() const
{
    jassert (toolbar_); return toolbar_->getBounds();
}

void BaseComponentToolbar::setToolbarBounds (const juce::Rectangle<int>& bounds)
{
    jassert (toolbar_); toolbar_->setBounds (bounds);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
