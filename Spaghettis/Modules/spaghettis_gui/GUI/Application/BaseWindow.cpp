
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

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
        
    if (!c || c->tryGrabFocus()) { setMinimumHeightCallback (mimimumHeight_); stopTimer(); }
    //
    }
    //
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::makeVisible()
{
    if (keyName_.isNotEmpty()) {
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
    }
    
    // setBoundsConstrained
    
    setVisible (true); addToDesktop(); toFront (true);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void BaseWindow::setMinimumHeightCallback (int h)
{
    if (h) {
    //
    juce::ComponentBoundsConstrainer *c = getConstrainer();
    
    jassert (c);
    
    #if SPAGHETTIS_MENUBAR
    
    h += Spaghettis()->getLookAndFeel().getDefaultMenuBarHeight();
    
    #endif
    
    h += Spaghettis()->getLookAndFeel().getWindowTitleHeight (this);
    
    c->setMinimumHeight (h);
    //
    }
}
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
