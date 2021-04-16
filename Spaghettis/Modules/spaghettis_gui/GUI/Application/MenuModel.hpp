
/* Copyright (c) 2021 Nicolas Danet. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class MenuModel : public juce::MenuBarModel {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    MenuModel (juce::ApplicationCommandManager* m)
    {
        jassert (m != nullptr);
        commandManager_ = m;
        setApplicationCommandManagerToWatch (commandManager_);
    }
    
    ~MenuModel() = default;

public:
    juce::StringArray getMenuBarNames() override
    {
        return juce::StringArray (NEEDS_TRANS ("File"));
    }

    juce::PopupMenu getMenuForIndex (int n, const juce::String&) override
    {
        juce::PopupMenu menu;
        
        if (n == 0) {
        //
        menu.addCommandItem (commandManager_, Commands::fileOpen);
        
        #if SPAGHETTIS_MENUBAR
        
        menu.addSeparator();
        menu.addCommandItem (commandManager_, juce::StandardApplicationCommandIDs::quit);
        
        #endif
        //
        }
        
        return menu;
    }

    void menuItemSelected (int, int) override
    {
    }
    
private:
    juce::ApplicationCommandManager* commandManager_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuModel)
};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

} // namespace spaghettis

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
