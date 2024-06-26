
/* Copyright (c) 2021 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

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
    explicit MenuModel (juce::ApplicationCommandManager* m)
    {
        jassert (m != nullptr);
        commandManager_ = m;
        setApplicationCommandManagerToWatch (commandManager_);
    }
    
    ~MenuModel() = default;

public:
    juce::StringArray getMenuBarNames() override
    {
        return { NEEDS_TRANS ("File"), NEEDS_TRANS ("Edit"), NEEDS_TRANS ("View"), NEEDS_TRANS ("Media") };
    }

    juce::PopupMenu getMenuForIndex (int, const juce::String&) override;

    void menuItemSelected (int, int) override;

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

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
