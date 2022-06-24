
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

BaseWindow::BaseWindow (const juce::String& name, const juce::String& s) :
    juce::DocumentWindow (name,
        Spaghettis()->getColour (Colours::windowBackground),
        DocumentWindow::allButtons,
        false),
    keyName_ (s),
    timerCount_ (0),
    mimimumHeight_ (0)
{
    setUsingNativeTitleBar (true);
    setResizable (true, true);
}

BaseWindow::~BaseWindow()
{
    if (keyName_.isNotEmpty()) {
    //
    juce::PropertiesFile& p = Spaghettis()->getProperties();
    
    auto e = std::make_unique<juce::XmlElement> (Ids::POSITION);
    
    e->setAttribute (Ids::value, getWindowStateAsString());
    
    p.setValue (keyName_ + "Position", e.get());
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

void BaseWindow::setDirtyFlag (bool isDirty) const
{
    juce::ComponentPeer* peer = getPeer();
    
    if (peer) {
        peer->setHasChangedSinceSaved (isDirty);
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::timerCallback()
{
    const int timerAttempts = 10;
    
    if (++timerCount_ > timerAttempts) { jassertfalse; stopTimer(); }
    else {
    //
    /* < https://forum.juce.com/t/getting-the-title-bar-height-in-a-windows-osx-app/38461/14 > */
    
    const int h = Spaghettis()->getLookAndFeel().getWindowTitleHeight (this);
    
    if (h != 0) {
    //
    BaseComponent* c = dynamic_cast<BaseComponent*> (getContentComponent());
        
    if (!c || c->tryGrabFocus()) { applyMinimumHeight (h); updateMenuBar(); stopTimer(); }
    //
    }
    //
    }
}

void BaseWindow::timerStart()
{
    const int primeInterval = 19; timerCount_ = 0; startTimer (primeInterval);
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::activeWindowStatusChanged()
{
    if (isActiveWindow()) { ensureAlertWindowsAlwaysOnTop(); timerStart(); }
}

void BaseWindow::moved()
{
    juce::ResizableWindow::moved();   ensureAlertWindowsAlwaysOnTop(); hasBeenChanged();
}

void BaseWindow::resized()
{
    juce::ResizableWindow::resized(); ensureAlertWindowsAlwaysOnTop(); hasBeenChanged();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::hasBeenChanged()
{
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::makeVisible (juce::Rectangle<int> window)
{
    if (!window.isEmpty()) { setBounds (window); }
    else if (keyName_.isNotEmpty()) {
    //
    juce::PropertiesFile& p = Spaghettis()->getProperties();
    
    const std::unique_ptr<juce::XmlElement> e (p.getXmlValue (keyName_ + "Position"));
    
    if (e && e->hasTagName (Ids::POSITION) && e->hasAttribute (Ids::value)) {
        const juce::String s = e->getStringAttribute (Ids::value);
        if (s.isNotEmpty()) {
            restoreWindowStateFromString (s);
        }
    }
    //
    } else {
        jassertfalse;
    }

    setVisible (true); addToDesktop(); toFront (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::applyMinimumHeight (int h)
{
    if (mimimumHeight_) {
    //
    int n = mimimumHeight_;
    
    juce::ComponentBoundsConstrainer *c = getConstrainer();
    
    jassert (c);
    
    #if SPAGHETTIS_MENUBAR
    
    n += Spaghettis()->getLookAndFeel().getDefaultMenuBarHeight();
    
    #endif
    
    n += h;
    
    c->setMinimumHeight (n);
    //
    }
}

void BaseWindow::requireMinimumHeight (int h)
{
    jassert (h > 0); mimimumHeight_ = h;
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::ensureAlertWindowsAlwaysOnTop()
{
    #if JUCE_LINUX
    
    Spaghettis()->getAlertRegister().ensureAlertWindowsAlwaysOnTop();
    
    #endif
}

void BaseWindow::updateMenuBar()
{
    Spaghettis()->getCommandManager().commandStatusChanged();
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
