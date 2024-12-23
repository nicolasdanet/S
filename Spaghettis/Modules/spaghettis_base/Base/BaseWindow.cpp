
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BaseWindow::BaseWindow (juce::ApplicationCommandManager& commandManager,
    juce::PropertiesFile* propertiesFile,
    const juce::String& name,
    const juce::String& key) :
        juce::DocumentWindow (name,
            Colours::fetchColour (Colours::windowsBackground),
            DocumentWindow::allButtons,
            false),
        commandManager_ (commandManager),
        propertiesFile_ (propertiesFile),
        name_ (name),
        keyName_ (key),
        timerCount_ (0),
        initialized_ (false)
{
    setUsingNativeTitleBar (true);
    setResizable (true, true);
    
    addKeyListener (commandManager_.getKeyMappings());
}

BaseWindow::~BaseWindow()
{
    removeKeyListener (commandManager_.getKeyMappings());
    
    if (propertiesFile_ && keyName_.isNotEmpty()) {
    //
    auto e = std::make_unique<juce::XmlElement> (Id::POSITION);
    
    e->setAttribute (Id::value, getWindowStateAsString());
    
    propertiesFile_->setValue (keyName_ + "Position", e.get());
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

juce::String BaseWindow::getKeyName() const
{
    return keyName_;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::showAsLocked()
{
    #if JUCE_LINUX
    
    setName (name_ + " (LOCKED)");
    
    #else
    
    juce::ComponentPeer* peer = getPeer();
    
    if (peer) { peer->setIcon (Icons::imagefromSVG ("icon_lock_svg")); }
    
    #endif
}

void BaseWindow::setDirtyFlag (bool isDirty) const
{
    juce::ComponentPeer* peer = getPeer();
    
    if (peer) { peer->setHasChangedSinceSaved (isDirty); }
}

void BaseWindow::close()
{
    closeButtonPressed();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

bool BaseWindow::isFullyInitialized() const
{
    return initialized_;
}

void BaseWindow::timerCallback()
{
    const int timerAttempts = 100;
    
    if (++timerCount_ > timerAttempts) { jassertfalse; stopTimer(); }
    else {
    //
    /* < https://forum.juce.com/t/getting-the-title-bar-height-in-a-windows-osx-app/38461/14 > */
    
    if (WindowsProperties::getTitleHeight (this)) {
    //
    BaseComponentFocus* c = dynamic_cast<BaseComponentFocus*> (getContentComponent());
    
    jassert (c);
    
    if (c->tryGrabFocus()) {
        stopTimer();
        commandManager_.commandStatusChanged();
        initialized_ = true;
    }
    //
    }
    //
    }
}

void BaseWindow::timerStart()
{
    const int primeInterval = 19; timerCount_ = 0; startTimer (primeInterval);
}

void BaseWindow::grabFocus()
{
    timerStart();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::activeWindowStatusChanged()
{
    if (isActiveWindow()) { ensureAlertWindowsAlwaysOnTop(); grabFocus(); }
}

void BaseWindow::moved()
{
    juce::ResizableWindow::moved();   ensureAlertWindowsAlwaysOnTop(); hasBeenMovedOrResized();
}

void BaseWindow::resized()
{
    juce::ResizableWindow::resized(); ensureAlertWindowsAlwaysOnTop(); hasBeenMovedOrResized();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::hasBeenMovedOrResized()
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::makeVisible (juce::Rectangle<int> window, bool locked)
{
    if (!window.isEmpty()) { setBounds (window); }
    else if (propertiesFile_ && keyName_.isNotEmpty()) {
    //
    const std::unique_ptr<juce::XmlElement> e (propertiesFile_->getXmlValue (keyName_ + "Position"));
    
    if (e && e->hasTagName (Id::POSITION) && e->hasAttribute (Id::value)) {
        const juce::String s = e->getStringAttribute (Id::value);
        if (s.isNotEmpty()) {
            restoreWindowStateFromString (s);
        }
    }
    //
    } else {
        jassertfalse;
    }
    
    setVisible (true); addToDesktop(); toFront (true);
    
    if (locked) { showAsLocked(); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::ensureAlertWindowsAlwaysOnTop()
{
    #if JUCE_LINUX
    
    AlertWindowRegister::getInstance()->ensureAlertWindowsAlwaysOnTop();
    
    #endif
}

BaseWindow* BaseWindow::getWindow (juce::Component* c)
{
    BaseWindow* w = dynamic_cast<BaseWindow*> (c->getTopLevelComponent());
        
    jassert (w != nullptr);
        
    return w;
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
