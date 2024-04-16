
/* Copyright (c) 2021 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "BinaryData.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

class Icons : private juce::DeletedAtShutdown {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    enum IconsIds : int {
        add             = 1,
        autoscroll,
        clear,
        edit,
        error,
        find,
        inspector,
        message,
        presets,
        restore,
        run,
        synchronize,
        sortUp,
        sortDown
    };

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    Icons()
    {
        /* Must be in same order as enum above. */
        
        addIconAction       ("Add",         "icon_create_new_folder_svg");
        addIconToggleOn     ("Autoscroll",  "icon_system_update_alt_svg");
        addIconAction       ("Clear",       "icon_delete_svg");
        addIconAction       ("Edit",        "icon_construction_svg");
        addIconToggleOn     ("Error",       "icon_error_svg");
        addIconAction       ("Find",        "icon_location_on_svg");
        addIconToggleOff    ("Inspector",   "icon_info_svg");
        addIconToggleOn     ("Message",     "icon_mail_svg");
        addIconToggleOff    ("Presets",     "icon_tune_svg");
        addIconAction       ("Restore",     "icon_history_svg");
        addIconAction       ("Run",         "icon_visibility_svg");
        addIconAction       ("Synchronize", "icon_sync_svg");
        addIconAction       ("SortUp",      "icon_text_rotate_up_svg");
        addIconAction       ("SortDown",    "icon_text_rotation_down_svg");
    }

    ~Icons()
    {
        clearSingletonInstance();
    }

    JUCE_DECLARE_SINGLETON_SINGLETHREADED (Icons, true);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    int getItemId (const juce::String& name) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    juce::String getName (int itemId) const;
    std::unique_ptr<juce::Drawable> getIconOff (int itemId) const;
    std::unique_ptr<juce::Drawable> getIconOn (int itemId) const;
    bool isToggle (int itemId) const;
    bool getDefaultState (int itemId) const;

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

private:
    void addIconAction (const juce::String&, const char*);
    void addIconToggleOn (const juce::String&, const char*);
    void addIconToggleOff (const juce::String&, const char*);
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

public:
    static juce::Image imagefromSVG (const char*);
    
private:
    std::vector<IconsElement> drawables_;
};
 
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

