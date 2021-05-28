
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
        return { NEEDS_TRANS ("File"), NEEDS_TRANS ("Media") };
    }

    juce::PopupMenu getMenuForIndex (int n, const juce::String&) override
    {
        juce::PopupMenu menu;
        
        if (n == 0) {
        //
        menu.addCommandItem (commandManager_, Commands::fileOpen);
        menu.addSeparator();
        #if SPAGHETTIS_MENUBAR
        menu.addCommandItem (commandManager_, Commands::preferences);
        #endif
        menu.addCommandItem (commandManager_, Commands::paths);
        menu.addSeparator();
        menu.addCommandItem (commandManager_, Commands::rescan);
        menu.addCommandItem (commandManager_, Commands::rescanLogged);
        
        #if SPAGHETTIS_MENUBAR
        menu.addSeparator();
        menu.addCommandItem (commandManager_, juce::StandardApplicationCommandIDs::quit);
        #endif
        //
        } else if (n == 1) {
        //
        menu.addCommandItem (commandManager_, Commands::dspSwitch);
        //
        }
        
        return menu;
    }

    void menuItemSelected (int, int) override
    {
    }

#if ! ( SPAGHETTIS_MENUBAR )

public:
    static juce::PopupMenu createAppleMenu (juce::ApplicationCommandManager* m)
    {
        juce::PopupMenu menu;
        
        menu.addCommandItem (m, Commands::preferences);
        
        return menu;
    }

#endif

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
