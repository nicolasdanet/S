
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Console : public Window {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Console (juce::String name) : Window (name)
    {
        setContentOwned (new ConsoleComponent(), true);
        
        juce::PropertiesFile* preferences = Spaghettis()->getPreferences();
        
        juce::String s = preferences->getValue ("ConsoleWindowPosition");
        
        if (s.isNotEmpty()) { restoreWindowStateFromString (s); }
    }

    ~Console() override
    {
        juce::PropertiesFile* preferences = Spaghettis()->getPreferences();
        
        preferences->setValue ("ConsoleWindowPosition", juce::var (getWindowStateAsString()));
    }
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    void closeButtonPressed() override
    {
        #if SPAGHETTIS_MENUBAR
        
        juce::PopupMenu::dismissAllActiveMenus();
        
        #endif
        
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Console)
};
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
